#include <assert.h>
#include "globalflags.h"
#include "util.h"
#include "tables.h"
#include "reservoir.h"
#include "quantize-pvt.h"

FLOAT masking_lower=1;
int convert_mdct, convert_psy, reduce_sidechannel;
/*
mt 5/99.  These global flags denote 4 possibilities:
                                                                mode    l3_xmin
1   MDCT input L/R, quantize L/R,   psy-model thresholds: L/R   -m s     either
2   MDCT input L/R, quantize M/S,   psy-model thresholds: L/R   -m j     orig
3   MDCT input M/S, quantize M/S,   psy-model thresholds: M/S   -m f     either
4   MDCT input L/R, quantize M/S,   psy-model thresholds: M/S   -m j -h  m/s

1:  convert_mdct = 0, convert_psy=0,  reduce_sidechannel=0          
2:  convert_mdct = 1, convert_psy=1,  reduce_sidechannel=1
3:  convert_mdct = 0, convert_psy=0,  reduce_sidechannel=1   (this mode no longer used)
4:  convert_mdct = 1, convert_psy=0,  reduce_sidechannel=1

if (convert_mdct), then iteration_loop will quantize M/S data from
the L/R input MDCT coefficients.

if (convert_psy), then calc_noise will compute the noise for the L/R
channels from M/S MDCT data and L/R psy-model threshold information.
Distortion in ether L or R channel will be marked as distortion in
both Mid and Side channels.  

if (reduce_sidechannel) then outer_loop will allocate less bits
to the side channel and more bits to the mid channel based on relative 
energies.
*/



/*
  The following table is used to implement the scalefactor
  partitioning for MPEG2 as described in section
  2.4.3.2 of the IS. The indexing corresponds to the
  way the tables are presented in the IS:

  [table_number][row_in_table][column of nr_of_sfb]
*/
unsigned nr_of_sfb_block[6][3][4] =
{
  {
    {6, 5, 5, 5},
    {9, 9, 9, 9},
    {6, 9, 9, 9}
  },
  {
    {6, 5, 7, 3},
    {9, 9, 12, 6},
    {6, 9, 12, 6}
  },
  {
    {11, 10, 0, 0},
    {18, 18, 0, 0},
    {15,18,0,0}
  },
  {
    {7, 7, 7, 0},
    {12, 12, 12, 0},
    {6, 15, 12, 0}
  },
  {
    {6, 6, 6, 3},
    {12, 9, 9, 6},
    {6, 12, 9, 6}
  },
  {
    {8, 8, 5, 0},
    {15,12,9,0},
    {6,18,9,0}
  }
};


/* Table B.6: layer3 preemphasis */
int  pretab[21] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 2, 2, 3, 3, 3, 2
};

/* This is the scfsi_band table from 2.4.2.7 of the IS */
/*int scfsi_band_long[5] = { 0, 6, 11, 16, 21 };*/

int *scalefac_band_long; 
int *scalefac_band_short;


FLOAT8 pow43[PRECALC_SIZE];
static FLOAT8 adj43[PRECALC_SIZE];
static FLOAT8 adj43asm[PRECALC_SIZE];
static FLOAT8 ATH_l[SBPSY_l];
static FLOAT8 ATH_s[SBPSY_l];



