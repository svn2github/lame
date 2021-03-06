#include "util.h"
#include "id3tag.h"
#include "get_audio.h"
#include "brhist.h"
#include "version.h"




/************************************************************************
*
* license
*
* PURPOSE:  Writes version and license to the file specified by #stdout#
*
************************************************************************/

void lame_print_license(lame_global_flags *gfp,char *name)  /* print version,license & exit */
{
  lame_print_version(stdout);
  fprintf(stdout,"\n");
  fprintf(stdout,"Read the file \"LICENSE\"\n");
  fprintf(stdout,"\n");
  exit(0);
}



/************************************************************************
*
* usage
*
* PURPOSE:  Writes command line syntax to the file specified by #stderr#
*
************************************************************************/

void lame_usage(lame_global_flags *gfp,char *name)  /* print syntax & exit */
{
  lame_print_version(stderr);
  fprintf(stderr,"\n");
  fprintf(stderr,"USAGE   :  %s [options] <infile> [outfile]\n",name);
  fprintf(stderr,"\n<infile> and/or <outfile> can be \"-\", which means stdin/stdout.\n");
  fprintf(stderr,"\n");
  fprintf(stderr,"Try \"%s --help\"     for more information\n",name);
  fprintf(stderr," or \"%s --longhelp\" for a complete options list\n",name);
  exit(1);
}



/************************************************************************
*
* usage
*
* PURPOSE:  Writes command line syntax to the file specified by #stdout#
*           but only the most important ones, to fit on a vt100 terminal
*
************************************************************************/

void lame_short_help(lame_global_flags *gfp,char *name)  /* print syntax & exit */
{
  lame_print_version(stdout); /* prints two lines */
  fprintf(stdout,"\n");
  fprintf(stdout,"USAGE   :  %s [options] <infile> [outfile]\n",name);
  fprintf(stdout,"\n<infile> and/or <outfile> can be \"-\", which means stdin/stdout.\n");
  fprintf(stdout,"\n");
  fprintf(stdout,"OPTIONS :\n");
  fprintf(stdout,"    -b bitrate      set the bitrate, default 128kbps\n");
  fprintf(stdout,"    -f              fast mode (lower quality)\n");
  fprintf(stdout,"    -h              higher quality, but a little slower.  Recommended.\n");
  fprintf(stdout,"    -k              keep ALL frequencies (disables all filters)\n");
  fprintf(stdout,"    -m mode         (s)tereo, (j)oint, (f)orce or (m)ono  (default j)\n");
  fprintf(stdout,"                    force = force ms_stereo on all frames.\n");
  fprintf(stdout,"    -V n            quality setting for VBR.  default n=%i\n",gfp->VBR_q);
  fprintf(stdout,"\n");
  fprintf(stdout,"    --preset type   type must be phone, voice, fm, tape, hifi, cd or studio\n");
  fprintf(stdout,"                    \"--preset help\" gives some more infos on these\n");
  fprintf(stdout,"\n");
  fprintf(stderr,"    --longhelp      full list of options\n");
  fprintf(stdout,"\n");
 
  exit(0);
}

/************************************************************************
*
* usage
*
* PURPOSE:  Writes command line syntax to the file specified by #stdout#
*
************************************************************************/

