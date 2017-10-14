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

/********************************************************************

 This is copied from vorbisfile.h, the interface to the Ogg Vorbis codec.
 We liked it so much we made it our own.
 
 function: stdio-based convenience library for opening/seeking/decoding

 ********************************************************************/

#ifndef _HIP_H_
#define _HIP_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include <stdio.h>
#include <sys/types.h>

typedef int64_t hip_int64_t;

/* The function prototypes for the callbacks are basically the same as for
 * the stdio functions fread, fseek, fclose, ftell. 
 * The one difference is that the FILE * arguments have been replaced with
 * a void * - this is to be used as a pointer to whatever internal data these
 * functions might need. In the stdio case, it's just a FILE * cast to a void *
 * 
 * If you use other functions, check the docs for these functions and return
 * the right values. For seek_func(), you *MUST* return -1 if the stream is
 * unseekable
 */
typedef struct {
  size_t (*read_func)  (void *ptr, size_t size, size_t nmemb, void *datasource);
  int    (*seek_func)  (void *datasource, hip_int64_t offset, int whence);
  int    (*close_func) (void *datasource);
  long   (*tell_func)  (void *datasource);
} hip_callbacks;

#define  NOTOPEN   0
#define  PARTOPEN  1
#define  OPENED    2
#define  STREAMSET 3
#define  INITSET   4


/* hip ERRORS and return codes ***********************************/

#define HIP_FALSE      -1  
#define HIP_EOF        -2
#define HIP_HOLE       -3

#define HIP_EREAD      -128
#define HIP_EFAULT     -129
#define HIP_EIMPL      -130  
#define HIP_EINVAL     -131
#define HIP_ENOTMPEG   -132
#define HIP_EBADHEADER -133
#define HIP_EVERSION   -134
#define HIP_ENOTAUDIO  -135
#define HIP_EBADPACKET -136
#define HIP_EBADLINK   -137
#define HIP_ENOSEEK    -138


typedef struct HIP_File {
  void            *datasource; /* Pointer to a FILE *, etc. */
  int              seekable;
  hip_int64_t      offset;
  hip_int64_t      end;

  /* If the FILE handle isn't seekable (eg, a pipe), only the current
     stream appears */
  hip_int64_t      dataoffset;
  hip_int64_t      pcmlength;

  /* Decoding working state local storage */
  hip_int64_t      pcm_offset;
  int              ready_state;
  
  hip_callbacks callbacks;

  /* this is the MPSTR */
  void * mp;   

  /* stuff we need from lame.h's mp3data_struct */
  int header_parsed;   /* 1 if header was parsed and following data was
                          computed                                       */
  int stereo;          /* number of channels                             */
  int samplerate;      /* sample rate                                    */
  int bitrate;         /* bitrate                                        */
  int mode;            /* mp3 frame type                                 */
  int mode_ext;        /* mp3 frame type                                 */
  int framesize;       /* number of samples per mp3 frame                */

  /* this data is only computed if mpglib detects a Xing VBR header */
  unsigned long nsamp; /* number of samples in mp3 file.                 */
  int totalframes;     /* total number of frames in mp3 file             */

  /* this data is not currently computed by the mpglib routines */
  int framenum;        /* frames decoded counter                         */
} HIP_File;

typedef struct mpeg_info{
  int channels;
  long rate;
  
  long bitrate_upper;
  long bitrate_nominal;
  long bitrate_lower;
} mpeg_info;


extern int hip_decode_init (HIP_File * hf);
 
/** hip_open
    
    This is the main function used to open and initialize an HIP_File
    structure. It sets up all the related decoding structure.
    
    @param file File pointer to an already opened file or pipe (it need not
    be seekable--though this obviously restricts what can be done with the
    bitstream).

    @param hf A pointer to the HIP_File structure--this is used for ALL
    the externally visible HIP functions. Once this has been called,
    the same HIP_File struct should be passed to all the HIP
    functions.
    
    @param initial (not currently used) Typically set to NULL. This parameter
    is useful if some data has already been read from the file and the
    stream is not seekable. It is used in conjunction with ibytes. In this
    case, initial should be a pointer to a buffer containing the data read.

    @param ibytes (not currently used) Typically set to 0. This parameter is
    useful if some data has already been read from the file and the stream
    is not seekable. In this case, ibytes should contain the length (in
    bytes) of the buffer.  Used together with initial
  */
extern int hip_open(FILE *f,HIP_File *hf,char *initial,long ibytes);
extern long hip_read(HIP_File *hf,char *buffer,int length,
		    int bigendianp,int word,int sgned,int *bitstream);
extern int hip_clear(HIP_File *hf);
extern mpeg_info *hip_info(HIP_File *hf,int link);

extern int hip_decode_headers(HIP_File * hf, unsigned char *in_buffer, int in_buffer_len, char *out_buffer, int out_buffer_len);

/* Resets the decoding, discarding partially decoded data. */
extern void hip_decode_reset(HIP_File *hf);

/* Returns the number of MP3 frames the audio data precedes the current frame, useful for noncontiguous decoding of frames. */
extern int hip_audiodata_precedesframes(HIP_File *hf);

/*

extern int hip_open_callbacks(void *datasource, HIP_File *hf,
		char *initial, long ibytes, hip_callbacks callbacks);

extern int hip_test(FILE *f,HIP_File *hf,char *initial,long ibytes);
extern int hip_test_callbacks(void *datasource, HIP_File *hf,
		char *initial, long ibytes, hip_callbacks callbacks);
extern int hip_test_open(HIP_File *hf);

extern long hip_bitrate(HIP_File *hf,int i);
extern long hip_bitrate_instant(HIP_File *hf);
extern long hip_streams(HIP_File *hf);
extern long hip_seekable(HIP_File *hf);

extern hip_int64_t hip_raw_total(HIP_File *hf,int i);
extern hip_int64_t hip_pcm_total(HIP_File *hf,int i);
extern double hip_time_total(HIP_File *hf,int i);

extern int hip_raw_seek(HIP_File *hf,long pos);
extern int hip_pcm_seek(HIP_File *hf,hip_int64_t pos);
extern int hip_pcm_seek_page(HIP_File *hf,hip_int64_t pos);
extern int hip_time_seek(HIP_File *hf,double pos);
extern int hip_time_seek_page(HIP_File *hf,double pos);

extern hip_int64_t hip_raw_tell(HIP_File *hf);
extern hip_int64_t hip_pcm_tell(HIP_File *hf);
extern double hip_time_tell(HIP_File *hf);
*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HIP_H_ */
