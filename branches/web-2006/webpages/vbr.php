<!DOCTYPE html
     PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
	<title>LAME MP3 Encoder</title>
	<meta name="author" content="Roberto Amorim - roberto@rjamorim.com" />
	<meta name="generator" content="jEdit 4.2" />
	<meta http-equiv="Content-Style-Type" content="text/css" />
	<meta http-equiv="content-type" content="text/html; charset=UTF-8" />
	<link rel="stylesheet" type="text/css" href="./styles/lame.css" />
    <!--[if IE]>
      <link rel="stylesheet" type="text/css" href="./styles/ie.css" />
    <![endif]-->
</head>
<body>
	<?php include("gpsycho.html") ?>
	<div id="container">
		<div id="content">
			<div align="center"><img src="./images/logo.gif" width="358" height="231" alt="LAME Official logo"></img>
			<h1>GPSYCHO - Variable Bit Rate</h1></div>
			<hr></hr>
<pre>Suggested usage: 

lame -v -V 2 -b 128  input.wav output.mp3 
  

VBR mode automatically uses the highest quality option.  So both 
"-v" and "-h" are not necessary when using -V. 

Options: 

-V n   where n=0..9.   0 = highest quality 
                       9 = lowest quality 

-b &lt;minimum allowed bitrate> 
-B &lt;maximum allowed bitrate> 

Using -B with other than 320kbs is not recommended, since even a 
128kbs CBR stream will sometimes use frames as large as 320kbs 
via the bitreservoir.</pre>
				<hr></hr>
<pre>Variables used in VBR code description: 
  

sfb:             scale factor band index. 
thm[sfb]:    Allowed masking.  thm[sfb] = How much noise is allowed in the sfb'th band, 
                   as computed by the GPSYCHO. 

gain[sfb]:    MDCT coefficents are scaled by 2^(-.25*gain) before quantizing. 
                   Smaller values of gain (more negative) mean that more bits are required 
                   to encode the coefficients, but the quantization noise will be (usually) smaller. 

desired_gain[sfb]:       The amount of gain needed so that if gain[sfb] &lt;= desired_gain[sfb], 
                                      the quantization noise will be &lt;= thm[sfb]. 
  

An MP3 can use the following variables  to achieve a given gain[sfb]: 

For longblocks: 

   gain[sfb][i] =   2^  [ -.25 * ( global_gain -210 - ifqstep*scalefac[gr][ch].l[sfb] -  ifqstep*pretab[sfb]) ] 

For shortblocks:  (i=0..2 for the three short blocks) 

    gain[sfb][i] =   2^  [ -.25*(  global_gain -210  -  8*subblock_gain[i]  - ifqstep*scalefac.s[sfb][i])   ] 
  

ifqstep =  scalefac_scale==0 ?  2 : 4;</pre>
				<hr></hr>
<pre>VBR Algorithm: 
  
  
step 1:    psymodel.c: 

           Computes the allowed maskings, thm[sfb] 
           thm[sfb] may be reduced by a few db depending on the quality setting. 
           The smaller thm[sfb], the more bits will be required to encode the 
           frame. 
  

step 2:    find_scalefac() in vbrquantize.c: 

           Compute desired_gain[sfb]: 

           for (sfb=0, sfb &lt; SBMAX, ++sfb) 
              compute desired_gain[sfb] using a divide and conquer iteration 
              so that quantization_noise[sfb] &lt; thm[sfb] 

              This requires 7 iterations of calc_sfb_noise() which computes 
              quantization error for the specified gain.  This is the only time 
              VBR need to do any (expensive) quantization! 
  

step 3:    VBR_noise_shaping() in vbrquantize.c: 

           Find a combination of global_gain, subblock_gain, preflag, 
           scalefac_scale, etc... so that:  gain[sfb] &lt;= desired_gain[sfb] 
  

step 4:    VBR_quantize_granule()  in vbrquantize.c 

           Calculate the number of bits needed to encode the frame with 
           the values computed in step 3.  Unlike CBR, VBR (usually) only 
           has to do this expensive huffman bit counting stuff once! 
  

step 5:    VBR_noise_shaping() in vbrquantize.c: 

           if bits &lt; minimum_bits:  Repeat step 3, only with a larger value of 
           global_gain.  (but allow bits &lt; minimum_bits for analog silence) 

           if bits > maximum_bits:  decrease global_gain, keeping all other 
           scalefactors the same. 

           Usually step 5 is not necessary. 
  
  
step 6:    VBR_quantize() in vbrquantize.c 

           After encoding both channels and granules, check to make sure 
           that the total number of bits for the whole frame does not 
           exceed the maximum allowed. If it does, lower the quality 
           and repeat steps 2,3 and 4 for the granules that were using 
           lots of bits.</pre>
				<hr></hr>
<pre>The actual flow chart looks something like this: 
  

VBR_quantize() 

   determine minbits, maxbits for each granule 
   determine max_frame_bits 
   adjust global quality setting based on VBR_q 

   do 
      frame_bits=0 

      loop over each channel, granule: 
          compute thm[sfb] 
          bits=VBR_noise_shaping():  Encodes each granule with minbits &lt;= bits &lt;= maxbits 
          frame_bits += bits 

      lower the global quality setting 

   while (frame_bits > max_frame_bits) 
    

VBR_noise_shaping(): 

    find_scalefac()   (computes desired_gain) 
    Estimate largest possible value of global_gain 

    do 
        compute_scalefac_long/short() 
        scalefacts, etc. so that gain &lt;= desired_gain) 

        bits = VBR_quantize_granule() 

        if (bits &lt; minbits &amp;&amp; analog silence) break; 
        if (bits >= minbits) break; 

        decrease global_gain (which increases number of bits used) 

    while 1 

    if bits > maxbits 
       do 
           increase global_gain 
           bits = VBR_quantize_granule() 
       while (bits > maxbits) 
  
 
find_scalefac() 

      Simple divide and conquer iteration which repeatidly 
      calls calc_sfb_noise() with different values of desired_gain 
      until it finds the largest desired_gain such that the 
      quantization_noise &lt; allowed masking 

      Requires 7 iterations.</pre>
		</div>
	<?php include("footer.html") ?>
	</div>
</body>
</html>