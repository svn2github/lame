/*
 * Copyright (C) 2000 Albert L. Faber
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
#include "hip.h"
#include "interface.h"

#define HF_MP(hf)     ((PMPSTR)hf->mp)
#define MAX_U_32_NUM  0xFFFFFFFF
/* #define HIP_DEBUG */

/* stuff from lame's tables.c */
static const int  bitrate_table    [3] [16] = {
    { 0,  8, 16, 24, 32, 40, 48, 56,  64,  80,  96, 112, 128, 144, 160, -1 },
    { 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, -1 },
    { 0,  8, 16, 24, 32, 40, 48, 56,  64,  80,  96, 112, 128, 144, 160, -1 },
};

static const int  samplerate_table [3]  [4] = { 
    { 22050, 24000, 16000, -1 },      /* MPEG 2 */
    { 44100, 48000, 32000, -1 },      /* MPEG 1 */  
    { 11025, 12000,  8000, -1 },      /* MPEG 2.5 */
};
/* end stuff from lame's tables.c */

/* stuff from VbrTag.c/.h */
const static char	VBRTag[]={"Xing"};
const static char	VBRTag2[]={"Info"};

#define FRAMES_FLAG     0x0001
#define BYTES_FLAG      0x0002
#define TOC_FLAG        0x0004
#define VBR_SCALE_FLAG  0x0008

#define NUMTOCENTRIES 100

#define FRAMES_AND_BYTES (FRAMES_FLAG | BYTES_FLAG)

/* toc may be NULL*/
typedef struct
{
  int		h_id;			/* from MPEG header, 0=MPEG2, 1=MPEG1 */
  int		samprate;		/* determined from MPEG header */
  int		flags;			/* from Vbr header data */
  int		frames;			/* total bit stream frames from Vbr header data */
  int		bytes;			/* total bit stream bytes from Vbr header data*/
  int		vbr_scale;		/* encoded vbr scale from Vbr header data*/
  unsigned char	toc[NUMTOCENTRIES];	/* may be NULL if toc not desired*/
  int           headersize;             /* size of VBR header, in bytes */
  int           enc_delay;              /* encoder delay */
  int           enc_padding;            /* encoder paddign added at end of stream */
}   VBRTAGDATA;
/* end stuff from VbrTag.c/.h */

static int  ExtractI4(unsigned char *buf);
int         hip_GetVbrTag(VBRTAGDATA *pTagData,  unsigned char *buf);

static int  is_syncword_mp123(const void *const headerptr);


int
hip_decode_init (HIP_File * hf)
{
  HF_MP(hf) = malloc(sizeof(MPSTR));
  if(HF_MP(hf) == NULL)
    return HIP_EFAULT;

  /* init the decoder and clear out our info struct */
  if (!InitMP3 (HF_MP(hf)))
    return HIP_EFAULT;

  /* allows us to distinguish a decoder initialized with a file
     vs. one initialized for use with a memory region. 
   */ 
  hf->datasource = NULL; 
  return 0;
}  

int
hip_open (FILE * file, HIP_File * hf, char *initial, long ibytes)
{
  unsigned char buf[100];
  int ret, len;
  char out[4608];
/*  short int pcm_l[1152], pcm_r[1152]; */


  ret = hip_decode_init(hf);
  if(ret != 0)
    return ret;

  hf->datasource = file;
  
  /* make sure we are looking at a mpeg audio file */
  len = 4;
  if (fread (&buf, 1, len, hf->datasource) != len)
    return HIP_ENOTMPEG;

  /* look for valid 4 byte MPEG header  */
  while (!is_syncword_mp123 (buf))
    {
      int i;
      for (i = 0; i < len - 1; i++)
        buf[i] = buf[i + 1];
      if (fread (buf + len - 1, 1, 1, hf->datasource) != 1)
        return HIP_ENOTMPEG;
    }


  /* now parse the current buffer looking for MP3 headers.   
     (as of 11/00: mpglib modified so that for the first frame where 
     headers are parsed, no data will be decoded.  
     However, for freeformat, we need to decode an entire frame,
     so hf->bitrate will be 0 until we have decoded the first
     frame.  Cannot decode first frame here because we are not
     yet prepared to handle the output.
  */
  
  ret = hip_decode_headers (hf, buf, len, out, sizeof(out));
  if (ret == -1)
    return -1;

  /* repeat until we decode a valid mp3 header.   */
  while (!hf->header_parsed)
    {
      len = fread (buf, 1, sizeof (buf), hf->datasource);
      if (len != sizeof (buf))
        return -1;
      ret =
        hip_decode_headers (hf, buf, len, out, sizeof(out));
      if (-1 == ret)
        return -1;
    }

  if (hf->bitrate == 0)
    {
      /* fprintf (stderr, "Input file is freeformat.\n"); */
    }

  if (hf->totalframes > 0)
    {
      /*
       * mpglib found a Xing VBR header and computed nsamp & totalframes 
       */
    }
  else
    {
      /*
       * set as unknown.  Later, we will take a guess based on file size *
       * ant bitrate 
       */
      hf->nsamp = MAX_U_32_NUM;
    }


#ifdef HIP_DEBUG 
    fprintf(stderr,"exiting hip_open:\n");
    fprintf(stderr,"  ret        = %i NEED_MORE=%i \n", ret, MP3_NEED_MORE);
    fprintf(stderr,"  stereo     = %i\n", HF_MP(hf)->fr.stereo); 
    fprintf(stderr,"  samp       = %li\n", freqs[HF_MP(hf)->fr.sampling_frequency]); 
    fprintf(stderr,"  framesize  = %i\n", HF_MP(hf)->framesize); 
    fprintf(stderr,"  bitrate    = %i\n", hf->bitrate); 
    fprintf(stderr,"  num frames = %u\n", HF_MP(hf)->num_frames); 
    fprintf(stderr,"  num samp   = %li\n", hf->nsamp);
    fprintf(stderr,"  mode       = %i\n", HF_MP(hf)->fr.mode); 
#endif

  return 0;


}


