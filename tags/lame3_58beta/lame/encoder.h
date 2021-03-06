#ifndef ENCODER_DOT_H
#define ENCODER_DOT_H
/***********************************************************************
*
*  encoder and decoder delays
*
***********************************************************************/
/* 
layerIII enc->dec delay:  1056    (observed)
layerII  enc->dec dealy:   480    (observed)


polyphase 256-16             (dec or enc)        = 240
mdct      256+32  (9*32)     (dec or enc)        = 288
total:    512+16

*/


/* original ISO routiens had a delay of 528! */
/* Takehiro's routines: */
#define MDCTDELAY 48  

/* 
   The encoder delay.  Minimum allowed is MDCTDELAY (see encoder.h)
   Setting ENCDELAY < MDCTDELAY will result in loss of PCM samples
   samples at the start of the file will be lost.

   suggested: 48
   set to 1160 to sync with FhG.
*/
#define ENCDELAY 48






/*
Most decoders, including the one we use,  have a delay of 528 samples.  
*/
#define DECDELAY 528
#define FFTOFFSET (224+MDCTDELAY)


/* number of subbands */
#define         SBLIMIT                 32

/* parition bands bands */
#define CBANDS          63

/* number of critical bands/scale factor bands where masking is computed*/
#define SBPSY_l 21
#define SBPSY_s 12

/* total number of scalefactor bands encoded */
#define SBMAX_l 22
#define SBMAX_s 13



/* FFT sizes */
#define BLKSIZE         1024
#define HBLKSIZE        513
#define BLKSIZE_s 256
#define HBLKSIZE_s 129


/* #define switch_pe        1800 */
#define NORM_TYPE       0
#define START_TYPE      1
#define SHORT_TYPE      2
#define STOP_TYPE       3
#endif
