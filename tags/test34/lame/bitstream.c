/*
 *	MP3 bitstream Output interface for LAME
 *
 *	Copyright (c) 1999 Takehiro TOMINAGA
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "tables.h"
#include "bitstream.h"
#include "quantize.h"
#include "quantize-pvt.h"

/* This is the scfsi_band table from 2.4.2.7 of the IS */
const int scfsi_band[5] = { 0, 6, 11, 16, 21 };



#define MAX_LENGTH      32   /* Maximum length of word written or
                                        read from bit stream */


#ifdef DEBUG
static int hoge, hogege;
#endif





const int pmask[8]={0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff};



void putheader_bits(lame_internal_flags *gfc,int w_ptr)
{
    Bit_stream_struc *bs;
    bs = &gfc->bs;
#ifdef DEBUG
    hoge += sideinfo_len * 8;
    hogege += sideinfo_len * 8;
#endif
    memcpy(&bs->buf[bs->buf_byte_idx], gfc->header[gfc->w_ptr].buf,
	   gfc->sideinfo_len);
    bs->buf_byte_idx += gfc->sideinfo_len;
    bs->totbit += gfc->sideinfo_len * 8;
    gfc->w_ptr = (gfc->w_ptr + 1) & (MAX_HEADER_BUF - 1);
}




/*write N bits into the bit stream */
INLINE static void
putbits2(lame_global_flags *gfp, unsigned int val, int j)
{
    lame_internal_flags *gfc=gfp->internal_flags;
    Bit_stream_struc *bs;
    bs = &gfc->bs;

    assert(j <= MAX_LENGTH);
    assert(val < (1 << j));
    while (j > 0) {
	int k;
	if (bs->buf_bit_idx == 0) {
	    bs->buf_bit_idx = 8;
	    bs->buf_byte_idx++;
	    assert(bs->buf_byte_idx < BUFFER_SIZE);
	    assert(gfc->header[gfc->w_ptr].write_timing >= bs->totbit);
	    if (gfc->header[gfc->w_ptr].write_timing == bs->totbit) {
	      putheader_bits(gfc,gfc->w_ptr);
	    }
	    bs->buf[bs->buf_byte_idx] = 0;
	}

	k = Min(j, bs->buf_bit_idx);
	bs->buf[bs->buf_byte_idx]
	    |= ((val >> (j-k)) & pmask[k - 1]) << (bs->buf_bit_idx - k);
	bs->buf_bit_idx -= k;
	j -= k;
	bs->totbit += k;
    }
}


/*write N bits into the header */
inline static void
writeheader(lame_internal_flags *gfc,unsigned int val, int j)
{
    int ptr = gfc->header[gfc->h_ptr].ptr;
    assert(j <= MAX_LENGTH);

    while (j > 0) {
	int k = Min(j, 8 - (ptr & 7));
	gfc->header[gfc->h_ptr].buf[ptr >> 3]
	    |= ((val >> (j-k)) & pmask[k-1]) << (8 - (ptr & 7) - k);
	ptr += k;
	j -= k;
    }
    gfc->header[gfc->h_ptr].ptr = ptr;
}


/* (jo) this wrapper function for BF_addEntry() updates also the crc */
static void
CRC_writeheader(lame_internal_flags *gfc,unsigned int value, int length,unsigned int *crc)
{
   unsigned int bit = 1 << length;

   while((bit >>= 1)){
      *crc <<= 1;
      if (!(*crc & 0x10000) ^ !(value & bit))
	*crc ^= CRC16_POLYNOMIAL;
   }
   *crc &= 0xffff;
   writeheader(gfc,value, length);
}