void lame_help(lame_global_flags *gfp,char *name)  /* print syntax & exit */
{
  lame_print_version(stdout);
  fprintf(stdout,"\n");
  fprintf(stdout,"USAGE   :  %s [options] <infile> [outfile]\n",name);
  fprintf(stdout,"\n<infile> and/or <outfile> can be \"-\", which means stdin/stdout.\n");
  fprintf(stdout,"\n");
  fprintf(stdout,"OPTIONS :\n");
  fprintf(stdout,"  Input options:\n");
  fprintf(stdout,"    -r              input is raw pcm\n");
  fprintf(stdout,"    -x              force byte-swapping of input\n");
  fprintf(stdout,"    -s sfreq        sampling frequency of input file(kHz) - default 44.1kHz\n");
  fprintf(stdout,"    --mp3input      input file is a MP3 file\n");
  fprintf(stdout,"    --ogginput      input file is a Ogg Vorbis file\n");
  fprintf(stdout,"\n");
  fprintf(stdout,"  Operational options:\n");
  fprintf(stdout,"    -m <mode>       (s)tereo, (j)oint, (f)orce or (m)ono  (default j)\n");
  fprintf(stdout,"                    force = force ms_stereo on all frames.\n");
  fprintf(stdout,"    -a              downmix from stereo to mono file for mono encoding\n");
  fprintf(stdout,"    -d              allow channels to have different blocktypes\n");
  fprintf(stdout,"    -S              don't print progress report, VBR histograms\n");
  fprintf(stdout,"    --ogg           encode to Ogg Vorbis instead of MP3\n");
  fprintf(stdout,"    --freeformat    produce a free format bitstream\n");
  fprintf(stdout,"    --decode        input=mp3 file, output=raw pcm\n");
  fprintf(stdout,"    --comp  <arg>   choose bitrate to achive a compression ratio of <arg>\n");
  fprintf(stdout,"    --athonly       only use the ATH for masking\n");
  fprintf(stdout,"    --noath         disable the ATH for masking\n");
  fprintf(stdout,"    --noshort       do not use short blocks\n");
  fprintf(stdout,"    --voice         experimental voice mode\n");
  fprintf(stdout,"    --preset type   type must be phone, voice, fm, tape, hifi, cd or studio\n");
  fprintf(stdout,"                    \"--preset help\" gives some more infos on these\n");
  fprintf(stdout,"\n");
  fprintf(stdout,"  CBR (constant bitrate, the default) options:\n");
  fprintf(stdout,"    -h              higher quality, but a little slower.  Recommended.\n");
  fprintf(stdout,"    -f              fast mode (very low quality)\n");
  fprintf(stdout,"    -b <bitrate>    set the bitrate in kbs, default 128kbps\n");
  fprintf(stdout,"\n");
  fprintf(stdout,"  VBR options:\n");
  fprintf(stdout,"    -v              use variable bitrate (VBR)\n");
  fprintf(stdout,"    -V n            quality setting for VBR.  default n=%i\n",gfp->VBR_q);
  fprintf(stdout,"                    0=high quality,bigger files. 9=smaller files\n");
  fprintf(stdout,"    -b <bitrate>    specify minimum allowed bitrate, default 32kbs\n");
  fprintf(stdout,"    -B <bitrate>    specify maximum allowed bitrate, default 256kbs\n");
  fprintf(stdout,"    -F              strictly enforce the -b option, for use with players that\n");
  fprintf(stdout,"                    do not support low bitrate mp3 (Apex AD600-A DVD/mp3 player)\n");
  fprintf(stdout,"    -t              disable Xing VBR informational tag\n");
  fprintf(stdout,"    --nohist        disable VBR histogram display\n");
  fprintf(stdout,"\n");
  fprintf(stdout,"    --abr <bitrate> specify average bitrate desired (instead of quality)\n");
  fprintf(stdout,"\n");
  fprintf(stdout,"  MP3 header/stream options:\n");
  fprintf(stdout,"    -e <emp>        de-emphasis n/5/c  (obsolete)\n");
  fprintf(stdout,"    -c              mark as copyright\n");
  fprintf(stdout,"    -o              mark as non-original\n");
  fprintf(stdout,"    -p              error protection.  adds 16bit checksum to every frame\n");
  fprintf(stdout,"                    (the checksum is computed correctly)\n");
  fprintf(stdout,"    --nores         disable the bit reservoir\n");
  fprintf(stdout,"    --strictly-enforce-ISO   comply as much as possible to ISO MPEG spec\n");
  fprintf(stdout,"\n");
  fprintf(stdout,"  Filter options:\n");
  fprintf(stdout,"    -k              keep ALL frequencies (disables all filters)\n");
  fprintf(stdout,"  --lowpass <freq>        frequency(kHz), lowpass filter cutoff above freq\n");
  fprintf(stdout,"  --lowpass-width <freq>  frequency(kHz) - default 15%% of lowpass freq\n");
  fprintf(stdout,"  --highpass <freq>       frequency(kHz), highpass filter cutoff below freq\n");
  fprintf(stdout,"  --highpass-width <freq> frequency(kHz) - default 15%% of highpass freq\n");
  fprintf(stdout,"  --resample <sfreq>  sampling frequency of output file(kHz)- default=automatic\n");
  fprintf(stdout,"  --cwlimit <freq>    compute tonality up to freq (in kHz) default 8.8717\n");
  fprintf(stdout,"\n");
  fprintf(stdout,"  Specifying any of the following options will add an ID3 tag:\n");
  fprintf(stdout,"     --tt \"title\"     title of song (max 30 chars)\n");
  fprintf(stdout,"     --ta \"artist\"    artist who did the song (max 30 chars)\n");
  fprintf(stdout,"     --tl \"album\"     album where it came from (max 30 chars)\n");
  fprintf(stdout,"     --ty \"year\"      year in which the song/album was made (max 4 chars)\n");
  fprintf(stdout,"     --tc \"comment\"   additional info (max 30 chars)\n");
  fprintf(stdout,"                      (or max 28 chars if using the \"track\" option)\n");
  fprintf(stdout,"     --tn \"track\"     track number of the song on the CD (1 to 99)\n");
  fprintf(stdout,"                      (using this option will add an ID3v1.1 tag)\n");
  fprintf(stdout,"     --tg \"genre\"     genre of song (name or number)\n");
  fprintf(stdout,"\n");
#ifdef HAVEGTK
  fprintf(stdout,"    -g              run graphical analysis on <infile>\n");
#endif
  display_bitrates(stdout);
  exit(0);
}