/************************************************************************/
/*  initialization for iteration_loop */
/************************************************************************/
void
iteration_init( FLOAT8 xr_org[2][2][576], 
		III_side_info_t *l3_side, int l3_enc[2][2][576],
		frame_params *fr_ps, III_psy_xmin *l3_xmin)
{
  gr_info *cod_info;
  layer *info  = fr_ps->header;
  int stereo = fr_ps->stereo;
  int ch, gr, i, mode_gr;

  l3_side->resvDrain = 0;
  mode_gr = (info->version == 1) ? 2 : 1;

  if ( frameNum==0 ) {
    scalefac_band_long  = &sfBandIndex[info->sampling_frequency + (info->version * 3)].l[0];
    scalefac_band_short = &sfBandIndex[info->sampling_frequency + (info->version * 3)].s[0];
    l3_side->main_data_begin = 0;
    memset((char *) &l3_xmin, 0, sizeof(l3_xmin));
    compute_ath(info,ATH_l,ATH_s);

    for(i=0;i<PRECALC_SIZE;i++)
        pow43[i] = pow((FLOAT8)i, 4.0/3.0);

    for (i = 0; i < PRECALC_SIZE; i++)
	adj43[i] = (i + 1) - pow(0.5 * (pow43[i] + pow43[i + 1]), 0.75);
    adj43[i] = 0.5;


    adj43asm[0] = 0.0;
    for (i = 1; i < PRECALC_SIZE; i++)
      adj43asm[i] = i - 0.5 - pow(0.5 * (pow43[i - 1] + pow43[i]),0.75);

  }

  

  convert_mdct=0;
  convert_psy=0;
  reduce_sidechannel=0;
  if (info->mode_ext==MPG_MD_MS_LR) {
    if (highq) {
      convert_mdct = 1;
      convert_psy = 0;
      reduce_sidechannel=1;
    }else{
      convert_mdct = 1;
      convert_psy = 1;
      reduce_sidechannel=1;
    }
  }
  if (convert_psy) memset(l3_enc,0,sizeof(int)*2*2*576);
  
  /* some intializations. */
  for ( gr = 0; gr < mode_gr; gr++ ){
    for ( ch = 0; ch < stereo; ch++ ){
      cod_info = (gr_info *) &(l3_side->gr[gr].ch[ch]);

      if ( cod_info->window_switching_flag != 0 && cod_info->block_type == SHORT_TYPE )
        {
	  cod_info->sfb_lmax = 0; /* No sb*/
	  cod_info->sfb_smax = 0;
        }
      else
	{
	  /* MPEG 1 doesnt use last scalefactor band */
	  cod_info->sfb_lmax = SBPSY_l;
	  cod_info->sfb_smax = SBPSY_s;    /* No sb */
	}

    }
  }


  /* dont bother with scfsi. */
  for ( ch = 0; ch < stereo; ch++ )
    for ( i = 0; i < 4; i++ )
      l3_side->scfsi[ch][i] = 0;

}





/* 
compute the ATH for each scalefactor band 
cd range:  0..96db

Input:  3.3kHz signal  32767 amplitude  (3.3kHz is where ATH is smallest = -5db)
longblocks:  sfb=12   en0/bw=-11db    max_en0 = 1.3db
shortblocks: sfb=5           -9db              0db

Input:  1 1 1 1 1 1 1 -1 -1 -1 -1 -1 -1 -1 (repeated)
longblocks:  amp=1      sfb=12   en0/bw=-103 db      max_en0 = -92db
            amp=32767   sfb=12           -12 db                 -1.4db 

Input:  1 1 1 1 1 1 1 -1 -1 -1 -1 -1 -1 -1 (repeated)
shortblocks: amp=1      sfb=5   en0/bw= -99                    -86 
            amp=32767   sfb=5           -9  db                  4db 


MAX energy of largest wave at 3.3kHz = 1db
AVE energy of largest wave at 3.3kHz = -11db
Let's take AVE:  -11db = maximum signal in sfb=12.  
Dynamic range of CD: 96db.  Therefor energy of smallest audible wave 
in sfb=12  = -11  - 96 = -107db = ATH at 3.3kHz.  

ATH formula for this wave: -5db.  To adjust to LAME scaling, we need
ATH = ATH_formula  - 103  (db)
ATH = ATH * 2.5e-10      (ener)

*/
FLOAT8 ATHformula(FLOAT8 f)
{
  FLOAT8 ath;
  f  = Max(0.02, f);
  /* from Painter & Spanias, 1997 */
  /* minimum: (i=77) 3.3kHz = -5db */
  ath=(3.640 * pow(f,-0.8)
       -  6.500 * exp(-0.6*pow(f-3.3,2.0))
       +  0.001 * pow(f,4.0));
  
  /* convert to energy */
  ath -= 114;    /* MDCT scaling.  From tests by macik and MUS420 code */
  //ath -= 109;    
  /*ath -= 200;*/ /* disables ATH */
  ath = pow( 10.0, ath/10.0 );
  return ath;
}
 