inline static void
encodeSideInfo2(lame_global_flags *gfp,int bitsPerFrame)
{
    lame_internal_flags *gfc=gfp->internal_flags;
    III_side_info_t *l3_side;
    int gr, ch;
    unsigned int crc;
    
    l3_side = &gfc->l3_side;
    gfc->header[gfc->h_ptr].ptr = 0;
    memset(gfc->header[gfc->h_ptr].buf, 0, gfc->sideinfo_len);
    crc = 0xffff; /* (jo) init crc16 for error_protection */
    if (gfp->out_samplerate < 16000) 
      writeheader(gfc,0xffe,                12);
    else
      writeheader(gfc,0xfff,                12);
    writeheader(gfc,gfc->version,            1);
    writeheader(gfc,4 - 3,                 2);
    writeheader(gfc,!gfp->error_protection,  1);
    /* (jo) from now on call the CRC_writeheader() wrapper to update crc */
    CRC_writeheader(gfc,gfc->bitrate_index,      4,&crc);
    CRC_writeheader(gfc,gfc->samplerate_index,   2,&crc);
    CRC_writeheader(gfc,gfc->padding,            1,&crc);
    CRC_writeheader(gfc,gfp->extension,          1,&crc);
    CRC_writeheader(gfc,gfp->mode,               2,&crc);
    CRC_writeheader(gfc,gfc->mode_ext,           2,&crc);
    CRC_writeheader(gfc,gfp->copyright,          1,&crc);
    CRC_writeheader(gfc,gfp->original,           1,&crc);
    CRC_writeheader(gfc,gfp->emphasis,           2,&crc);
    if (gfp->error_protection) {
	writeheader(gfc,0, 16); /* dummy */
    }

    if (gfc->version == 1) {
	/* MPEG1 */
	CRC_writeheader(gfc,l3_side->main_data_begin, 9,&crc);

	if (gfc->stereo == 2)
	    CRC_writeheader(gfc,l3_side->private_bits, 3,&crc);
	else
	    CRC_writeheader(gfc,l3_side->private_bits, 5,&crc);

	for (ch = 0; ch < gfc->stereo; ch++) {
	    int scfsi_band;
	    for (scfsi_band = 0; scfsi_band < 4; scfsi_band++) {
		CRC_writeheader(gfc,l3_side->scfsi[ch][scfsi_band], 1,&crc);
	    }
	}

	for (gr = 0; gr < 2; gr++) {
	    for (ch = 0; ch < gfc->stereo; ch++) {
		gr_info *gi = &l3_side->gr[gr].ch[ch].tt;
		CRC_writeheader(gfc,gi->part2_3_length,       12,&crc);
		CRC_writeheader(gfc,gi->big_values / 2,        9,&crc);
		CRC_writeheader(gfc,gi->global_gain,           8,&crc);
		CRC_writeheader(gfc,gi->scalefac_compress,     4,&crc);
		CRC_writeheader(gfc,gi->window_switching_flag, 1,&crc);

		if (gi->window_switching_flag) {
		    CRC_writeheader(gfc,gi->block_type,       2,&crc);
		    CRC_writeheader(gfc,gi->mixed_block_flag, 1,&crc);

		    CRC_writeheader(gfc,gi->table_select[0],  5,&crc);
		    CRC_writeheader(gfc,gi->table_select[1],  5,&crc);

		    CRC_writeheader(gfc,gi->subblock_gain[0], 3,&crc);
		    CRC_writeheader(gfc,gi->subblock_gain[1], 3,&crc);
		    CRC_writeheader(gfc,gi->subblock_gain[2], 3,&crc);
		} else {
		    assert(gi->block_type == NORM_TYPE);
		    CRC_writeheader(gfc,gi->table_select[0], 5,&crc);
		    CRC_writeheader(gfc,gi->table_select[1], 5,&crc);
		    CRC_writeheader(gfc,gi->table_select[2], 5,&crc);

		    CRC_writeheader(gfc,gi->region0_count, 4,&crc);
		    CRC_writeheader(gfc,gi->region1_count, 3,&crc);
		}
		CRC_writeheader(gfc,gi->preflag,            1,&crc);
		CRC_writeheader(gfc,gi->scalefac_scale,     1,&crc);
		CRC_writeheader(gfc,gi->count1table_select, 1,&crc);
	    }
	}
    } else {
	/* MPEG2 */
	CRC_writeheader(gfc,l3_side->main_data_begin, 8,&crc);
	CRC_writeheader(gfc,l3_side->private_bits, gfc->stereo,&crc);

	gr = 0;
	for (ch = 0; ch < gfc->stereo; ch++) {
	    gr_info *gi = &l3_side->gr[gr].ch[ch].tt;

	    CRC_writeheader(gfc,gi->part2_3_length,       12,&crc);
	    CRC_writeheader(gfc,gi->big_values / 2,        9,&crc);
	    CRC_writeheader(gfc,gi->global_gain,           8,&crc);
	    CRC_writeheader(gfc,gi->scalefac_compress,     9,&crc);
	    CRC_writeheader(gfc,gi->window_switching_flag, 1,&crc);

	    if (gi->window_switching_flag) {
		CRC_writeheader(gfc,gi->block_type,       2,&crc);
		CRC_writeheader(gfc,gi->mixed_block_flag, 1,&crc);

		CRC_writeheader(gfc,gi->table_select[0],  5,&crc);
		CRC_writeheader(gfc,gi->table_select[1],  5,&crc);

		CRC_writeheader(gfc,gi->subblock_gain[0], 3,&crc);
		CRC_writeheader(gfc,gi->subblock_gain[1], 3,&crc);
		CRC_writeheader(gfc,gi->subblock_gain[2], 3,&crc);
	    } else {
		CRC_writeheader(gfc,gi->table_select[0], 5,&crc);
		CRC_writeheader(gfc,gi->table_select[1], 5,&crc);
		CRC_writeheader(gfc,gi->table_select[2], 5,&crc);

		CRC_writeheader(gfc,gi->region0_count, 4,&crc);
		CRC_writeheader(gfc,gi->region1_count, 3,&crc);
	    }

	    CRC_writeheader(gfc,gi->scalefac_scale,     1,&crc);
	    CRC_writeheader(gfc,gi->count1table_select, 1,&crc);
	}
    }

    if (gfp->error_protection) {
	/* (jo) error_protection: add crc16 information to header */
	gfc->header[gfc->h_ptr].buf[4] = crc >> 8;
	gfc->header[gfc->h_ptr].buf[5] = crc & 255;
    }

    {
	int old = gfc->h_ptr;
	assert(gfc->header[old].ptr == gfc->sideinfo_len * 8);

	gfc->h_ptr = (old + 1) & (MAX_HEADER_BUF - 1);
	gfc->header[gfc->h_ptr].write_timing =
	    gfc->header[old].write_timing + bitsPerFrame;
    }
}