int  
wrap_decodeMP3(PMPSTR mp,unsigned char *inmemory,int inmemsize,char *outmemory,int outmemsize,int *done)
{
  int ret;

  ret = decodeMP3(mp,inmemory,inmemsize,outmemory,outmemsize,done);
  
#ifdef HIP_DEBUG 
  fprintf(stderr,"wrap_decodeMP3: return = %i, bytes = %i\n", ret, *done);
#endif
  
  return ret;
}

void hip_decode_reset(HIP_File *hf)
{
  decode_reset(HF_MP(hf));
}

int hip_audiodata_precedesframes(HIP_File *hf)
{
  return audiodata_precedesframes(HF_MP(hf));
}

long
hip_read (HIP_File * hf, char *out_buffer, int out_buffer_len,
          int bigendianp, int word, int sgned, int *bitstream)
{
  int      in_buffer_len = 0;
  unsigned char in_buffer[1024];
  int     processed_bytes;
  int     decode_status;
  
  memset(out_buffer, 0, out_buffer_len);
        
  /* first see if we still have data buffered in the decoder: */
  decode_status = wrap_decodeMP3(HF_MP(hf), in_buffer, in_buffer_len, out_buffer, out_buffer_len, &processed_bytes);
  if (decode_status == MP3_NEED_MORE || decode_status == MP3_OK)  // LMS
								  // added check.
    { 
      /* read until we get a valid output frame */
      while (1) 
	{
	  in_buffer_len = fread(in_buffer, 1, sizeof(in_buffer), hf->datasource);
	  if (in_buffer_len == 0) 
	    {
	      /* we are done reading the file, but check for buffered data */
	      decode_status = wrap_decodeMP3(HF_MP(hf), in_buffer, in_buffer_len, out_buffer, out_buffer_len, &processed_bytes);
	      if (decode_status == MP3_NEED_MORE || decode_status == MP3_ERR) 
		return 0;  // done with file
	      break;
	    }  
       
	  decode_status = wrap_decodeMP3(HF_MP(hf), in_buffer, in_buffer_len, out_buffer, out_buffer_len, &processed_bytes);

	  if (processed_bytes == -1) 
	    /* FIXME: is this the right error code? */
	    return HIP_HOLE;
	  if (decode_status == MP3_OK && processed_bytes > 0) 
	    break;
	}
    }
  
  return (long)processed_bytes;
}

int
hip_clear (HIP_File * hf)
{
  ExitMP3 (HF_MP(hf));
  free(hf->mp);
  
  return 0;
}

mpeg_info *
hip_info(HIP_File * hf, int link) 
{
  mpeg_info *mi;
  mi = malloc(sizeof(mpeg_info));
  if(mi == NULL)
    return NULL;
  mi->channels = hf->stereo;
  mi->rate = (long)hf->samplerate;
  mi->bitrate_upper = (long)hf->bitrate;
  mi->bitrate_nominal = (long)hf->bitrate;
  mi->bitrate_lower = (long)hf->bitrate;

  return mi;
}

/* from lame's mpglib_interface.c */
/*
 * For hip_decode_headers:  return code
 * -1     error
 *  0     ok, but need more data before outputing any samples
 *  n     number of samples output.  either 576 or 1152 depending on MP3 file.
 */
