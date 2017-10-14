#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hip.h"

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif


int main(int argc, char **argv){
  HIP_File hf;
  int eof=0;
  int current_section;
  char pcmout[8000];

#ifdef _WIN32
  _setmode( _fileno( stdin ), _O_BINARY );
  _setmode( _fileno( stdout ), _O_BINARY );
#endif

  if(hip_open(stdin, &hf, NULL, 0) < 0) {
      fprintf(stderr,"Input does not appear to be an mpeg bitstream.\n");
      exit(1);
  }

  while(!eof){
    long ret=hip_read(&hf,pcmout,sizeof(pcmout),0,2,1,&current_section);
    if (ret == 0) {
      /* EOF */
      eof=1;
    } else if (ret < 0) {
      /* error in the stream.  Not a problem, just reporting it in
         case we (the app) cares.  In this case, we notify since this
         is a test application.
      */
      fprintf(stderr,"Error in the stream %ld\n", ret);
    } else {
      fwrite(pcmout,1,ret,stdout);
    }
  }

  hip_clear(&hf);
    
  fprintf(stderr,"Done.\n");
  return(0);
}

