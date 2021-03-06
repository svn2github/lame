/* 
 * Mpeg Layer-1 audio decoder 
 * --------------------------
 * copyright (c) 1995 by Michael Hipp, All rights reserved. See also 'README'
 * near unoptimzed ...
 *
 * may have a few bugs after last optimization ... 
 *
 */

#ifdef USE_LAYER_1

#include <assert.h>
#include "mpg123.h"

real muls [27] [64];   // used by Layer 1 and 2 decoding


void   I_step_one ( 
		unsigned int         balloc [], 
		unsigned int         scale_index [2] [SBLIMIT],
		const struct frame*  fr )
{
    unsigned int*  sca = (unsigned int*) scale_index;
    unsigned int*  ba;
    unsigned int   jsbound;
    unsigned int   i;
    
    switch ( fr -> channels ) {
    case 1:
        ba = balloc;
        for ( i = 0; i < SBLIMIT; i++ )
            *ba++ = getbits (4);
	    
        ba = balloc;
        for ( i = 0; i < SBLIMIT; i++ )
            if (*ba++)
                *sca++ = getbits (6);
	break;
    case 2:
	jsbound = fr -> jsbound;
	
        ba = balloc;
	for ( i = 0; i < jsbound; i++ ) { 
            *ba++ = getbits (4);
            *ba++ = getbits (4);
        }
        for ( i = jsbound; i < SBLIMIT; i++ )
            *ba++ = getbits (4);

        ba = balloc;
        for ( i = 0; i < jsbound; i++ ) {
            if (*ba++)
    		*sca++ = getbits (6);
            if (*ba++)
    		*sca++ = getbits (6);
        }
        for ( i = jsbound; i < SBLIMIT; i++)
            if (*ba++) {
                *sca++ = getbits (6);
                *sca++ = getbits (6);
            }
	break;
    default:
        assert (0);
	break;
    }
}

void I_step_two (
		real                 fraction    [2] [SBLIMIT],
		unsigned int         balloc      [2 * SBLIMIT],
	        unsigned int         scale_index [2] [SBLIMIT],
		const struct frame*  fr)
{
    int             smpb [2*SBLIMIT];                   /* values: 0...65535 */
    int*            sample;
    unsigned int*   sca = (unsigned int*) scale_index;
    unsigned int*   ba;
    register real*  f0;
    register real*  f1;
    real            samp;
    int             jsbound;
    int             i;
    int             n;

    switch ( fr -> channels ) {
    case 1:
        f0     = fraction [0];
	
        ba     = balloc;
        sample = smpb;
        for ( i = 0; i < SBLIMIT; i++ )
            if ( (n = *ba++) != 0 )
                *sample++ = getbits(n+1);
		
        ba     = balloc;
        sample = smpb;
        for ( i = 0; i < SBLIMIT; i++ ) {
            if ( (n=*ba++) != 0 )
                *f0++ = (real) ( (-1<<n) + 1 + *sample++) * muls [n+1] [*sca++];
            else
                *f0++ = 0.;
        }
	
        for ( i = fr -> down_sample_sblimit; i < 32; i++ )
            fraction [0] [i] = 0.;
	break;
	
    case 2:
	jsbound = fr -> jsbound;
	f0      = fraction [0];
	f1      = fraction [1];
	
	ba      = balloc;
	sample  = smpb;
        for ( i = 0; i < jsbound; i++ ) {
            if ( (n = *ba++) != 0 )
    		*sample++ = getbits (n+1);
            if ( (n = *ba++) != 0 )
    		*sample++ = getbits(n+1);
        }
        for ( i = jsbound; i < SBLIMIT; i++ ) 
            if ( (n = *ba++) != 0 )
                *sample++ = getbits (n+1);

        ba      = balloc;
	sample  = smpb;
        for ( i = 0; i < jsbound; i++ ) {
            if ( (n = *ba++) != 0 )
                *f0++ = (real) ( (-1<<n) + 1 + *sample++) * muls [n+1] [*sca++];
            else
                *f0++ = 0.;
		
            if ( (n = *ba++) != 0 )
                *f1++ = (real) ( (-1<<n) + 1 + *sample++) * muls [n+1] [*sca++];
            else
                *f1++ = 0.;
        }
        for ( i = jsbound; i < SBLIMIT; i++ ) {
            if ( (n = *ba++) != 0 ) {
                samp = (real) ( (-1<<n) + 1 + *sample++ );
                *f0++ = samp * muls [n+1] [*sca++];
                *f1++ = samp * muls [n+1] [*sca++];
            } else {
                *f0++ = *f1++ = 0.;
	    }
        }
	
        for ( i = fr -> down_sample_sblimit; i < 32; i++ )
            fraction [0] [i] = fraction [1] [i] = 0.;
        break;
	
    default:
        assert (0);
	break;
    }
}

//int do_layer1(struct frame *fr,int outmode,struct audio_info_struct *ai)
int do_layer1(struct frame *fr,unsigned char *pcm_sample,int *pcm_point)
{
  int clip=0;
  int i,channels = fr->channels;
  unsigned int balloc[2*SBLIMIT];
  unsigned int scale_index[2][SBLIMIT];
  real fraction[2][SBLIMIT];
  int single = fr->single;

  fr->jsbound = (fr->mode == MPG_MD_JOINT_STEREO) ? (fr->mode_ext<<2)+4 : 32;

  if(channels == 1 || single == 3)
    single = 0;

  I_step_one(balloc,scale_index,fr);

  for (i=0;i<SCALE_BLOCK;i++)
  {
    I_step_two(fraction,balloc,scale_index,fr);

    if(single >= 0)
    {
      clip += synth_1to1_mono( (real *) fraction[single],pcm_sample,pcm_point);
    }
    else {
        int p1 = *pcm_point;
        clip += synth_1to1( (real *) fraction[0],0,pcm_sample,&p1);
        clip += synth_1to1( (real *) fraction[1],1,pcm_sample,pcm_point);
    }

//    if(pcm_point >= audiobufsize)
//      audio_flush(outmode,ai);
  }

  return clip;
}


#endif
