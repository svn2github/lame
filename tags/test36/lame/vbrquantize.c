/*
 *	MP3 quantization
 *
 *	Copyright (c) 1999 Mark Taylor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <assert.h>
#include "util.h"
#include "l3side.h"
#include "quantize.h"
#include "l3bitstream.h"
#include "reservoir.h"
#include "quantize-pvt.h"
#ifdef HAVEGTK
#include "gtkanal.h"
#endif


#if (defined(__GNUC__) && defined(__i386__))
#define USE_GNUC_ASM
#endif
#ifdef _MSC_VER
#define USE_MSC_ASM
#endif



/*********************************************************************
 * XRPOW_FTOI is a macro to convert floats to ints.  
 * if XRPOW_FTOI(x) = nearest_int(x), then QUANTFAC(x)=adj43asm[x]
 *                                         ROUNDFAC= -0.0946
 *
 * if XRPOW_FTOI(x) = floor(x), then QUANTFAC(x)=asj43[x]   
 *                                   ROUNDFAC=0.4054
 *********************************************************************/
#ifdef USE_GNUC_ASM
#  define QUANTFAC(rx)  adj43asm[rx]
#  define ROUNDFAC -0.0946
#  define XRPOW_FTOI(src, dest) \
     asm ("fistpl %0 " : "=m"(dest) : "t"(src) : "st")
#elif defined (USE_MSC_ASM)
#  define QUANTFAC(rx)  adj43asm[rx]
#  define ROUNDFAC -0.0946
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
#  define ROUNDFAC 0.4054
#  define XRPOW_FTOI(src,dest) ((dest) = (int)(src))
#endif









FLOAT8 calc_sfb_ave_noise(int average,FLOAT8 *xr, FLOAT8 *xr34, int stride, int bw, FLOAT8 sfpow)
{
  int j;
  FLOAT8 xfsf=0, xfsf_p1=0, xfsf_m1=0;
  FLOAT8 sfpow34,sfpow34_p1,sfpow34_m1;
  FLOAT8 sfpow_p1,sfpow_m1;


  sfpow34  = pow(sfpow,-3.0/4.0);
  sfpow_p1 = sfpow*1.189207115;  /* pow(2,(sf+1)/4.0) */
  sfpow_m1 = sfpow*.8408964153;  /* pow(2,(sf-1)/4.0) */
  sfpow34_p1 = sfpow34*0.878126080187;
  sfpow34_m1 = sfpow34*1.13878863476;       /* .84089 ^ -3/4 */


  for ( j=0; j < stride*bw ; j += stride) {
    int ix;
    FLOAT8 temp,temp_p1,temp_m1;

#if 0
    if (xr34[j]*sfpow34 > IXMAX_VAL) return -1;
    ix=floor( xr34[j]*sfpow34);
    temp = fabs(xr[j])- pow43[ix]*sfpow;
    temp *= temp;

    if (ix < IXMAX_VAL) {
      temp2 = fabs(xr[j])- pow43[ix+1]*sfpow;
      temp2 *=temp2;
      if (temp2<temp) {
	temp=temp2;
	++ix;
      }
    }
#else
    if (xr34[j]*sfpow34_m1 > IXMAX_VAL) return -1;

    temp = xr34[j]*sfpow34;
    XRPOW_FTOI(temp, ix);
    XRPOW_FTOI(temp + QUANTFAC(ix), ix);
    temp = fabs(xr[j])- pow43[ix]*sfpow;
    temp *= temp;
    
    if (average) {
      temp_p1 = xr34[j]*sfpow34_p1;
      XRPOW_FTOI(temp_p1, ix);
      XRPOW_FTOI(temp_p1 + QUANTFAC(ix), ix);
      temp_p1 = fabs(xr[j])- pow43[ix]*sfpow_p1;
      temp_p1 *= temp_p1;
      
      temp_m1 = xr34[j]*sfpow34_m1;
      XRPOW_FTOI(temp_m1, ix);
      XRPOW_FTOI(temp_m1 + QUANTFAC(ix), ix);
      temp_m1 = fabs(xr[j])- pow43[ix]*sfpow_m1;
      temp_m1 *= temp_m1;
    }
#endif
    
#ifdef MAXQUANTERROR
    xfsf = Max(xfsf,temp);
    if (average) {
      xfsf_p1 = Max(xfsf_p1,temp_p1);
      xfsf_m1 = Max(xfsf_m1,temp_m1);
    }
#else
    xfsf += temp;
    if (average) {
      xfsf_p1 += temp_p1;
      xfsf_m1 += temp_m1;
    }
#endif
  }
  if (average) {
    if (xfsf_p1>xfsf) xfsf = xfsf_p1;
    if (xfsf_m1>xfsf) xfsf = xfsf_m1;
  }
#ifdef MAXQUANTERROR
  return xfsf;
#else
  return xfsf/bw;
#endif
}