int
hip_decode_headers(HIP_File * hf, unsigned char *in_buffer, int in_buffer_len, 
    char *out_buffer, int out_buffer_len)
{
    static const int smpls[2][4] = {
        /* Layer   I    II   III */
        {0, 384, 1152, 1152}, /* MPEG-1     */
        {0, 384, 1152, 576} /* MPEG-2(.5) */
    };
    int     processed_bytes;
    int     decode_status;
    int     ret;

    hf->header_parsed = 0;

    /* fprintf(stderr, "HF_MP(hf) = %p\n", HF_MP(hf)); */
    decode_status =
        decodeMP3(HF_MP(hf), in_buffer, in_buffer_len, out_buffer, out_buffer_len, &processed_bytes);
    /* three cases:  
     * 1. headers parsed, but data not complete
     *       HF_MP(hf)->header_parsed==1 
     *       HF_MP(hf)->framesize=0           
     *       HF_MP(hf)->fsizeold=size of last frame, or 0 if this is first frame
     *
     * 2. headers, data parsed, but ancillary data not complete
     *       HF_MP(hf)->header_parsed==1 
     *       HF_MP(hf)->framesize=size of frame           
     *       HF_MP(hf)->fsizeold=size of last frame, or 0 if this is first frame
     *
     * 3. frame fully decoded:  
     *       HF_MP(hf)->header_parsed==0 
     *       HF_MP(hf)->framesize=0           
     *       HF_MP(hf)->fsizeold=size of frame (which is now the last frame)
     *
     */
    if (HF_MP(hf)->header_parsed || HF_MP(hf)->fsizeold > 0 || HF_MP(hf)->framesize > 0) {
      	hf->header_parsed = 1;
        hf->stereo = HF_MP(hf)->fr.stereo;
        hf->samplerate = freqs[HF_MP(hf)->fr.sampling_frequency];
        hf->mode = HF_MP(hf)->fr.mode;
        hf->mode_ext = HF_MP(hf)->fr.mode_ext;
        hf->framesize = smpls[HF_MP(hf)->fr.lsf][HF_MP(hf)->fr.lay];

        /* free format, we need the entire frame before we can determine
         * the bitrate.  If we haven't gotten the entire frame, bitrate=0 */
        if (HF_MP(hf)->fsizeold > 0) /* works for free format and fixed, no overrun, temporal results are < 400.e6 */
            hf->bitrate = 8 * (4 + HF_MP(hf)->fsizeold) * hf->samplerate /
                (1.e3 * hf->framesize) + 0.5;
        else if (HF_MP(hf)->framesize > 0)
            hf->bitrate = 8 * (4 + HF_MP(hf)->framesize) * hf->samplerate /
                (1.e3 * hf->framesize) + 0.5;
        else
            hf->bitrate =
                tabsel_123[HF_MP(hf)->fr.lsf][HF_MP(hf)->fr.lay - 1][HF_MP(hf)->fr.bitrate_index];



        if (HF_MP(hf)->num_frames > 0) {
            /* Xing VBR header found and num_frames was set */
            hf->totalframes = HF_MP(hf)->num_frames;
            hf->nsamp = hf->framesize * HF_MP(hf)->num_frames;
        }
    }

    switch (decode_status) {
    case MP3_OK:
        ret = processed_bytes;
        break;

    case MP3_NEED_MORE:
        ret = 0;
        break;

    case MP3_ERR:
        ret = -1;
        break;

    default:
        assert(0);

    }

#ifdef HIP_DEBUG
    fprintf(stderr,"hip_decode_headers: decode status = %i / ret = %i\n", ret, decode_status);
#endif
    return ret;
}

/* from lame's get_audio.c */
static int
is_syncword_mp123(const void *const headerptr)
{
    const unsigned char *const p = headerptr;
    static const char abl2[16] =
        { 0, 7, 7, 7, 0, 7, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8 };

    if ((p[0] & 0xFF) != 0xFF)
        return 0;       // first 8 bits must be '1'
    if ((p[1] & 0xE0) != 0xE0)
        return 0;       // next 3 bits are also
    if ((p[1] & 0x18) == 0x08)
        return 0;       // no MPEG-1, -2 or -2.5
    if ((p[1] & 0x06) == 0x00)
        return 0;       // no Layer I, II and III
    if ((p[2] & 0xF0) == 0xF0)
        return 0;       // bad bitrate
    if ((p[2] & 0x0C) == 0x0C)
        return 0;       // no sample frequency with (32,44.1,48)/(1,2,4)    
    if (((p[1] & 0x18) == 0x18) && ((p[1] & 0x06) == 0x04)) // illegal Layer II bitrate/Channel Mode comb
        if (abl2[p[2] >> 4] & (1 << (p[3] >> 6)))
            return 0;
    if ((p[3] & 3) == 2)
        return 0;       /* reserved enphasis mode */
    return 1;
}