/*
  Some combinations of bitrate, Fs, and stereo make it impossible to stuff
  out a frame using just main_data, due to the limited number of bits to
  indicate main_data_length. In these situations, we put stuffing bits into
  the ancillary data...
*/

INLINE static void
drain_into_ancillary(lame_global_flags *gfp,int remainingBits)
{
  /*    lame_internal_flags *gfc=gfp->internal_flags;*/
    assert(remainingBits >= 0);
#ifdef DEBUG
    printf("remain %d\n",remainingBits);
    hoge += remainingBits;
    hogege += remainingBits;
#endif

    putbits2(gfp,0, remainingBits & (MAX_LENGTH - 1));
    remainingBits /= MAX_LENGTH;
    for (; remainingBits > 0; remainingBits--) {
	putbits2(gfp,0, MAX_LENGTH);
    }
}

inline static int
huffman_coder_count1(lame_global_flags *gfp,int *ix, gr_info *gi)
{
  /*    lame_internal_flags *gfc=gfp->internal_flags;*/
    /* Write count1 area */
    const struct huffcodetab *h = &ht_takehiro[gi->count1table_select + 32];
    int i,bits=0;
#ifdef DEBUG
    int gegebo = gfc->bs.totbit;
#endif

    ix += gi->big_values;
    assert(gi->count1table_select < 2);


    for (i = (gi->count1 - gi->big_values) / 4; i > 0; --i) {
	HUFFBITS huffbits = 0;
	int p = 0, v;

	v = ix[0];
	if (v) {
	    p += 8;
	    if (v < 0)
		huffbits++;
	    assert(-1 <= v && v <= 1);
	}

	v = ix[1];
	if (v) {
	    p += 4;
	    huffbits *= 2;
	    if (v < 0)
		huffbits++;
	    assert(-1 <= v && v <= 1);
	}

	v = ix[2];
	if (v) {
	    p += 2;
	    huffbits *= 2;
	    if (v < 0)
		huffbits++;
	    assert(-1 <= v && v <= 1);
	}

	v = ix[3];
	if (v) {
	    p++;
	    huffbits *= 2;
	    if (v < 0)
		huffbits++;
	    assert(-1 <= v && v <= 1);
	}

	ix += 4;
	putbits2(gfp,huffbits + h->table[p], h->hlen[p]);
	bits += h->hlen[p];
    }
#ifdef DEBUG
    printf("%d %d %d %d\n",gfc->bs.totbit -gegebo, gi->count1_bits, gi->big_values, gi->count1);
#endif
    return bits;
}