int find_scalefac(FLOAT8 *xr,FLOAT8 *xr34,int stride,int sfb,
		     FLOAT8 l3_xmin,int bw)
{
  FLOAT8 xfsf,sfpow;
  int i,sf,sf_ok,delsf;

  /* search will range from sf:  -209 -> 45  */
  sf = -82;
  delsf = 128;

  sf_ok=10000;
  for (i=0; i<7; i++) {
    delsf /= 2;
    sfpow = pow(2.0,sf/4.0); 
    if (delsf >= 16)
      xfsf = calc_sfb_ave_noise(0,xr,xr34,stride,bw,sfpow);
    else
      xfsf = calc_sfb_ave_noise(1,xr,xr34,stride,bw,sfpow);

    if (xfsf < 0) {
      /* scalefactors too small */
      sf += delsf;
    }else{
      if (sf_ok==10000) sf_ok=sf;  
      if (xfsf > l3_xmin)  {
	/* distortion.  try a smaller scalefactor */
	sf -= delsf;
      }else{
	sf_ok = sf;
	sf += delsf;
      }
    }
  } 
  assert(sf_ok!=10000);
  assert(delsf==1);

  return sf;
}



/*
    sfb=0..5  scalefac < 16 
    sfb>5     scalefac < 8

    ifqstep = ( cod_info->scalefac_scale == 0 ) ? 2 : 4;
    ol_sf =  (cod_info->global_gain-210.0);
    ol_sf -= 8*cod_info->subblock_gain[i];
    ol_sf -= ifqstep*scalefac[gr][ch].s[sfb][i];

*/
int compute_scalefacs_short(int sf[SBPSY_s][3],gr_info *cod_info,
int scalefac[SBPSY_s][3],int sbg[3])
{
  int maxrange,maxrange1,maxrange2,maxover;
  int sfb,i;
  int ifqstep = ( cod_info->scalefac_scale == 0 ) ? 2 : 4;

  maxover=0;
  maxrange1 = 15;
  maxrange2 = 7;


  for (i=0; i<3; ++i) {
    int maxsf1=0,maxsf2=0,minsf=1000;
    sbg[i]=0;
    /* see if we should use subblock gain */
    for ( sfb = 0; sfb < SBPSY_s; sfb++ ) {
      if (sfb < 6) {
	if (-sf[sfb][i]>maxsf1) maxsf1 = -sf[sfb][i];
      } else {
	if (-sf[sfb][i]>maxsf2) maxsf2 = -sf[sfb][i];
      }
      if (-sf[sfb][i]<minsf) minsf = -sf[sfb][i];
    }

    /* boost subblock gain as little as possible so we can
     * reach maxsf1 with scalefactors 
     * 8*sbg >= maxsf1   
     */
    maxsf1 = Max(maxsf1-maxrange1*ifqstep,maxsf2-maxrange2*ifqstep);

    if (minsf >0 ) sbg[i] = floor(.125*minsf + .001);
    if (maxsf1 > 0)  sbg[i]  = Max(sbg[i],maxsf1/8 + (maxsf1 % 8 != 0));
    if (sbg[i] > 7) sbg[i]=7;


    for ( sfb = 0; sfb < SBPSY_s; sfb++ ) {
      sf[sfb][i] += 8*sbg[i];

      if (sf[sfb][i] < 0) {
	maxrange = sfb < 6 ? maxrange1 : maxrange2;
	scalefac[sfb][i]=-sf[sfb][i]/ifqstep + (-sf[sfb][i]%ifqstep != 0);
	if (scalefac[sfb][i]>maxrange) scalefac[sfb][i]=maxrange;
	
	if (-(sf[sfb][i] + scalefac[sfb][i]*ifqstep) >maxover)  {
	  maxover=-(sf[sfb][i] + scalefac[sfb][i]*ifqstep);
	  printf("sfb=%i i=%i  sf=%i   scalefac=%i \n",sfb,i,sf[sfb][i],
		 scalefac[sfb][i]);
	}
      }
    }
  }

  return maxover;
}





