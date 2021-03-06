#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#ifdef _WIN32
/* needed to set stdout to binary */
#include <io.h>
#endif
/*
 main.c is example code for how to use libmp3lame.a.  To use this library,
 you only need the library and lame.h.  All other .h files are private
 to the library.
*/
#include "lame.h"

#ifdef HAVEGTK
#include "gtkanal.h"
#include <gtk/gtk.h>
#endif

#ifdef __riscos__
#include "asmstuff.h"
#endif

/* PLL 14/04/2000 */
#if macintosh
#include <console.h>
#endif


/************************************************************************
*
* main
*
* PURPOSE:  MPEG-1,2 Layer III encoder with GPSYCHO
* psychoacoustic model.
*
************************************************************************/


int main(int argc, char **argv)
{

  char mp3buffer[LAME_MAXMP3BUFFER];
  short int Buffer[2][1152];
  int iread,imp3;
  lame_global_flags gf;
  FILE *outf;
#ifdef __riscos__
  int i;
#endif

#if macintosh
  argc = ccommand(&argv);
#endif

  extern void main_CRC_init (void);

  main_CRC_init ();  /* ulgy, it's C, not Ada, C++ or Modula */
  
  /* initialize libmp3lame */
  if (lame_init(&gf)<0) {
    fprintf(stderr,"fatal error during initialization\n");
    exit(-1);
  }
  if(argc==1) lame_usage(&gf,argv[0]);  /* no command-line args, print usage, exit  */

  /* parse the command line arguments, setting various flags in the
   * struct 'gf'.  If you want to parse your own arguments,
   * or call libmp3lame from a program which uses a GUI to set arguments,
   * skip this call and set the values of interest in the gf struct.
   * (see lame.h for documentation about these parameters)
   */
  lame_parse_args(&gf,argc, argv);

  /* Mostly it is not useful to use the same input and output name.
     This test is very easy and buggy and don't recognize different names
     assigning the same file
   */
  if ( 0 != strcmp ( "-"      , gf.outPath )  && 
       0 == strcmp ( gf.inPath, gf.outPath ) ) {
      fprintf(stderr,"Input file and Output file are the same. Abort.\n" );
      exit(-1);
  }

  /* open the wav/aiff/raw pcm or mp3 input file.  This call will
   * open the file with name gf.inFile, try to parse the headers and
   * set gf.samplerate, gf.num_channels, gf.num_samples.
   * if you want to do your own file input, skip this call and set
   * samplerate, num_channels and num_samples yourself.
   */
  
  lame_init_infile(&gf);

  /* Now that all the options are set, lame needs to analyze them and
   * set some more internal options and check for problems
   */
  if (lame_init_params(&gf)<0)  {
    fprintf(stderr,"fatal error during initialization\n");
    exit(-1);
  }

  if (!gf.decode_only)
    lame_print_config(&gf);   /* print usefull information about options being used */


  if (!gf.gtkflag) {
    /* open the output file */
    if (!strcmp(gf.outPath, "-")) {
#ifdef __EMX__
      _fsetmode(stdout,"b");
#elif (defined  __BORLANDC__)
      setmode(_fileno(stdout), O_BINARY);
#elif (defined  __CYGWIN__)
      setmode(fileno(stdout), _O_BINARY);
#elif (defined _WIN32)
      _setmode(_fileno(stdout), _O_BINARY);
#endif
      outf = stdout;
    } else {
      if ((outf = fopen(gf.outPath, "wb+")) == NULL) {
	fprintf(stderr,"Could not create \"%s\".\n", gf.outPath);
	exit(-1);
      }
    }
#ifdef __riscos__
    /* Assign correct file type */
    for (i = 0; gf.outPath[i]; i++) {
      if (gf.outPath[i] == '.')
        gf.outPath[i] = '/';
      else if (gf.outPath[i] == '/')
        gf.outPath[i] = '.';
    }
    if (gf.decode_only)
      SetFiletype(gf.outPath, 0xfb1); /* Wave */
    else
      SetFiletype(gf.outPath, 0x1ad); /* AMPEG */
#endif
  }


  if (gf.gtkflag) {

#ifdef HAVEGTK
    gtk_init (&argc, &argv);
    gtkcontrol(&gf);
#else
    fprintf(stderr,"Error: lame not compiled with GTK support \n");
#endif

  } else if (gf.decode_only) {

    /* decode an mp3 file to a .wav */
    lame_decoder(&gf,outf,gf.encoder_delay);

  } else {

      lame_id3v2_tag(&gf,outf); /* add ID3 version 2 tag to mp3 file */

      /* encode until we hit eof */
      do {
	/* read in 'iread' samples */
	iread=lame_readframe(&gf,Buffer);


	/* encode */
	imp3=lame_encode_buffer(&gf,Buffer[0],Buffer[1],iread,
              mp3buffer,(int)sizeof(mp3buffer));

	/* was our output buffer big enough? */
	if (imp3<0) {
	  if (imp3==-1) fprintf(stderr,"mp3 buffer is not big enough... \n");
	  else fprintf(stderr,"mp3 internal error:  error code=%i\n",imp3);
	  exit(-1);
	}

	/* imp3 is not negative, but fwrite needs an unsigned here */
	if (fwrite(mp3buffer,1,(unsigned int)imp3,outf) != (size_t)imp3) {
	  fprintf(stderr,"Error writing mp3 output \n");
	  exit(-1);
	}
      } while (iread);
      imp3=lame_encode_finish(&gf,mp3buffer,(int)sizeof(mp3buffer));   /* may return one more mp3 frame */
      if (imp3<0) {
	if (imp3==-1) fprintf(stderr,"mp3 buffer is not big enough... \n");
	else fprintf(stderr,"mp3 internal error:  error code=%i\n",imp3);
	exit(-1);
      }
      /* imp3 is not negative, but fwrite needs an unsigned here */
      fwrite(mp3buffer,1,(unsigned int)imp3,outf);
      lame_mp3_tags_fid(&gf,outf);       /* add ID3 version 1 or VBR tags to mp3 file */
      fclose(outf);
    }
  lame_close_infile(&gf);            /* close the input file */
  return 0;
}