void compute_ath(layer *info,FLOAT8 ATH_l[SBPSY_l],FLOAT8 ATH_s[SBPSY_l])
{
  int sfb,i,start,end;
  FLOAT8 ATH_f;
  FLOAT8 samp_freq = s_freq[info->version][info->sampling_frequency];

  /* last sfb is not used */
  for ( sfb = 0; sfb < SBPSY_l; sfb++ ) {
    start = scalefac_band_long[ sfb ];
    end   = scalefac_band_long[ sfb+1 ];
    ATH_l[sfb]=1e99;
    for (i=start ; i < end; i++) {
      ATH_f = ATHformula(samp_freq*i/(2*576)); /* freq in kHz */
      ATH_l[sfb]=Min(ATH_l[sfb],ATH_f);
    }
    /*
    printf("sfb=%i %f  ATH=%f %f  %f   \n",sfb,samp_freq*start/(2*576),
10*log10(ATH_l[sfb]),
10*log10( ATHformula(samp_freq*start/(2*576)))  ,
10*log10(ATHformula(samp_freq*end/(2*576))));
    */
  }

  for ( sfb = 0; sfb < SBPSY_s; sfb++ ){
    start = scalefac_band_short[ sfb ];
    end   = scalefac_band_short[ sfb+1 ];
    ATH_s[sfb]=1e99;
    for (i=start ; i < end; i++) {
      ATH_f = ATHformula(samp_freq*i/(2*192));     /* freq in kHz */
      ATH_s[sfb]=Min(ATH_s[sfb],ATH_f);
    }
  }
}





/* convert from L/R <-> Mid/Side */
void ms_convert(FLOAT8 xr[2][576],FLOAT8 xr_org[2][576])
{
  int i;
  for ( i = 0; i < 576; i++ ) {
    xr[0][i] = (xr_org[0][i]+xr_org[1][i])/SQRT2;
    xr[1][i] = (xr_org[0][i]-xr_org[1][i])/SQRT2;
  }
}



/************************************************************************
 * allocate bits among 2 channels based on PE
 * mt 6/99
 ************************************************************************/
void on_pe(FLOAT8 pe[2][2],III_side_info_t *l3_side,
int targ_bits[2],int mean_bits,int stereo, int gr)
{
  gr_info *cod_info;
  int extra_bits,tbits,bits;
  int add_bits[2]; 
  int ch;

  /* allocate targ_bits for granule */
  ResvMaxBits2( mean_bits, &tbits, &extra_bits, gr);
    

  for (ch=0 ; ch < stereo ; ch ++) {
    /******************************************************************
     * allocate bits for each channel 
     ******************************************************************/
    cod_info = &l3_side->gr[gr].ch[ch].tt;
    
    targ_bits[ch]=tbits/stereo;
    
    /* allocate extra bits from reservoir based on PE */
    bits=0;
    
    /* extra bits based on PE > 700 */
    add_bits[ch]=(pe[gr][ch]-750)/1.55;  /* 1.4; */
    
    /* short blocks need extra, no matter what the pe */
    if (cod_info->block_type==SHORT_TYPE) 
      if (add_bits[ch]<500) add_bits[ch]=500;
    
    if (add_bits[ch] < 0) add_bits[ch]=0;
    bits += add_bits[ch];
    
    if (bits > extra_bits) add_bits[ch] = (extra_bits*add_bits[ch])/bits;
    if ((targ_bits[ch]+add_bits[ch]) > 4095) 
      add_bits[ch]=4095-targ_bits[ch];

    targ_bits[ch] = targ_bits[ch] + add_bits[ch];
    extra_bits -= add_bits[ch];
  }
}

void reduce_side(int targ_bits[2],FLOAT8 ms_ener_ratio,int mean_bits)
{
int ch;
int stereo=2;
    /*  ms_ener_ratio = 0:  allocate 66/33  mid/side  fac=.33  
     *  ms_ener_ratio =.5:  allocate 50/50 mid/side   fac= 0 */
    /* 75/25 split is fac=.5 */
    /* float fac = .50*(.5-ms_ener_ratio[gr])/.5;*/
    float fac = .33*(.5-ms_ener_ratio)/.5;
    if (fac<0) fac=0;
    
    if (targ_bits[1] >= 125) {
      /* dont reduce side channel below 125 bits */
      if (targ_bits[1]-targ_bits[1]*fac > 125) {
	targ_bits[0] += targ_bits[1]*fac;
	targ_bits[1] -= targ_bits[1]*fac;
      } else {
	targ_bits[0] += targ_bits[1] - 125;
	targ_bits[1] = 125;
      }
    }
    
    /* dont allow to many bits per channel */  
    for (ch=0; ch<stereo; ch++) {
      int max_bits = Min(4095,mean_bits/2 + 1200);
      if (targ_bits[ch] > max_bits) {
	targ_bits[ch] = max_bits;
      }
    }

}

/*************************************************************************** 
 *         inner_loop                                                      * 
 *************************************************************************** 
 * The code selects the best quantizerStepSize for a particular set
 * of scalefacs                                                            */
 