/*
  Implements the pseudocode of page 98 of the IS
  */

inline static int
HuffmanCode(lame_global_flags *gfp,int table_select, int x, int y)
{
  /*    lame_internal_flags *gfc=gfp->internal_flags;*/
    unsigned int code, ext, xlen;
    int cbits, xbits;
    unsigned int signx, signy, linbits;
    const struct huffcodetab *h;

    cbits = 0;
    xbits = 0;
    code  = 0;
    signx = signy = 0;

    if (x < 0) {
	signx++;
	x = -x;
    }

    if (y < 0) {
	signy++;
	y = -y;
    }

    assert(table_select>0);
    h = &(ht_takehiro[table_select]);
    linbits = h->xlen;
    ext = signx;
    xlen = h->xlen;

    if (table_select > 15) {
	/* use ESC-words */
	if (x > 14) {
	    int linbitsx = x - 15;
	    assert(linbitsx <= h->linmax);
	    ext |= linbitsx << 1;
	    xbits = linbits;
	    x = 15;
	}

	if (y > 14) {
	    int linbitsy = y - 15;
	    assert(linbitsy <= h->linmax);
	    ext <<= linbits;
	    ext |= linbitsy;
	    xbits += linbits;
	    y = 15;
	}
	xlen = 16;
    }

    if (x != 0) {
	cbits--;
    }

    if (y != 0) {
	ext <<= 1;
	ext |= signy;
	cbits--;
    }

    xbits -= cbits;

    assert(x <= 15);
    assert(y <= 15);

    x = x * xlen + y;

    code = h->table[x];
    cbits += h->hlen[x];

    assert(cbits <= 32);
    assert(xbits <= 32);

    putbits2(gfp,code, cbits);
    putbits2(gfp, ext, xbits);
    return cbits+xbits;
}

static int
Huffmancodebits(lame_global_flags *gfp,int tableindex, int start, int end, int *ix)
{
    int i,bits;

    assert(tableindex < 32);
    if (!tableindex) return 0;

    bits=0;
    for (i = start; i < end; i += 2) {
      bits +=HuffmanCode(gfp,tableindex, ix[i], ix[i + 1]);
    }
    return bits;
}



/*
  Note the discussion of huffmancodebits() on pages 28
  and 29 of the IS, as well as the definitions of the side
  information on pages 26 and 27.
  */