int max_range_short[SBPSY_s]=
{15, 15, 15, 15, 15, 15 ,  7,    7,    7,    7,   7,     7 };
int max_range_long[SBPSY_l]=
{15,   15,  15,  15,  15,  15,  15,  15,  15,  15,  15,   7,   7,   7,   7,   7,   7,   7,    7,    7,    7};


/*
	  ifqstep = ( cod_info->scalefac_scale == 0 ) ? 2 : 4;
	  ol_sf =  (cod_info->global_gain-210.0);
	  ol_sf -= ifqstep*scalefac[gr][ch].l[sfb];
	  if (cod_info->preflag && sfb>=11) 
	  ol_sf -= ifqstep*pretab[sfb];
*/
int compute_scalefacs_long(int sf[SBPSY_l],gr_info *cod_info,int scalefac[SBPSY_l])
{
  int sfb;
  int maxover;
  int ifqstep = ( cod_info->scalefac_scale == 0 ) ? 2 : 4;
  

  if (cod_info->preflag)
    for ( sfb = 11; sfb < SBPSY_l; sfb++ ) 
      sf[sfb] += pretab[sfb]*ifqstep;


  maxover=0;
  for ( sfb = 0; sfb < SBPSY_l; sfb++ ) {

    if (sf[sfb]<0) {
      /* ifqstep*scalefac >= -sf[sfb], so round UP */
      scalefac[sfb]=-sf[sfb]/ifqstep  + (-sf[sfb] % ifqstep != 0);
      if (scalefac[sfb] > max_range_long[sfb]) scalefac[sfb]=max_range_long[sfb];
      
      /* sf[sfb] should now be positive: */
      if (  -(sf[sfb] + scalefac[sfb]*ifqstep)  > maxover) {
	maxover = -(sf[sfb] + scalefac[sfb]*ifqstep);
      }
    }
  }

  return maxover;
}
  
  



/************************************************************************
 *
 * VBR_noise_shapping()
 *
 * compute scalefactors, l3_enc, and return number of bits needed to encode
 *
 *
 ************************************************************************/