/* from lame's VbrTag.c */
static int 
ExtractI4(unsigned char *buf)
{
	int x;
	/* big endian extract */
	x = buf[0];
	x <<= 8;
	x |= buf[1];
	x <<= 8;
	x |= buf[2];
	x <<= 8;
	x |= buf[3];
	return x;
}

/* from lame's VbrTag.c */
int 
hip_GetVbrTag(VBRTAGDATA *pTagData,  unsigned char *buf)
{
	int			i, head_flags;
	int			h_bitrate,h_id, h_mode, h_sr_index;
        int enc_delay,enc_padding; 

	/* get Vbr header data */
	pTagData->flags = 0;

	/* get selected MPEG header data */
	h_id       = (buf[1] >> 3) & 1;
	h_sr_index = (buf[2] >> 2) & 3;
	h_mode     = (buf[3] >> 6) & 3;
        h_bitrate  = ((buf[2]>>4)&0xf);
	h_bitrate = bitrate_table[h_id][h_bitrate];

        /* check for FFE syncword */
        if ((buf[1]>>4)==0xE) 
            pTagData->samprate = samplerate_table[2][h_sr_index];
        else
            pTagData->samprate = samplerate_table[h_id][h_sr_index];
        /*	if( h_id == 0 )
        		pTagData->samprate >>= 1;
	*/
	/*  determine offset of header */
	if( h_id )
	{
                /* mpeg1 */
		if( h_mode != 3 )	buf+=(32+4);
		else				buf+=(17+4);
	}
	else
	{
                /* mpeg2 */
		if( h_mode != 3 ) buf+=(17+4);
		else              buf+=(9+4);
	}

	if( buf[0] != VBRTag[0] && buf[0] != VBRTag2[0] ) return 0;    /* fail */
	if( buf[1] != VBRTag[1] && buf[1] != VBRTag2[1]) return 0;    /* header not found*/
	if( buf[2] != VBRTag[2] && buf[2] != VBRTag2[2]) return 0;
	if( buf[3] != VBRTag[3] && buf[3] != VBRTag2[3]) return 0;

	buf+=4;

	pTagData->h_id = h_id;

	head_flags = pTagData->flags = ExtractI4(buf); buf+=4;      /* get flags */

	if( head_flags & FRAMES_FLAG )
	{
		pTagData->frames   = ExtractI4(buf); buf+=4;
	}

	if( head_flags & BYTES_FLAG )
	{
		pTagData->bytes = ExtractI4(buf); buf+=4;
	}

	if( head_flags & TOC_FLAG )
	{
		if( pTagData->toc != NULL )
		{
			for(i=0;i<NUMTOCENTRIES;i++)
				pTagData->toc[i] = buf[i];
		}
		buf+=NUMTOCENTRIES;
	}

	pTagData->vbr_scale = -1;

	if( head_flags & VBR_SCALE_FLAG )
	{
		pTagData->vbr_scale = ExtractI4(buf); buf+=4;
	}

	pTagData->headersize = 
	  ((h_id+1)*72000*h_bitrate) / pTagData->samprate;

        buf+=21;
        enc_delay = buf[0] << 4;
        enc_delay += buf[1] >> 4;
        enc_padding= (buf[1] & 0x0F)<<8;
        enc_padding += buf[2];
        // check for reasonable values (this may be an old Xing header,
        // not a INFO tag)
        if (enc_delay<0 || enc_delay > 3000) enc_delay=-1;
        if (enc_padding<0 || enc_padding > 3000) enc_padding=-1;

        pTagData->enc_delay=enc_delay;
        pTagData->enc_padding=enc_padding;

#ifdef HIP_DEBUG
	fprintf(stderr,"exit hip_GetVbrTag:\n");
	fprintf(stderr,"  tag         = %s\n",VBRTag);
	fprintf(stderr,"  head_flags  = %d\n",head_flags);
	fprintf(stderr,"  bytes       = %d\n",pTagData->bytes);
	fprintf(stderr,"  frames      = %d\n",pTagData->frames);
	fprintf(stderr,"  VBR Scale   = %d\n",pTagData->vbr_scale);
	fprintf(stderr,"  enc_delay   = %i \n",enc_delay);
	fprintf(stderr,"  enc_padding = %i \n",enc_padding);
	fprintf(stderr,"  toc         =\n");
	if( pTagData->toc != NULL )
	{
		for(i=0;i<NUMTOCENTRIES;i++)
		{
			if( (i%10) == 0 ) fprintf(stderr,"\n");
			fprintf(stderr," %3d", (int)(pTagData->toc[i]));
		}
	}
	fprintf(stderr,"\n");
#endif
	return 1;       /* success */
}