static int
ShortHuffmancodebits(lame_global_flags *gfp,int *ix, gr_info *gi)
{
    lame_internal_flags *gfc=gfp->internal_flags;
    int sfb,i,block,start,end,bits=0;
    int region1Start, region2Start;

    region1Start=scalefac_band.s[3];
    if (region1Start > gi->big_values/3)
	region1Start = gi->big_values/3;

    /* short blocks do not have a region2 */
    region2Start=gi->big_values/3;
    

    for ( sfb = 0; sfb < 13; sfb++ )      {
      unsigned tableindex = 100;
      start = scalefac_band.s[ sfb ];
      end   = scalefac_band.s[ sfb+1 ];

      if (start>=region2Start) break;
      
      if ( start < region1Start )
	tableindex = gi->table_select[ 0 ];
      else
	tableindex = gi->table_select[ 1 ];
      assert( tableindex < 32 );
      if (tableindex>0) {
	for (block=0 ; block<3; ++block) {
	  for (i = start; i < end; i += 2) {
	    bits +=HuffmanCode(gfp,tableindex, ix[3*i+block], ix[3*(i+1)+block]);
	  }
	}
      }
    }
    return bits;
}

static int
LongHuffmancodebits(lame_global_flags *gfp,int *ix, gr_info *gi)
{
    lame_internal_flags *gfc=gfp->internal_flags;
    int i, bigvalues,bits=0;
    int region1Start, region2Start;

    bigvalues = gi->big_values;
    assert(0 <= bigvalues && bigvalues <= 576);

    i = gi->region0_count + 1;
    assert(i < 23);
    region1Start = scalefac_band.l[i];
    i += gi->region1_count + 1;
    assert(i < 23);
    region2Start = scalefac_band.l[i];

    if (region1Start > bigvalues)
	region1Start = bigvalues;

    if (region2Start > bigvalues)
	region2Start = bigvalues;

    bits +=Huffmancodebits(gfp,gi->table_select[0], 0, region1Start, ix);
    bits +=Huffmancodebits(gfp,gi->table_select[1], region1Start, region2Start, ix);
    bits +=Huffmancodebits(gfp,gi->table_select[2], region2Start, bigvalues, ix);
    return bits;
}