int
inner_loop( FLOAT8 xr[2][2][576], FLOAT8 xrpow[576],
	    int l3_enc[2][2][576], int max_bits,
	    gr_info *cod_info, int gr, int ch )
{
    int bits;
    assert( max_bits >= 0 );
    cod_info->quantizerStepSize -= 1.0;;
    do
    {
      cod_info->quantizerStepSize += 1.0;
      bits = count_bits(l3_enc[gr][ch], xrpow, cod_info);  
    }
    while ( bits > max_bits );
    return bits;
}



/*************************************************************************/
/*            scale_bitcount                                             */
/*************************************************************************/

/* Also calculates the number of bits necessary to code the scalefactors. */

int scale_bitcount( III_scalefac_t *scalefac, gr_info *cod_info,
		int gr, int ch )
{
    int i, k, sfb, max_slen1 = 0, max_slen2 = 0, /*a, b, */ ep = 2;

    static int slen1[16] = { 1, 1, 1, 1, 8, 2, 2, 2, 4, 4, 4, 8, 8, 8,16,16 };
    static int slen2[16] = { 1, 2, 4, 8, 1, 2, 4, 8, 2, 4, 8, 2, 4, 8, 4, 8 };

    static int slen1_tab[16] = {0,
	18, 36, 54, 54, 36, 54, 72, 54, 72, 90, 72, 90,108,108,126
    };
    static int slen2_tab[16] = {0,
	10, 20, 30, 33, 21, 31, 41, 32, 42, 52, 43, 53, 63, 64, 74
    };
    int *tab;


    if ( cod_info->block_type == SHORT_TYPE )
    {
            tab = slen1_tab;
            /* a = 18; b = 18;  */
            for ( i = 0; i < 3; i++ )
            {
                for ( sfb = 0; sfb < 6; sfb++ )
                    if ( scalefac->s[gr][ch][sfb][i] > max_slen1 )
                        max_slen1 = scalefac->s[gr][ch][sfb][i];
                for (sfb = 6; sfb < SBPSY_s; sfb++ )
                    if ( scalefac->s[gr][ch][sfb][i] > max_slen2 )
                        max_slen2 = scalefac->s[gr][ch][sfb][i];
            }
    }
    else
    { /* block_type == 1,2,or 3 */
        tab = slen2_tab;
        /* a = 11; b = 10;   */
        for ( sfb = 0; sfb < 11; sfb++ )
            if ( scalefac->l[gr][ch][sfb] > max_slen1 )
                max_slen1 = scalefac->l[gr][ch][sfb];

	if (!cod_info->preflag) {
	    for ( sfb = 11; sfb < SBPSY_l; sfb++ )
		if (scalefac->l[gr][ch][sfb] < pretab[sfb])
		    break;

	    if (sfb == SBPSY_l) {
		cod_info->preflag = 1;
		for ( sfb = 11; sfb < SBPSY_l; sfb++ )
		    scalefac->l[gr][ch][sfb] -= pretab[sfb];
	    }
	}

	/*
	 * Note that scfsi is not enabled for frames containing short blocks
	 */

        for ( sfb = 11; sfb < SBPSY_l; sfb++ )
            if ( scalefac->l[gr][ch][sfb] > max_slen2 )
                max_slen2 = scalefac->l[gr][ch][sfb];
    }



    /* from Takehiro TOMINAGA <tominaga@isoternet.org> 10/99
     * loop over *all* posible values of scalefac_compress to find the
     * one which uses the smallest number of bits.  ISO would stop
     * at first valid index */
    cod_info->part2_length=10000;
    for ( k = 0; k < 16; k++ )
    {
        if ( (max_slen1 < slen1[k]) && (max_slen2 < slen2[k]) &&
             (cod_info->part2_length > tab[k])) {
	  cod_info->part2_length=tab[k];
	  cod_info->scalefac_compress=k;
	  ep=0;  /* we found a suitable scalefac_compress */
	}
    }
    return ep;
}



/*
  table of largest scalefactors for MPEG2
*/
static unsigned max_sfac_tab[6][4] =
{
    {4, 4, 3, 3},
    {4, 4, 3, 0},
    {3, 2, 0, 0},
    {4, 5, 5, 0},
    {3, 3, 3, 0},
    {2, 2, 0, 0}
};





/*************************************************************************/
/*            scale_bitcount_lsf                                         */
/*************************************************************************/