/************************************************************************
*
* usage
*
* PURPOSE:  Writes presetting info to #stdout#
*
************************************************************************/

void lame_presets_info(lame_global_flags *gfp,char *name)  /* print syntax & exit */
{
  lame_print_version(stdout);
  fprintf(stdout,"\n");
  fprintf(stdout,"Presets are some shortcuts for common settings.\n");
  fprintf(stdout,"They can be combined with -v if you want VBR MP3s.\n");
  fprintf(stdout,"\n");
  fprintf(stdout,"                 phone   sw     am    fm  voice radio  tape  hifi   cd studio\n");
  fprintf(stdout,"=============================================================================\n");
  fprintf(stdout,"--resample        8.0  11.025 16.0  22.05 32.0                               \n");
  fprintf(stdout,"--lowpass         3.2   4.800  7.2   9.95 12.3  15.0  18.5  20.24            \n");
  fprintf(stdout,"--lowpass-width   1.0   0.500  0.5   0.88  2.0   0.0   2.0   2.20            \n");
  fprintf(stdout,"--noshort         yes                      yes                               \n");
  fprintf(stdout,"-h                                                           yes   yes   yes \n");
  fprintf(stdout,"-m                  m     m      m     j     m     j     j     j     s     s \n");
  fprintf(stdout,"-b                 16    24     32    64    56   112   128   160   192   256 \n");
  fprintf(stdout,"-- PLUS WITH -v -------------------------------------------------------------\n");
  fprintf(stdout,"-V                  6     5      5     5     4     4     4     3     2     0 \n");
  fprintf(stdout,"-b                  8     8     16    24    32    64    96   112   128   160 \n");
  fprintf(stdout,"-B                 56    64    128   160   128   256   320   320   320   320 \n");
  fprintf(stdout,"\n");
  fprintf(stdout,"EXAMPLES:\n");
  fprintf(stdout," a) --preset fm\n");
  fprintf(stdout,"    equals: --resample 22.05 --lowpass 9.95 --lowpass-width 0.88 -mj -b 64\n");
  fprintf(stdout," b) -v --preset studio\n");
  fprintf(stdout,"    equals: -h -m s -V 0 -b 160 -B 320\n");
  fprintf(stdout,"\n");

  exit(0);
}