inline static int
writeMainData(lame_global_flags *gfp,
      int              l3_enc[2][2][576],
  	III_scalefac_t   scalefac[2][2] )
{
    int i, gr, ch, sfb,data_bits,tot_bits=0;
    lame_internal_flags *gfc=gfp->internal_flags;
    III_side_info_t *l3_side;
    l3_side = &gfc->l3_side;


    if (gfc->version == 1) {
	/* MPEG 1 */
	for (gr = 0; gr < 2; gr++) {
	    for (ch = 0; ch < gfc->stereo; ch++) {
		gr_info *gi = &l3_side->gr[gr].ch[ch].tt;
		int slen1 = slen1_tab[gi->scalefac_compress];
		int slen2 = slen2_tab[gi->scalefac_compress];
		data_bits=0;

#ifdef DEBUG
		hogege = gfc->bs.totbit;
#endif
		if (gi->block_type == SHORT_TYPE) {
		    for (sfb = 0; sfb < SBPSY_s; sfb++) {
			int slen = sfb < 6 ? slen1 : slen2;
			putbits2(gfp,Max(scalefac[gr][ch].s[sfb][0], 0), slen);
			putbits2(gfp,Max(scalefac[gr][ch].s[sfb][1], 0), slen);
			putbits2(gfp,Max(scalefac[gr][ch].s[sfb][2], 0), slen);
			tot_bits += 3*slen;
		    }
		    data_bits += ShortHuffmancodebits(gfp,l3_enc[gr][ch], gi);
		} else {
		    int i;
		    for (i = 0; i < sizeof(scfsi_band) / sizeof(int) - 1;
			 i++) {
			if (gr != 0 && l3_side->scfsi[ch][i])
			    continue;

			for (sfb = scfsi_band[i]; sfb < scfsi_band[i + 1];
			     sfb++) {
			    putbits2(gfp,Max(scalefac[gr][ch].l[sfb], 0),
				    sfb < 11 ? slen1 : slen2);
			    tot_bits += sfb < 11 ? slen1 : slen2;
			}
		    }
		    data_bits +=LongHuffmancodebits(gfp,l3_enc[gr][ch], gi);
		}
		data_bits +=huffman_coder_count1(gfp,l3_enc[gr][ch], gi);
#ifdef DEBUG
		printf("<%d> ", gfc->bs.totbit-hogege);
#endif
		/* does bitcount in quantize.c agree with actual bit count?*/
		assert(data_bits==gi->part2_3_length-gi->part2_length);
		tot_bits += data_bits;

	    } /* for ch */
	} /* for gr */
    } else {
	/* MPEG 2 */
	gr = 0;
	for (ch = 0; ch < gfc->stereo; ch++) {
	    gr_info *gi = &l3_side->gr[gr].ch[ch].tt;
	    int sfb_partition;
	    assert(gi->sfb_partition_table);
	    data_bits = 0;

	    sfb = 0;
	    sfb_partition = 0;
	    if (gi->block_type == SHORT_TYPE) {
		for (; sfb_partition < 4; sfb_partition++) {
		    int sfbs = gi->sfb_partition_table[sfb_partition] / 3;
		    int slen = gi->slen[sfb_partition];
		    for (i = 0; i < sfbs; i++, sfb++) {
			putbits2(gfp,Max(scalefac[gr][ch].s[sfb][0], 0), slen);
			putbits2(gfp,Max(scalefac[gr][ch].s[sfb][1], 0), slen);
			putbits2(gfp,Max(scalefac[gr][ch].s[sfb][2], 0), slen);
			tot_bits += 3*slen;
		    }
		}
		data_bits += ShortHuffmancodebits(gfp,l3_enc[gr][ch], gi);
	    } else {
		for (; sfb_partition < 4; sfb_partition++) {
		    int sfbs = gi->sfb_partition_table[sfb_partition];
		    int slen = gi->slen[sfb_partition];
		    for (i = 0; i < sfbs; i++, sfb++) {
			putbits2(gfp,Max(scalefac[gr][ch].l[sfb], 0), slen);
			tot_bits += slen;
		    }
		}
		data_bits +=LongHuffmancodebits(gfp,l3_enc[gr][ch], gi);
	    }
	    data_bits +=huffman_coder_count1(gfp,l3_enc[gr][ch], gi);

	    /* does bitcount in quantize.c agree with actual bit count?*/
	    assert(data_bits==gi->part2_3_length-gi->part2_length);
	    tot_bits += data_bits;
	} /* for ch */
    } /* for gf */
    return tot_bits;
} /* main_data */




void
flush_bitstream(lame_global_flags *gfp)
{
  lame_internal_flags *gfc=gfp->internal_flags;
  int flushbits,remaining_headers;
  int bitsPerFrame, mean_bits;
  int last_ptr,first_ptr;
  first_ptr=gfc->w_ptr;           /* first header to add to bitstream */
  last_ptr = gfc->h_ptr - 1;   /* last header to add to bitstream */
  if (last_ptr==-1) last_ptr=31;   

  /* add this many bits to bitstream so we can flush all headers */
  flushbits = gfc->header[last_ptr].write_timing - gfc->bs.totbit;

  if (flushbits >= 0) {
    /* if flushbits >= 0, some headers have not yet be written */
    /* reduce flushbits by the size of the headers */
    remaining_headers= 1+last_ptr - first_ptr;
    if (last_ptr < first_ptr) 
      remaining_headers= 1+last_ptr - first_ptr + MAX_HEADER_BUF;
    flushbits -= remaining_headers*8*gfc->sideinfo_len;
  }


  /* finally, add some bits so that the last frame is complete
   * these bits are not necessary to decode the last frame, but
   * some decoders will ignore last frame if these bits are missing 
   */
  getframebits(gfp,&bitsPerFrame,&mean_bits);
  flushbits += bitsPerFrame;
  if (flushbits<0) {
#if 0
    /* if flushbits < 0, this would mean that the buffer looks like:
     * (data...)  last_header  (data...)  (extra data that should not be here...)
     */
    printf("last header write_timing = %i \n",gfc->header[last_ptr].write_timing);
    printf("first header write_timing = %i \n",gfc->header[first_ptr].write_timing);
    printf("bs.totbit:                 %i \n",gfc->bs.totbit);
    printf("first_ptr, last_ptr        %i %i \n",first_ptr,last_ptr);
    printf("remaining_headers =        %i \n",remaining_headers);
    printf("bitsperframe:              %i \n",bitsPerFrame);
    printf("sidelen:                   %i \n",gfc->sideinfo_len);
#endif
    fprintf(stderr,"strange error flushing buffer ... \n");
  } else {
    drain_into_ancillary(gfp,flushbits);
  }

  assert (gfc->header[last_ptr].write_timing + bitsPerFrame  == gfc->bs.totbit);
}

