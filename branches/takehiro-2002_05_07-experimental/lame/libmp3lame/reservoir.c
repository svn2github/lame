/*
 *	bit reservoir source file
 *
 *	Copyright (c) 1999 Mark Taylor
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

/* $Id$ */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <assert.h>
#include "bitstream.h"
#include "reservoir.h"

#ifdef WITH_DMALLOC
#include <dmalloc.h>
#endif

/*
  ResvFrameBegin:
  Called (repeatedly) at the beginning of a frame. Updates the maximum
  size of the reservoir, and checks to make sure main_data_begin
  was set properly by the formatter
*/

/*
 *  Background information:
 *
 *  This is the original text from the ISO standard. Because of 
 *  sooo many bugs and irritations correcting comments are added 
 *  in brackets []. A '^W' means you should remove the last word.
 *
 *  1) The following rule can be used to calculate the maximum 
 *     number of bits used for one granule [^W frame]: 
 *     At the highest possible bitrate of Layer III (320 kbps 
 *     per stereo signal [^W^W^W], 48 kHz) the frames must be of
 *     [^W^W^W are designed to have] constant length, i.e. 
 *     one buffer [^W^W the frame] length is:
 *
 *         320 kbps * 1152/48 kHz = 7680 bit = 960 byte
 *
 *     This value is used as the maximum buffer per channel [^W^W] at 
 *     lower bitrates [than 320 kbps]. At 64 kbps mono or 128 kbps 
 *     stereo the main granule length is 64 kbps * 576/48 kHz = 768 bit
 *     [per granule and channel] at 48 kHz sampling frequency. 
 *     This means that there is a maximum deviation (short time buffer 
 *     [= reservoir]) of 7680 - 2*2*768 = 4608 bits is allowed at 64 kbps. 
 *     The actual deviation is equal to the number of bytes [with the 
 *     meaning of octets] denoted by the main_data_end offset pointer. 
 *     The actual maximum deviation is (2^9-1)*8 bit = 4088 bits 
 *     [for MPEG-1 and (2^8-1)*8 bit for MPEG-2, both are hard limits].
 *     ... The xchange of buffer bits between the left and right channel
 *     is allowed without restrictions [exception: dual channel].
 *     Because of the [constructed] constraint on the buffer size
 *     main_data_end is always set to 0 in the case of bit_rate_index==14, 
 *     i.e. data rate 320 kbps per stereo signal [^W^W^W]. In this case 
 *     all data are allocated between adjacent header [^W sync] words 
 *     [, i.e. there is no buffering at all].
 */

int
ResvFrameBegin(lame_global_flags *gfp, int *mean_bits)
{
    lame_internal_flags *gfc=gfp->internal_flags;
    int fullFrameBits;
    int resvLimit;
    int maxmp3buf;
    III_side_info_t     *l3_side = &gfc->l3_side;
    int frameLength;

    frameLength = getframebits(gfp);
    *mean_bits = (frameLength - l3_side->sideinfo_len * 8) / gfc->mode_gr;

/*
 *  Meaning of the variables:
 *      resvLimit: (0, 8, ..., 8*255 (MPEG-2), 8*511 (MPEG-1))
 *          Number of bits can be stored in previous frame(s) due to 
 *          counter size constaints
 *      maxmp3buf: ( ??? ... 8*1951 (MPEG-1 and 2), 8*2047 (MPEG-2.5))
 *          Number of bits allowed to encode one frame (you can take 8*511 bit 
 *          from the bit reservoir and at most 8*1440 bit from the current 
 *          frame (320 kbps, 32 kHz), so 8*1951 bit is the largest possible 
 *          value for MPEG-1 and -2)
 *       
 *          maximum allowed granule/channel size times 4 = 8*2047 bits.,
 *          so this is the absolute maximum supported by the format.
 *
 *          
 *      fullFrameBits:  maximum number of bits available for encoding
 *                      the current frame.
 *
 *      mean_bits:      target number of bits per granule.  
 *
 *      frameLength:
 *
 *      l3_side->ResvMax:   maximum allowed reservoir 
 *
 *      l3_side->ResvSize:  current reservoir size
 *
 *      l3_side->resvDrain_pre:
 *         ancillary data to be added to previous frame:
 *         (only usefull in VBR modes if it is possible to have
 *         maxmp3buf < fullFrameBits)).  Currently disabled, 
 *         see #define NEW_DRAIN
 *
 *      l3_side->resvDrain_post:
 *         ancillary data to be added to this frame:
 *
 */

    if (gfp->mean_bitrate_kbps >= 320) {
        /* in freeformat the buffer is constant*/
        maxmp3buf = 8*((int)((gfp->mean_bitrate_kbps*1000 * 1152/8)
			     /gfp->out_samplerate +.5));
    } else {
        /* maximum allowed frame size.  dont use more than this number of
           bits, even if the frame has the space for them: */
        /* Bouvigne suggests this more lax interpretation of the ISO doc 
           instead of using 8*960. */

	/*all mp3 decoders should have enough buffer to handle this value: size of a 320kbps 32kHz frame*/
	maxmp3buf = 8*1440;

        if (gfp->strict_ISO)
	    maxmp3buf = ((int)(320000*1152 / gfp->out_samplerate + 4)) & (~7);
    }

    l3_side->ResvMax = maxmp3buf - frameLength;
    /* main_data_begin has 9 bits in MPEG-1, 8 bits MPEG-2 */
    resvLimit = (8*256)*gfc->mode_gr-8;
    if (l3_side->ResvMax > resvLimit)
	l3_side->ResvMax = resvLimit;
    if (l3_side->ResvMax < 0 ||  gfp->disable_reservoir)
	l3_side->ResvMax = 0;
    assert ( 0 == l3_side->ResvMax % 8 );

    fullFrameBits
	= *mean_bits * gfc->mode_gr + Min(l3_side->ResvSize, l3_side->ResvMax);

    if (fullFrameBits > maxmp3buf)
        fullFrameBits = maxmp3buf;

    l3_side->resvDrain_pre = 0;
#ifdef HAVE_GTK
    if (gfc->pinfo) {
        gfc->pinfo->mean_bits = *mean_bits / gfc->channels_out;
        gfc->pinfo->resvsize  = l3_side->ResvSize;
    }
#endif

    return fullFrameBits;
}