/************************************************************************
*
* parse_args
*
* PURPOSE:  Sets encoding parameters to the specifications of the
* command line.  Default settings are used for parameters
* not specified in the command line.
*
* If the input file is in WAVE or AIFF format, the sampling frequency is read
* from the AIFF header.
*
* The input and output filenames are read into #inpath# and #outpath#.
*
************************************************************************/
void lame_parse_args(lame_global_flags *gfp,int argc, char **argv)
{
  FLOAT srate;
  int   err = 0, i = 0;
  int autoconvert=0;
  int user_quality=0;
  char *programName = argv[0]; 
  int track = 0;

  gfp->inPath[0] = '\0';   
  gfp->outPath[0] = '\0';
  /* turn on display options. user settings may turn them off below */
  gfp->silent=0;
  gfp->brhist_disp = 1;
  id3_inittag(&gfp->id3tag);

  /* process args */
  while(++i<argc && err == 0) {
    char c, *token, *arg, *nextArg;
    int  argUsed;
    
    token = argv[i];
    if(*token++ == '-') {
      if(i+1 < argc) nextArg = argv[i+1];
      else           nextArg = "";
      argUsed = 0;
      if (! *token) {
	/* The user wants to use stdin and/or stdout. */
	if(gfp->inPath[0] == '\0')       strncpy(gfp->inPath, argv[i],MAX_NAME_SIZE);
	else if(gfp->outPath[0] == '\0') strncpy(gfp->outPath, argv[i],MAX_NAME_SIZE);
      } 
      if (*token == '-') {
	/* GNU style */
	token++;

	if (strcmp(token, "resample")==0) {
	  argUsed=1;
	  srate = atof( nextArg );
	  /* samplerate = rint( 1000.0 * srate ); $A  */
	  gfp->out_samplerate =  (( 1000.0 * srate ) + 0.5);
	  if (srate  < 1) {
	    fprintf(stderr,"Must specify a samplerate with --resample\n");
	    exit(1);
	  }
	}
	else if (strcmp(token, "abr")==0) {
	  argUsed=1;
	  gfp->VBR = 3; 
	  gfp->VBR_mean_bitrate_kbps = atoi(nextArg);
	  gfp->VBR_mean_bitrate_kbps = Min(gfp->VBR_mean_bitrate_kbps,310); 
	  gfp->VBR_mean_bitrate_kbps = Max(gfp->VBR_mean_bitrate_kbps,4); 
	}
	else if (strcmp(token, "mp3input")==0) {
	  gfp->input_format=sf_mp3;
	}
	else if (strcmp(token, "ogginput")==0) {
	  gfp->input_format=sf_ogg;
	}
	else if (strcmp(token, "ogg")==0) {
	  gfp->ogg=1;
	}
	else if (strcmp(token, "voice")==0) {
	  gfp->lowpassfreq=12000;
	  gfp->VBR_max_bitrate_kbps=160;
	  gfp->no_short_blocks=1;
	}
	else if (strcmp(token, "noshort")==0) {
	  gfp->no_short_blocks=1;
	}
	else if (strcmp(token, "decode")==0) {
	  gfp->decode_only=1;
	}
	else if (strcmp(token, "noath")==0) {
	  gfp->noATH=1;
	}
	else if (strcmp(token, "nores")==0) {
	  gfp->disable_reservoir=1;
	  gfp->padding_type=0;
	}
	else if (strcmp(token, "strictly-enforce-ISO")==0) {
	  gfp->strict_ISO=1;
	}
	else if (strcmp(token, "athonly")==0) {
	  gfp->ATHonly=1;
	}
	else if (strcmp(token, "freeformat")==0) {
	  gfp->free_format=1;
	}
	else if (strcmp(token, "athshort")==0) {
	  gfp->ATHshort=1;
	}
	else if (strcmp(token, "nohist")==0) {
	  gfp->brhist_disp = 0;
	}
	/* options for ID3 tag */
 	else if (strcmp(token, "tt")==0) {
 		gfp->id3tag_used=1;      argUsed = 1;
  		strncpy(gfp->id3tag.title, nextArg, 30);
 		}
 	else if (strcmp(token, "ta")==0) {
 		gfp->id3tag_used=1; argUsed = 1;
  		strncpy(gfp->id3tag.artist, nextArg, 30);
 		}
 	else if (strcmp(token, "tl")==0) {
 		gfp->id3tag_used=1; argUsed = 1;
  		strncpy(gfp->id3tag.album, nextArg, 30);
 		}
 	else if (strcmp(token, "ty")==0) {
 		gfp->id3tag_used=1; argUsed = 1;
  		strncpy(gfp->id3tag.year, nextArg, 4);
 		}
 	else if (strcmp(token, "tc")==0) {
 		gfp->id3tag_used=1; argUsed = 1;
  		strncpy(gfp->id3tag.comment, nextArg, 30);
 		}
 	else if (strcmp(token, "tn")==0) {
 		gfp->id3tag_used=1; argUsed = 1;
  		track = atoi(nextArg);
  		if (track < 1) { track = 1; }
  		if (track > 99) { track = 99; }
  		gfp->id3tag.track = track;
 		}
 	else if (strcmp(token, "tg")==0) {
		argUsed = strtol (nextArg, &token, 10);
		if (nextArg==token) {
		  /* Genere was given as a string, so it's number*/
		  for (argUsed=0; argUsed<=genre_last; argUsed++) {
		    if (!strcmp (genre_list[argUsed], nextArg)) { break; }
		  }
 		}
		if (argUsed>genre_last) { 
		  argUsed=255; 
		  fprintf(stderr,"Unknown genre: %s.  Specifiy genre number \n", nextArg);
		}
	        argUsed &= 255; c=(char)(argUsed);

 		gfp->id3tag_used=1; argUsed = 1;
  		strncpy(gfp->id3tag.genre, &c, 1);
	       }
	else if (strcmp(token, "lowpass")==0) {
	  argUsed=1;
	  gfp->lowpassfreq =  (( 1000.0 * atof( nextArg ) ) + 0.5);
	  if (gfp->lowpassfreq  < 1) {
	    fprintf(stderr,"Must specify lowpass with --lowpass freq, freq >= 0.001 kHz\n");
	    exit(1);
	  }
	}
	else if (strcmp(token, "lowpass-width")==0) {
	  argUsed=1;
	  gfp->lowpasswidth =  (( 1000.0 * atof( nextArg ) ) + 0.5);
	  if (gfp->lowpasswidth  < 0) {
	    fprintf(stderr,"Must specify lowpass width with --lowpass-width freq, freq >= 0 kHz\n");
	    exit(1);
	  }
	}
	else if (strcmp(token, "highpass")==0) {
	  argUsed=1;
	  gfp->highpassfreq =  (( 1000.0 * atof( nextArg ) ) + 0.5);
	  if (gfp->highpassfreq  < 1) {
	    fprintf(stderr,"Must specify highpass with --highpass freq, freq >= 0.001 kHz\n");
	    exit(1);
	  }
	}
	else if (strcmp(token, "highpass-width")==0) {
	  argUsed=1;
	  gfp->highpasswidth =  (( 1000.0 * atof( nextArg ) ) + 0.5);
	  if (gfp->highpasswidth  < 0) {
	    fprintf(stderr,"Must specify highpass width with --highpass-width freq, freq >= 0 kHz\n");
	    exit(1);
	  }
	}
	else if (strcmp(token, "cwlimit")==0) {
	  argUsed=1;
	  gfp->cwlimit =  atof( nextArg );
	  if (gfp->cwlimit <= 0 ) {
	    fprintf(stderr,"Must specify cwlimit in kHz\n");
	    exit(1);
	  }
	} 
	else if (strcmp(token, "comp")==0) {
	  argUsed=1;
	  gfp->compression_ratio =  atof( nextArg );
	  if (gfp->compression_ratio < 1.0 ) {
	    fprintf(stderr,"Must specify compression ratio >= 1.0\n");
	    exit(1);
	  }
	} /* some more GNU-ish options could be added
	   * version       => complete name, version and license info (normal exit)  
	   * quiet/silent  => no messages on screen
	   * brief         => few messages on screen (name, status report)
	   * verbose       => all infos to screen (brhist, internal flags/filters)
	   * o/output file => specifies output filename
	   * O             => stdout
	   * i/input file  => specifies input filename
	   * I             => stdin
	   */
	else if (strcmp(token, "version") ==0
	       ||strcmp(token, "license")==0){
	  lame_print_license(gfp,programName);  /* doesn't return */
	}
	else if (strcmp(token, "help") ==0
	       ||strcmp(token, "usage")==0){
	  lame_short_help(gfp,programName);  /* doesn't return */
	}
	else if (strcmp(token, "longhelp") ==0){
	  lame_help(gfp,programName);  /* doesn't return */
	}
	else if (strcmp(token, "preset")==0) {
	  argUsed=1;
	  if (strcmp(nextArg,"phone")==0)
	  { /* when making changes, please update help text too */
	    gfp->out_samplerate =  8000;
	    gfp->lowpassfreq=3200;
	    gfp->lowpasswidth=1000;
	    gfp->no_short_blocks=1;
	    gfp->quality = 5;
	    gfp->mode = MPG_MD_MONO; 
	    gfp->mode_fixed = 1; 
	    gfp->brate = 16; 
	    gfp->VBR_q=6;
	    gfp->VBR_min_bitrate_kbps=8;
	    gfp->VBR_max_bitrate_kbps=56;
	  }
	  else if (strcmp(nextArg,"sw")==0)
	  { /* when making changes, please update help text too */
	    gfp->out_samplerate =  11025;
	    gfp->lowpassfreq=4800;
	    gfp->lowpasswidth=500;
	    gfp->quality = 5;
	    gfp->mode = MPG_MD_MONO; 
	    gfp->mode_fixed = 1; 
	    gfp->brate = 24; 
	    gfp->VBR_q=5;
	    gfp->VBR_min_bitrate_kbps=8;
	    gfp->VBR_max_bitrate_kbps=64;
	  }
	  else if (strcmp(nextArg,"am")==0)
	  { /* when making changes, please update help text too */
	    gfp->out_samplerate =  16000;
	    gfp->lowpassfreq=7200;
	    gfp->lowpasswidth=500;
	    gfp->quality = 5;
	    gfp->mode = MPG_MD_MONO; 
	    gfp->mode_fixed = 1; 
	    gfp->brate = 32; 
	    gfp->VBR_q=5;
	    gfp->VBR_min_bitrate_kbps=16;
	    gfp->VBR_max_bitrate_kbps=128;
	  }
	  else if (strcmp(nextArg,"fm")==0)
	  { /* when making changes, please update help text too */
	    gfp->out_samplerate =  22050; 
            gfp->lowpassfreq=9950;
            gfp->lowpasswidth=880;
	    gfp->quality = 5;
	    gfp->mode = MPG_MD_JOINT_STEREO; 
	    gfp->mode_fixed = 1; 
	    gfp->brate = 64; 
	    gfp->VBR_q=5;
	    gfp->VBR_min_bitrate_kbps=24;
	    gfp->VBR_max_bitrate_kbps=160;
	  }
	  else if (strcmp(nextArg,"voice")==0)
	  { /* when making changes, please update help text too */
	    gfp->out_samplerate =  32000; 
	    gfp->lowpassfreq=12300;
	    gfp->lowpasswidth=2000;
	    gfp->no_short_blocks=1;
	    gfp->quality = 5;
	    gfp->mode = MPG_MD_MONO; 
	    gfp->mode_fixed = 1; 
	    gfp->brate = 56; 
	    gfp->VBR_q=4;
	    gfp->VBR_min_bitrate_kbps=32;
	    gfp->VBR_max_bitrate_kbps=128;
	  }
	  else if (strcmp(nextArg,"radio")==0)
	  { /* when making changes, please update help text too */
            gfp->lowpassfreq=15000;
            gfp->lowpasswidth=0;
	    gfp->quality = 5;
	    gfp->mode = MPG_MD_JOINT_STEREO; 
	    gfp->mode_fixed = 1; 
	    gfp->brate = 112; 
	    gfp->VBR_q=4;
	    gfp->VBR_min_bitrate_kbps=64;
	    gfp->VBR_max_bitrate_kbps=256;
	  }
	  else if (strcmp(nextArg,"tape")==0)
	  { /* when making changes, please update help text too */
            gfp->lowpassfreq=18500;
            gfp->lowpasswidth=2000;
	    gfp->quality = 5;
	    gfp->mode = MPG_MD_JOINT_STEREO; 
	    gfp->mode_fixed = 1; 
	    gfp->brate = 128; 
	    gfp->VBR_q=4;
	    gfp->VBR_min_bitrate_kbps=96;
	    gfp->VBR_max_bitrate_kbps=320;
	  }
	  else if (strcmp(nextArg,"hifi")==0)
	  { /* when making changes, please update help text too */
            gfp->lowpassfreq=20240;
            gfp->lowpasswidth=2200;
	    gfp->quality = 2;
	    gfp->mode = MPG_MD_JOINT_STEREO; 
	    gfp->mode_fixed = 1; 
	    gfp->brate = 160;            
	    gfp->VBR_q=3;
	    gfp->VBR_min_bitrate_kbps=112;
	    gfp->VBR_max_bitrate_kbps=320;
	  }
	  else if (strcmp(nextArg,"cd")==0)
	  { /* when making changes, please update help text too */
	    gfp->lowpassfreq=-1;
            gfp->highpassfreq=-1;
	    gfp->quality = 2;
	    gfp->mode = MPG_MD_STEREO; 
	    gfp->mode_fixed = 1; 
	    gfp->brate = 192;  
	    gfp->VBR_q=2;
	    gfp->VBR_min_bitrate_kbps=128;
	    gfp->VBR_max_bitrate_kbps=320;
	  }
	  else if (strcmp(nextArg,"studio")==0)
	  { /* when making changes, please update help text too */
	    gfp->lowpassfreq=-1;
            gfp->highpassfreq=-1;
	    gfp->quality = 2; 
	    gfp->mode = MPG_MD_STEREO; 
	    gfp->mode_fixed = 1; 
	    gfp->brate = 256; 
	    gfp->VBR_q=0;
	    gfp->VBR_min_bitrate_kbps=160;
	    gfp->VBR_max_bitrate_kbps=320;
	  }
	  else if (strcmp(nextArg,"help")==0)
	  {
	    lame_presets_info(gfp,programName);  /* doesn't return */
	  }
	  else
	    {
	      fprintf(stderr,"%s: --preset type, type must be phone, voice, fm, tape, hifi, cd or studio, not %s\n",
		      programName, nextArg);
	      exit(1);
	    }
	} /* --preset */
	else
	  {
	    fprintf(stderr,"%s: unrec option --%s\n",
		    programName, token);
	  }
	i += argUsed;
	
      } else  while( (c = *token++) ) {
	if(*token ) arg = token;
	else                             arg = nextArg;
	switch(c) {
	case 'm':        argUsed = 1;   gfp->mode_fixed = 1;
	  if (*arg == 's')
	    { gfp->mode = MPG_MD_STEREO; }
	  else if (*arg == 'd')
	    { gfp->mode = MPG_MD_DUAL_CHANNEL; }
	  else if (*arg == 'j')
	    { gfp->mode = MPG_MD_JOINT_STEREO; }
	  else if (*arg == 'f')
	    { gfp->mode = MPG_MD_JOINT_STEREO; gfp->force_ms=1; }
	  else if (*arg == 'm')
	    { gfp->mode = MPG_MD_MONO; }
	  else {
	    fprintf(stderr,"%s: -m mode must be s/d/j/f/m not %s\n",
		    programName, arg);
	    err = 1;
	  }
	  break;
	case 'V':        argUsed = 1;   gfp->VBR = 1;  
	  gfp->VBR_q = atoi(arg);
	  if (gfp->VBR_q <0) gfp->VBR_q=0;
	  if (gfp->VBR_q >9) gfp->VBR_q=9;
	  gfp->quality = 2;
	  break;
	case 'q':        argUsed = 1; 
	  user_quality = atoi(arg);
	  if (user_quality<0) user_quality=0;
	  if (user_quality>9) user_quality=9;
	  break;
	case 's':
	  argUsed = 1;
	  srate = atof( arg );
	  /* samplerate = rint( 1000.0 * srate ); $A  */
	  gfp->in_samplerate =  (( 1000.0 * srate ) + 0.5);
	  break;
	case 'b':        
	  argUsed = 1;
	  gfp->brate = atoi(arg); 
	  gfp->VBR_min_bitrate_kbps=gfp->brate;
	  break;
	case 'B':        
	  argUsed = 1;
	  gfp->VBR_max_bitrate_kbps=atoi(arg); 
	  break;	
	case 'F':        
	  gfp->VBR_hard_min=1;
	  break;	
	case 't':  /* dont write VBR tag */
	  gfp->bWriteVbrTag=0;
	  break;
	case 'r':  /* force raw pcm input file */
#ifdef LIBSNDFILE
	  fprintf(stderr,"WARNING: libsndfile may ignore -r and perform fseek's on the input.\n");
	  fprintf(stderr,"Compile without libsndfile if this is a problem.\n");
#endif
	  gfp->input_format=sf_raw;
	  break;
	case 'x':  /* force byte swapping */
	  gfp->swapbytes=TRUE;
	  break;
	case 'p': /* (jo) error_protection: add crc16 information to stream */
	  gfp->error_protection = 1; 
	  break;
	case 'a': /* autoconvert input file from stereo to mono - for mono mp3 encoding */
	  autoconvert=1;
	  gfp->mode=MPG_MD_MONO;
	  gfp->mode_fixed=1;
	  break;
	case 'h': 
	  gfp->quality = 2;
	  break;
	case 'k': 
	  gfp->lowpassfreq=-1;
	  gfp->highpassfreq=-1;
	  break;
	case 'd': 
	  gfp->allow_diff_short = 1;
	  break;
	case 'v': 
	  gfp->VBR = 1; 
	  gfp->quality = 2;
	  break;
	case 'S': 
	  gfp->silent = TRUE;
	  break;
	case 'X':        argUsed = 1;   
	  gfp->VBR_max_bitrate_kbps=atoi(arg); 
	  break;


	case 'Y': 
	  gfp->experimentalY = TRUE;
	  break;
	case 'Z': 
	  gfp->experimentalZ = TRUE;
	  break;
	case 'f': 
	  gfp->quality= 7;
	  break;
	case 'g': /* turn on gtk analysis */
	  gfp->gtkflag = TRUE;
	  break;

	case 'e':        argUsed = 1;
	  if (*arg == 'n')                    gfp->emphasis = 0;
	  else if (*arg == '5')               gfp->emphasis = 1;
	  else if (*arg == 'c')               gfp->emphasis = 3;
	  else {
	    fprintf(stderr,"%s: -e emp must be n/5/c not %s\n",
		    programName, arg);
	    err = 1;
	  }
	  break;
	case 'c':   gfp->copyright = 1; break;
	case 'o':   gfp->original  = 0; break;
	
	case '?':   lame_help(gfp,programName);  /* doesn't return */
	default:    fprintf(stderr,"%s: unrec option %c\n",
				programName, c);
	err = 1; break;
	}
	if(argUsed) {
	  if(arg == token)    token = "";   /* no more from token */
	  else                ++i;          /* skip arg we used */
	  arg = ""; argUsed = 0;
	}
      }
    } else {
      if(gfp->inPath[0] == '\0')       strncpy(gfp->inPath, argv[i], MAX_NAME_SIZE);
      else if(gfp->outPath[0] == '\0') strncpy(gfp->outPath, argv[i], MAX_NAME_SIZE);
      else {
	fprintf(stderr,"%s: excess arg %s\n", programName, argv[i]);
	err = 1;
      }
    }
  }  /* loop over args */

  if(err || gfp->inPath[0] == '\0') lame_usage(gfp,programName);  /* never returns */
  if (gfp->inPath[0]=='-') gfp->silent=1;  /* turn off status - it's broken for stdin */
  if(gfp->outPath[0] == '\0') {
    if (gfp->inPath[0]=='-') {
      /* if input is stdin, default output is stdout */
      strcpy(gfp->outPath,"-");
    }else {
      strncpy(gfp->outPath, gfp->inPath, MAX_NAME_SIZE - 4);
      if (gfp->decode_only) {
	strncat(gfp->outPath, ".wav", 4 );
      } else if (gfp->ogg) {
	strncat(gfp->outPath, ".ogg", 4 );
      }else{
	strncat(gfp->outPath, ".mp3", 4 );
      }
    }
  }
  /* some file options not allowed with stdout */
  if (gfp->outPath[0]=='-') {
    gfp->bWriteVbrTag=0; /* turn off VBR tag */
    if (gfp->id3tag_used) {
      gfp->id3tag_used=0;         /* turn of id3 tagging */
      fprintf(stderr,"id3tag ignored: id3 tagging not supported for stdout.\n");
    }
  }


  /* if user did not explicitly specify input is mp3, check file name */
  if (gfp->input_format != sf_mp3 || gfp->input_format != sf_ogg) {
    if (!(strcmp((char *) &gfp->inPath[strlen(gfp->inPath)-4],".mp3")))
      gfp->input_format = sf_mp3;
    else if (!(strcmp((char *) &gfp->inPath[strlen(gfp->inPath)-4],".ogg")))
      gfp->input_format = sf_ogg;
  }
  
#if !(defined HAVEMPGLIB || defined AMIGA_MPEGA)
  if (gfp->input_format == sf_mp3) {
    fprintf(stderr,"Error: libmp3lame not compiled with mp3 *decoding* support \n");
    exit(1);
  }
#endif
  /* default guess for number of channels */
  if (autoconvert) gfp->num_channels=2; 
  else if (gfp->mode == MPG_MD_MONO) gfp->num_channels=1;
  else gfp->num_channels=2;

  /* user specified a quality value.  override any defaults set above */
  if (user_quality)   gfp->quality=user_quality;

  if (gfp->free_format) {
    if (gfp->brate<8) {
      fprintf(stderr,"For free format, specify a bitrate between 8 and 320kbs\n");
      exit(1);
    }
  }

}