/* Also counts the number of bits to encode the scalefacs but for MPEG 2 */ 
/* Lower sampling frequencies  (24, 22.05 and 16 kHz.)                   */
 
/*  This is reverse-engineered from section 2.4.3.2 of the MPEG2 IS,     */
/* "Audio Decoding Layer III"                                            */

int scale_bitcount_lsf( III_scalefac_t *scalefac, gr_info *cod_info,
		    int gr, int ch )
{
    int table_number, row_in_table, partition, nr_sfb, window, over;
    int i, sfb, max_sfac[ 4 ];
    unsigned *partition_table;

    /*
      Set partition table. Note that should try to use table one,
      but do not yet...
    */
    if ( cod_info->preflag )
	table_number = 2;
    else
	table_number = 0;

    for ( i = 0; i < 4; i++ )
	max_sfac[i] = 0;

    if ( cod_info->window_switching_flag != 0 && cod_info->block_type == SHORT_TYPE )
    {
	    row_in_table = 1;
	    partition_table = &nr_of_sfb_block[table_number][row_in_table][0];
	    for ( sfb = 0, partition = 0; partition < 4; partition++ )
	    {
		nr_sfb = partition_table[ partition ] / 3;
		for ( i = 0; i < nr_sfb; i++, sfb++ )
		    for ( window = 0; window < 3; window++ )
			if ( scalefac->s[gr][ch][sfb][window] > max_sfac[partition] )
			    max_sfac[partition] = scalefac->s[gr][ch][sfb][window];
	    }
    }
    else
    {
	row_in_table = 0;
	partition_table = &nr_of_sfb_block[table_number][row_in_table][0];
	for ( sfb = 0, partition = 0; partition < 4; partition++ )
	{
	    nr_sfb = partition_table[ partition ];
	    for ( i = 0; i < nr_sfb; i++, sfb++ )
		if ( scalefac->l[gr][ch][sfb] > max_sfac[partition] )
		    max_sfac[partition] = scalefac->l[gr][ch][sfb];
	}
    }

    for ( over = 0, partition = 0; partition < 4; partition++ )
    {
	if ( max_sfac[partition] > max_sfac_tab[table_number][partition] )
	    over++;
    }
    if ( !over )
    {
	/*
	  Since no bands have been over-amplified, we can set scalefac_compress
	  and slen[] for the formatter
	*/
	static int log2tab[] = { 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4 };

	unsigned slen1, slen2, slen3, slen4;

        cod_info->sfb_partition_table = &nr_of_sfb_block[table_number][row_in_table][0];
	for ( partition = 0; partition < 4; partition++ )
	    cod_info->slen[partition] = log2tab[max_sfac[partition]];

	/* set scalefac_compress */
	slen1 = cod_info->slen[ 0 ];
	slen2 = cod_info->slen[ 1 ];
	slen3 = cod_info->slen[ 2 ];
	slen4 = cod_info->slen[ 3 ];

	switch ( table_number )
	{
	  case 0:
	    cod_info->scalefac_compress = (((slen1 * 5) + slen2) << 4)
		+ (slen3 << 2)
		+ slen4;
	    break;

	  case 1:
	    cod_info->scalefac_compress = 400
		+ (((slen1 * 5) + slen2) << 2)
		+ slen3;
	    break;

	  case 2:
	    cod_info->scalefac_compress = 500 + (slen1 * 3) + slen2;
	    break;

	  default:
	    fprintf( stderr, "intensity stereo not implemented yet\n" );
	    exit( EXIT_FAILURE );
	    break;
	}
    }
#ifdef DEBUG
    if ( over ) 
        printf( "---WARNING !! Amplification of some bands over limits\n" );
#endif
    if (!over) {
      assert( cod_info->sfb_partition_table );     
      cod_info->part2_length=0;
      for ( partition = 0; partition < 4; partition++ )
	cod_info->part2_length += cod_info->slen[partition] * cod_info->sfb_partition_table[partition];
    }
    return over;
}





/*************************************************************************/
/*            calc_xmin                                                  */
/*************************************************************************/