/*
  ResvMaxBits
  returns targ_bits:  target number of bits to use for 1 granule
         extra_bits:  amount extra available from reservoir
  Mark Taylor 4/99
*/
void ResvMaxBits(lame_internal_flags *gfc, int mean_bits, int *targ_bits, int *extra_bits)
{
    int ResvSize = gfc->l3_side.ResvSize, ResvMax = gfc->l3_side.ResvMax;
    if (ResvSize*10 >= ResvMax*6) {
	int add_bits = 0;
	if (ResvSize*10 >= ResvMax*9) {
	    /* reservoir is filled over 90% -> forced to use excessed bits */
	    add_bits = ResvSize - (ResvMax * 9) / 10;
	    mean_bits += add_bits;
	    gfc->substep_shaping |= 0x80;
	}
	/* max amount from the reservoir we are allowed to use. ISO says 60% */
	ResvSize = (ResvMax*6)/10 - add_bits;
	if (ResvSize < 0) ResvSize = 0;
    } else {
	/* build up reservoir.  this builds the reservoir a little slower
	 * than FhG.  It could simple be mean_bits/15, but this was rigged
	 * to always produce 100 (the old value) at 128kbs */
	/*    *targ_bits -= (int) (mean_bits/15.2);*/
	if (!(gfc->substep_shaping & 1))
	    mean_bits -= mean_bits/10;
	gfc->substep_shaping &= 0x7f;
    }

    *targ_bits = mean_bits;
    *extra_bits = ResvSize;
}



/*
  ResvFrameEnd:
  Called after all granules in a frame have been allocated. Makes sure
  that the reservoir size is within limits, possibly by adding stuffing
  bits.
*/
void
ResvFrameEnd(III_side_info_t *l3_side, int mean_bits)
{
    int stuffingBits, over_bits;

    l3_side->ResvSize += mean_bits;
    l3_side->resvDrain_pre = 0;

    /* we must be byte aligned */
    stuffingBits = l3_side->ResvSize % 8;
    over_bits = (l3_side->ResvSize - stuffingBits) - l3_side->ResvMax;
    if (over_bits > 0) /* reservoir is overflowed */
	stuffingBits += over_bits;
    l3_side->ResvSize -= stuffingBits;

#undef NEW_DRAIN
#ifdef NEW_DRAIN
    /* drain as many bits as possible into previous frame ancillary data
     * In particular, in VBR mode ResvMax may have changed, and we have
     * to make sure main_data_begin does not create a reservoir bigger
     * than ResvMax  mt 4/00*/
    mean_bits = Min(l3_side->main_data_begin, stuffingBits/8);
    l3_side->main_data_begin -= mean_bits;

    mean_bits *= 8;
    l3_side->resvDrain_pre = mean_bits;
    stuffingBits -= mean_bits;

    /* drain just enough to be byte aligned.  The remaining bits will
     * be added to the reservoir, and we will deal with them next frame.
     * If the next frame is at a lower bitrate, it may have a larger ResvMax,
     * and we will not have to waste these bits!  mt 4/00 */
#endif
    /* drain the rest into this frames ancillary data*/
    l3_side->resvDrain_post = stuffingBits;

    return;
}