/*
  format_bitstream()

  This is called after a frame of audio has been quantized and coded.
  It will write the encoded audio to the bitstream. Note that
  from a layer3 encoder's perspective the bit stream is primarily
  a series of main_data() blocks, with header and side information
  inserted at the proper locations to maintain framing. (See Figure A.7
  in the IS).
  */
int
format_bitstream(lame_global_flags *gfp, int bitsPerFrame,
      int              l3_enc[2][2][576],
  	III_scalefac_t   scalefac[2][2] )
{
    lame_internal_flags *gfc=gfp->internal_flags;
    int bits;
    III_side_info_t *l3_side;
    l3_side = &gfc->l3_side;


#ifdef DEBUG
    hoge = bs.totbit;
    hogege = bs.totbit;

    printf("%d %d %d %d ->%d //\n",
	   l3_side->tt[0][0].part2_3_length,
	   l3_side->tt[0][1].part2_3_length,
	   l3_side->tt[1][0].part2_3_length,
	   l3_side->tt[1][1].part2_3_length,

	   l3_side->tt[0][0].part2_3_length +
	   l3_side->tt[0][1].part2_3_length +
	   l3_side->tt[1][0].part2_3_length +
	   l3_side->tt[1][1].part2_3_length);
#endif
    encodeSideInfo2(gfp,bitsPerFrame);
    bits = 8*gfc->sideinfo_len;
    bits+=writeMainData(gfp,l3_enc,scalefac);
    drain_into_ancillary(gfp,l3_side->resvDrain);
    bits += l3_side->resvDrain;

    l3_side->main_data_begin += (bitsPerFrame-bits)/8;
#ifdef DEBUG
    printf("total bits used = %i max=%i mdb=%i \n",bits,bitsPerFrame,l3_side->main_data_begin*8);
    printf("%d -> %d\n",
	   gfc->bs.totbit - hoge,
	   l3_side->tt[0][0].part2_3_length +
	   l3_side->tt[0][1].part2_3_length +
	   l3_side->tt[1][0].part2_3_length +
	   l3_side->tt[1][1].part2_3_length -
	   (bs.totbit - hoge));
#endif
    assert(gfc->bs.totbit % 8 == 0);

    return gfc->bs.totbit;
}


void add_dummy_vbrframe(lame_global_flags *gfp,int bitsPerFrame)
{
  lame_internal_flags *gfc = gfp->internal_flags;
  int bits;

  gfc->header[gfc->h_ptr].ptr = 0;
  memset(gfc->header[gfc->h_ptr].buf, 0, gfc->sideinfo_len);
  /* vbr header frames never have CRC */
  bits = bitsPerFrame-8*gfc->sideinfo_len - 16*gfp->error_protection;

  /* add one byte, cause header to be written */
  putbits2(gfp,0,8);   
  /* setup for next header */
  gfc->h_ptr = (gfc->h_ptr + 1) & (MAX_HEADER_BUF - 1);
  gfc->header[gfc->h_ptr].write_timing = bitsPerFrame;

  drain_into_ancillary(gfp,bits-8);

}