int
VBR_noise_shapping (lame_global_flags *gfp,
                FLOAT8 pe[2][2], FLOAT8 ms_ener_ratio[2],
                FLOAT8 xr[2][2][576], III_psy_ratio ratio[2][2],
                int l3_enc[2][2][576],
                III_scalefac_t scalefac[2][2])
{
  lame_internal_flags *gfc=gfp->internal_flags;
  FLOAT8    masking_lower_db;
  int       start,end,bw,sfb,l, i,ch, gr, bits, vbrmax;
  scalefac_struct2 vbrsf;
  int maxover0,maxover1,maxover0p,maxover1p,maxover;
  int ifqstep,analog_silence;
  III_side_info_t * l3_side;
  gr_info *cod_info;  

  l3_side = &gfc->l3_side;
  iteration_init(gfp,l3_side,l3_enc);


  bits = 0; 
  for (gr = 0; gr < gfc->mode_gr; gr++) {
    if (convert_mdct)
      ms_convert(xr[gr],xr[gr]);
    for (ch = 0; ch < gfc->stereo; ch++) { 
      FLOAT8 xr34[576];
      III_psy_xmin l3_xmin;

      int shortblock;
      cod_info = &l3_side->gr[gr].ch[ch].tt;
      shortblock = (cod_info->block_type == SHORT_TYPE);

      for(i=0;i<576;i++) {
	FLOAT8 temp=fabs(xr[gr][ch][i]);
	xr34[i]=sqrt(sqrt(temp)*temp);
      }

      analog_silence = 
	(0==calc_xmin( gfp,xr[gr][ch], &ratio[gr][ch], cod_info, &l3_xmin));

      vbrmax=-10000;
      if (shortblock) {
	for ( sfb = 0; sfb < SBPSY_s; sfb++ )  {
	  for ( i = 0; i < 3; i++ ) {
	    start = gfc->scalefac_band.s[ sfb ];
	    end   = gfc->scalefac_band.s[ sfb+1 ];
	    bw = end - start;
	    vbrsf.s[sfb][i] = find_scalefac(&xr[gr][ch][3*start+i],&xr34[3*start+i],3,sfb,
		   masking_lower*l3_xmin.s[sfb][i],bw);
	    if (vbrsf.s[sfb][i]>vbrmax) vbrmax=vbrsf.s[sfb][i];
	  }
	}
      }else{
	for ( sfb = 0; sfb < SBPSY_l; sfb++ )   {
	  start = gfc->scalefac_band.l[ sfb ];
	  end   = gfc->scalefac_band.l[ sfb+1 ];
	  bw = end - start;
	  vbrsf.l[sfb] = find_scalefac(&xr[gr][ch][start],&xr34[start],1,sfb,
	  		 masking_lower*l3_xmin.l[sfb],bw);
	  if (vbrsf.l[sfb]>vbrmax) vbrmax = vbrsf.l[sfb];
	}

      } /* compute needed scalefactors */




      if (shortblock) {
	/******************************************************************
	 *
	 *  short block scalefacs
	 *
	 ******************************************************************/
	maxover0=0;
	maxover1=0;
	for ( sfb = 0; sfb < SBPSY_s; sfb++ ) {
	  for ( i = 0; i < 3; i++ ) {
	    maxover0 = Max(maxover0,(vbrmax - vbrsf.s[sfb][i]) - (4*14 + 2*max_range_short[sfb]) );
	    maxover1 = Max(maxover1,(vbrmax - vbrsf.s[sfb][i]) - (4*14 + 4*max_range_short[sfb]) );
	  }
	}
	if (maxover0==0) 
	  cod_info->scalefac_scale = 0;
	else if (maxover1==0)
	  cod_info->scalefac_scale = 1;
	else {
	  cod_info->scalefac_scale = 1;
          vbrmax -= maxover1;
	}


	/* sf =  (cod_info->global_gain-210.0) */
	cod_info->global_gain = vbrmax +210;
	if (cod_info->global_gain>255) cod_info->global_gain=255;

	for ( sfb = 0; sfb < SBPSY_s; sfb++ ) {
	  for ( i = 0; i < 3; i++ ) {
	    vbrsf.s[sfb][i]-=vbrmax;
	  }
	}
	maxover=compute_scalefacs_short(vbrsf.s,cod_info,scalefac[gr][ch].s,cod_info->subblock_gain);
	assert(maxover <=0);
	{
	  int minsfb=999;
	  for (i=0; i<3; i++) minsfb = Min(minsfb,cod_info->subblock_gain[i]);
	  minsfb = Min(cod_info->global_gain/8,minsfb);
	  vbrmax -= 8*minsfb; 
	  cod_info->global_gain -= 8*minsfb;
	  for (i=0; i<3; i++) cod_info->subblock_gain[i] -= minsfb;
	}



	/* quantize xr34[] based on computed scalefactors */
	ifqstep = ( cod_info->scalefac_scale == 0 ) ? 2 : 4;
	for ( sfb = 0; sfb < SBPSY_s; sfb++ ) {
	  start = gfc->scalefac_band.s[ sfb ];
	  end   = gfc->scalefac_band.s[ sfb+1 ];
	  for (i=0; i<3; i++) {
	    FLOAT8 fac;
	    fac = (8*cod_info->subblock_gain[i]+ifqstep*scalefac[gr][ch].s[sfb][i]);
	    fac = pow(2.0,.75*fac/4.0);
	    for ( l = start; l < end; l++ ) 
	      xr34[3*l +i]*=fac;
	  }
	}



      }else{
	/******************************************************************
	 *
	 *  long block scalefacs
	 *
	 ******************************************************************/
	maxover0=0;
	maxover1=0;
	maxover0p=0;
	maxover1p=0;


	for ( sfb = 0; sfb < SBPSY_l; sfb++ ) {
	  maxover0 = Max(maxover0,(vbrmax - vbrsf.l[sfb]) - 2*max_range_long[sfb] );
	  maxover0p = Max(maxover0,(vbrmax - vbrsf.l[sfb]) - 2*(max_range_long[sfb]+pretab[sfb]) );
	  maxover1 = Max(maxover1,(vbrmax - vbrsf.l[sfb]) - 4*max_range_long[sfb] );
	  maxover1p = Max(maxover1,(vbrmax - vbrsf.l[sfb]) - 4*(max_range_long[sfb]+pretab[sfb]));
	}


        if (maxover0==0) {
	  cod_info->scalefac_scale = 0;
         } else if (maxover0p==0) {
            cod_info->scalefac_scale = 0;
            cod_info->preflag=1;
        } else if (maxover1==0) {
	  cod_info->scalefac_scale = 1;
        } else if (maxover1p==0) {
            cod_info->scalefac_scale = 1;
            cod_info->preflag=1;
	} else {
	  cod_info->scalefac_scale = 1;
          vbrmax -= maxover1;
	}


	/* sf =  (cod_info->global_gain-210.0) */
	cod_info->global_gain = vbrmax +210;
	if (cod_info->global_gain>255) cod_info->global_gain=255;

	for ( sfb = 0; sfb < SBPSY_l; sfb++ )   
	  vbrsf.l[sfb] -= vbrmax;


	maxover=compute_scalefacs_long(vbrsf.l,cod_info,scalefac[gr][ch].l);
	assert(maxover <=0);


	/* quantize xr34[] based on computed scalefactors */
	ifqstep = ( cod_info->scalefac_scale == 0 ) ? 2 : 4;
	for ( sfb = 0; sfb < SBPSY_l; sfb++ ) {
          FLOAT8 fac;
	  fac = ifqstep*scalefac[gr][ch].l[sfb];
	  if (cod_info->preflag)
	    fac += ifqstep*pretab[sfb];

	  assert(fac >= -vbrsf.l[sfb]);
	  fac=pow(2.0,.75*fac/4.0);
	  start = gfc->scalefac_band.l[ sfb ];
	  end   = gfc->scalefac_band.l[ sfb+1 ];
	  for ( l = start; l < end; l++ ) 
	    xr34[l]*=fac;
	}
      } 



      /* encode scalefacs */
      if ( gfc->version == 1 ) 
	scale_bitcount(&scalefac[gr][ch], cod_info);
      else
	scale_bitcount_lsf(&scalefac[gr][ch], cod_info);

      /* quantize xr34 */
      cod_info->part2_3_length = count_bits(gfp,l3_enc[gr][ch],xr34,cod_info);
      cod_info->part2_3_length += cod_info->part2_length;
      assert((int)count_bits != LARGE_BITS);

#ifdef HAVEGTK
      /* do this before calling best_scalefac_store! */
      if (gfp->gtkflag) {
	FLOAT8 noise[4];
	FLOAT8 xfsf[4][SBPSY_l];
	FLOAT8 distort[4][SBPSY_l];

	noise[0]=calc_noise1( gfp, xr[gr][ch], l3_enc[gr][ch], cod_info, 
		  xfsf,distort, &l3_xmin, &scalefac[gr][ch], 
                  &noise[2],&noise[3],&noise[1]);

	set_pinfo (gfp, cod_info, &ratio[gr][ch], &scalefac[gr][ch], xr[gr][ch], xfsf, noise, gr, ch);
      }
#endif

      cod_info = &l3_side->gr[gr].ch[ch].tt;
      best_scalefac_store(gfp,gr, ch, l3_enc, l3_side, scalefac);
      if (gfc->use_best_huffman==1 && cod_info->block_type != SHORT_TYPE) {
	best_huffman_divide(gfc, gr, ch, cod_info, l3_enc[gr][ch]);
      }

      bits+=cod_info->part2_3_length;
#ifdef HAVEGTK
      if (gfp->gtkflag)
	pinfo->LAMEmainbits[gr][ch]=cod_info->part2_3_length;
#endif

    } /* ch */
  } /* gr */
  return bits;
}