/*
  Calculate the allowed distortion for each scalefactor band,
  as determined by the psychoacoustic model.
  xmin(sb) = ratio(sb) * en(sb) / bw(sb)

  returns number of sfb's with energy > ATH
*/
int calc_xmin( FLOAT8 xr[2][2][576], III_psy_ratio *ratio,
	   gr_info *cod_info, III_psy_xmin *l3_xmin,
	   int gr, int ch )
{
    int start, end, sfb, l, b, over=0;
    FLOAT8 en0, bw, ener;

    D192_3 *xr_s;
    xr_s = (D192_3 *) xr[gr][ch] ;

    if (ATHonly) {    
      for ( sfb = cod_info->sfb_smax; sfb < SBPSY_s; sfb++ )
	  for ( b = 0; b < 3; b++ )
	      l3_xmin->s[gr][ch][sfb][b]=ATH_s[sfb];
      for ( sfb = 0; sfb < cod_info->sfb_lmax; sfb++ )
	  l3_xmin->l[gr][ch][sfb]=ATH_l[sfb];
      
    }else{

      for ( sfb = cod_info->sfb_smax; sfb < SBPSY_s; sfb++ ){
	start = scalefac_band_short[ sfb ];
        end   = scalefac_band_short[ sfb + 1 ];
	bw = end - start;
        for ( b = 0; b < 3; b++ ) {
	  for ( en0 = 0.0, l = start; l < end; l++ ) {
	    ener = (*xr_s)[l][b] * (*xr_s)[l][b];
	    en0 += ener;
	  }
	  en0 /= bw;
	  l3_xmin->s[gr][ch][sfb][b]=masking_lower*ratio->s[gr][ch][sfb][b]*en0;
	  l3_xmin->s[gr][ch][sfb][b]=Max(ATH_s[sfb],l3_xmin->s[gr][ch][sfb][b]);
	  if (en0 > ATH_s[sfb]) over++;
	}
      }
      
      
      for ( sfb = 0; sfb < cod_info->sfb_lmax; sfb++ ){
	start = scalefac_band_long[ sfb ];
	end   = scalefac_band_long[ sfb+1 ];
	bw = end - start;
	
        for ( en0 = 0.0, l = start; l < end; l++ ) {
	  ener = xr[gr][ch][l] * xr[gr][ch][l];
	  en0 += ener;
	}
	en0 /= bw;
        l3_xmin->l[gr][ch][sfb] =masking_lower*ratio->l[gr][ch][sfb] * en0;
	l3_xmin->l[gr][ch][sfb]=Max(ATH_l[sfb],l3_xmin->l[gr][ch][sfb]);
	if (en0 > ATH_l[sfb]) over++;
      }
    }
    return over;
}








/*************************************************************************/
/*            loop_break                                                 */
/*************************************************************************/

/*  Function: Returns zero if there is a scalefac which has not been
    amplified. Otherwise it returns one. 
*/

int loop_break( III_scalefac_t *scalefac, gr_info *cod_info,
	    int gr, int ch )
{
    int i, sfb;

    for ( sfb = 0; sfb < cod_info->sfb_lmax; sfb++ )
        if ( scalefac->l[gr][ch][sfb] == 0 )
	    return 0;

    for ( sfb = cod_info->sfb_smax; sfb < SBPSY_s; sfb++ )
        for ( i = 0; i < 3; i++ )
            if ( scalefac->s[gr][ch][sfb][i] == 0 )
		return 0;

    return 1;
}






#if defined(__GNUC__) && defined(__i386__)
#  define QUANTFAC(rx)  adj43asm[rx]
#  define XRPOW_FTOI(src, dest) \
     asm ("fistpl %0 " : "=m"(dest) : "t"(src) : "st")
#elif defined (_MSC_VER)
#  define QUANTFAC(rx)  adj43asm[rx]
#  define XRPOW_FTOI(src, dest) do { \
     FLOAT8 src_ = (src); \
     int dest_; \
     { \
       __asm fld src_ \
       __asm fistp dest_ \
     } \
     (dest) = dest_; \
   } while (0)
#else
#  define QUANTFAC(rx)  adj43[rx]
#  define XRPOW_FTOI(src,dest) ((dest) = (int)(src))
#endif

/*********************************************************************
 * nonlinear quantization of xr 
 * More accurate formula than the ISO formula.  Takes into account
 * the fact that we are quantizing xr -> ix, but we want ix^4/3 to be 
 * as close as possible to x^4/3.  (taking the nearest int would mean
 * ix is as close as possible to xr, which is different.)
 * From Segher Boessenkool <segher@eastsite.nl>  11/1999
 * ASM optimization from 
 *    Mathew Hendry <scampi@dial.pipex.com> 11/1999
 *    Acy Stapp <AStapp@austin.rr.com> 11/1999
 *    Takehiro Tominaga <tominaga@isoternet.org> 11/1999
 *********************************************************************/