void
VBR_quantize(lame_global_flags *gfp,
                FLOAT8 pe[2][2], FLOAT8 ms_ener_ratio[2],
                FLOAT8 xr[2][2][576], III_psy_ratio ratio[2][2],
                int l3_enc[2][2][576],
                III_scalefac_t scalefac[2][2])
{
  lame_internal_flags *gfc=gfp->internal_flags;
  int bits,gr,ch,i;
  III_side_info_t * l3_side;
  gr_info *cod_info;  
  FLOAT8 masking_lower_db;
  static const FLOAT dbQ[10]={-6.0,-4.5,-3.0,-1.5,0,0.3,0.6,1.0,1.5,2.0};


  /* Adjust allowed masking based on quality setting */
  assert( gfp->VBR_q <= 9 );
  assert( gfp->VBR_q >= 0 );
  masking_lower_db = dbQ[gfp->VBR_q];	

  //  masking_lower_db = 0;
  masking_lower = pow(10.0,masking_lower_db/10);


  bits = VBR_noise_shapping (gfp,pe, ms_ener_ratio,
     xr, ratio,l3_enc,scalefac);


  /*******************************************************************
   * how many bits are available for each bitrate?
   *******************************************************************/
  { int bitsPerFrame,mean_bits;
  l3_side = &gfc->l3_side;
  for( gfc->bitrate_index = 1;
       gfc->bitrate_index <= gfc->VBR_max_bitrate;
       gfc->bitrate_index++    ) {
    getframebits (gfp,&bitsPerFrame, &mean_bits);
    if (ResvFrameBegin(gfp,l3_side, mean_bits, bitsPerFrame)>=bits) break;
  }
  assert(gfc->bitrate_index <= gfc->VBR_max_bitrate);

  for (gr = 0; gr < gfc->mode_gr; gr++)
    for (ch = 0; ch < gfc->stereo; ch++) {
      cod_info = &l3_side->gr[gr].ch[ch].tt;
      ResvAdjust (gfp,cod_info, l3_side, mean_bits);

      /*******************************************************************
       * set the sign of l3_enc from the sign of xr
       *******************************************************************/
      for ( i = 0; i < 576; i++) {
        if (xr[gr][ch][i] < 0) l3_enc[gr][ch][i] *= -1;
      }
    }
  ResvFrameEnd (gfp,l3_side, mean_bits);
  }
}