void quantize_xrpow(FLOAT8 xr[576], int ix[576], gr_info *cod_info) {
  /* quantize on xr^(3/4) instead of xr */
  const FLOAT8 quantizerStepSize = cod_info->quantizerStepSize;
  const FLOAT8 istep = pow(2.0, quantizerStepSize * -0.1875);
  
#ifndef _MSC_VER
  {
      FLOAT8 x;
      int j, rx;
      for (j = 576 / 4; j > 0; --j) {
          x = *xr++ * istep;
          XRPOW_FTOI(x, rx);
          XRPOW_FTOI(x + QUANTFAC(rx), *ix++);
          x = *xr++ * istep;
          XRPOW_FTOI(x, rx);
          XRPOW_FTOI(x + QUANTFAC(rx), *ix++);
          x = *xr++ * istep;
          XRPOW_FTOI(x, rx);
          XRPOW_FTOI(x + QUANTFAC(rx), *ix++);
          x = *xr++ * istep;
          XRPOW_FTOI(x, rx);
          XRPOW_FTOI(x + QUANTFAC(rx), *ix++);
      }
  }
#else
/* def _MSC_VER */
  {
      /* asm from Acy Stapp <AStapp@austin.rr.com> */
      int rx[4];
      _asm {
          fld qword ptr [istep]
          mov esi, dword ptr [xr]
          lea edi, dword ptr [adj43asm]
          mov edx, dword ptr [ix]
          mov ecx, 576/4
      } loop1: _asm {
          fld qword ptr [esi]         // 0
          fld qword ptr [esi+8]       // 1 0
          fld qword ptr [esi+16]      // 2 1 0
          fld qword ptr [esi+24]      // 3 2 1 0

          fxch st(3)                  // 0 2 1 3
          fmul st(0), st(4)
          fxch st(2)                  // 1 2 0 3
          fmul st(0), st(4)
          fxch st(1)                  // 2 1 0 3
          fmul st(0), st(4)
          fxch st(3)                  // 3 1 0 2
          fmul st(0), st(4)

          add esi, 32
          add edx, 16

          fxch st(2)                  // 0 1 3 2
          fist dword ptr [rx]
          fxch st(1)                  // 1 0 3 2
          fist dword ptr [rx+4]
          fxch st(3)                  // 2 0 3 1
          fist dword ptr [rx+8]
          fxch st(2)                  // 3 0 2 1
          fist dword ptr [rx+12]

          dec ecx

          mov eax, dword ptr [rx]
          mov ebx, dword ptr [rx+4]
          fxch st(1)                  // 0 3 2 1
          fadd qword ptr [edi+eax*8]
          fxch st(3)                  // 1 3 2 0
          fadd qword ptr [edi+ebx*8]

          mov eax, dword ptr [rx+8]
          mov ebx, dword ptr [rx+12]
          fxch st(2)                  // 2 3 1 0
          fadd qword ptr [edi+eax*8]
          fxch st(1)                  // 3 2 1 0
          fadd qword ptr [edi+ebx*8]

          fxch st(3)                  // 0 2 1 3
          fistp dword ptr [edx-16]    // 2 1 3
          fxch st(1)                  // 1 2 3
          fistp dword ptr [edx-12]    // 2 3
          fistp dword ptr [edx-8]     // 3
          fistp dword ptr [edx-4]

          jnz loop1

          mov dword ptr [xr], esi
          mov dword ptr [ix], edx
          fstp st(0)
      }
  }
#endif
}



void quantize_xrpow_ISO( FLOAT8 xr[576], int ix[576], gr_info *cod_info )
{
  /* quantize on xr^(3/4) instead of xr */
  register int j;
  FLOAT8 quantizerStepSize;
  FLOAT8 istep;
  FLOAT8 rx;
#if defined(__GNUC__) && defined(__i386__) 
#elif defined (_MSC_VER)
  FLOAT8 temp0;
#else
  FLOAT8 compareval0;
#endif

  quantizerStepSize = cod_info->quantizerStepSize;
  
  istep = pow ( 2.0, quantizerStepSize * -0.1875 );
  
#if defined(_MSC_VER)
      /* asm from Acy Stapp <AStapp@austin.rr.com> */
      temp0 = 0.4054 - 0.5;
      _asm {
          mov ecx, 576/4;
          fld qword ptr [temp0];
          fld qword ptr [istep];
          mov eax, dword ptr [xr];
          mov edx, dword ptr [ix];
      } loop0: _asm {
          fld qword ptr [eax]; // 0
          fld qword ptr [eax+8]; // 1 0
          fld qword ptr [eax+16]; // 2 1 0
          fld qword ptr [eax+24]; // 3 2 1 0

          add eax, 32;
          add edx, 16;

          fxch st(3); // 0 2 1 3
          fmul st(0), st(4);
          fxch st(2); // 1 2 0 3
          fmul st(0), st(4);
          fxch st(1); // 2 1 0 3
          fmul st(0), st(4);
          fxch st(3); // 3 1 0 2
          fmul st(0), st(4);

          fxch st(2); // 0 1 3 2
          fadd st(0), st(5);
          fxch st(1); // 1 0 3 2
          fadd st(0), st(5);
          fxch st(3); // 2 0 3 1
          fadd st(0), st(5);
          fxch st(2); // 3 0 2 1
          fadd st(0), st(5);

          fxch st(1); // 0 3 2 1 
          fistp dword ptr [edx-16]; // 3 2 1
          fxch st(2); // 1 2 3
          fistp dword ptr [edx-12];
          fistp dword ptr [edx-8];
          fistp dword ptr [edx-4];

          loop loop0;

          mov dword ptr [xr], eax;
          mov dword ptr [ix], edx;
          fstp st(0);
          fstp st(0);
      }
#elif defined(__GNUC__) && defined(__i386__) 
      for (j=576/4;j>0;j--) {
         XRPOW_FTOI(istep * (*xr++) - 0.0946, *ix++);
         XRPOW_FTOI(istep * (*xr++) - 0.0946, *ix++);
         XRPOW_FTOI(istep * (*xr++) - 0.0946, *ix++);
         XRPOW_FTOI(istep * (*xr++) - 0.0946, *ix++);
      }
#else
      compareval0 = (1.0 - 0.4054)/istep;
      /* depending on architecture, it may be worth calculating a few more compareval's.
         eg.  compareval1 = (2.0 - 0.4054/istep); 
              .. and then after the first compare do this ...
              if compareval1>*xr then ix = 1;
         On a pentium166, it's only worth doing the one compare (as done here), as the second
         compare becomes more expensive than just calculating the value. Architectures with 
         slow FP operations may want to add some more comparevals. try it and send your diffs 
         statistically speaking
         73% of all xr*istep values give ix=0
         16% will give 1
         4%  will give 2
      */
      for (j=576;j>0;j--) 
        {
          if (compareval0 > *xr) {
            *(ix++) = 0;
            xr++;
          } else
            *(ix++) = (int)( istep*(*(xr++))  + 0.4054);
        }
#endif
}













typedef enum {
    BINSEARCH_NONE,
    BINSEARCH_UP, 
    BINSEARCH_DOWN
} binsearchDirection_t;

/*-------------------------------------------------------------------------*/
int 
bin_search_StepSize2(int      desired_rate, 
		    FLOAT8   start, 
		    int     *ix,
		    FLOAT8   xrs[576], 
		    FLOAT8   xrspow[576], 
		    gr_info *cod_info)
/*-------------------------------------------------------------------------*/
{
    int flag_GoneOver = 0;
    int CurrentStep = 4;
    int nBits;
    int StepSize = start;
    binsearchDirection_t Direction = BINSEARCH_NONE;
    do
    {
	cod_info->quantizerStepSize = StepSize;
	nBits = count_bits(ix, xrspow, cod_info);  

	if (CurrentStep == 1 )
        {
	    break; /* nothing to adjust anymore */
	}
	if (flag_GoneOver)
	{
	    CurrentStep /= 2;
	}
	if (nBits > desired_rate)  /* increase Quantize_StepSize */
	{
	    if (Direction == BINSEARCH_DOWN && !flag_GoneOver)
	    {
		flag_GoneOver = 1;
		CurrentStep /= 2; /* late adjust */
	    }
	    Direction = BINSEARCH_UP;
	    StepSize += CurrentStep;
	}
	else if (nBits < desired_rate)
	{
	    if (Direction == BINSEARCH_UP && !flag_GoneOver)
	    {
		flag_GoneOver = 1;
		CurrentStep /= 2; /* late adjust */
	    }
	    Direction = BINSEARCH_DOWN;
	    StepSize -= CurrentStep;
	}
	else break; /* nBits == desired_rate;; most unlikely to happen.
*/
    } while (1); /* For-ever, break is adjusted. */
    return nBits;
}






