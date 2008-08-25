<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<title>LAME MP3 Encoder :: GPSYCHO - Mid/Side Stereo</title>
	<meta name="author" content="Roberto Amorim - roberto@rjamorim.com" />
	<meta name="generator" content="jEdit 4.2" />
	<meta name="cvs-version" content="$Id: ms_stereo.php,v 1.4 2008-08-25 02:24:41 rjamorim Exp $" />
	<meta http-equiv="content-type" content="text/html; charset=UTF-8" />
	<link rel="stylesheet" type="text/css" href="styles/lame.css" />
	<!--[if IE]>
	<link rel="stylesheet" type="text/css" href="styles/ie.css" />
	<![endif]-->
</head>
<body>

<?php include("menu.html") ?>

<div id="container">
<div id="content">

<div align="center">
	<img src="images/logo.gif" width="358" height="231" alt="LAME Official Logo" />
	<h1>GPSYCHO - Mid/Side Stereo</h1>
</div>

<p>
	There are two stereo modes in GPsycho: "stereo" and "jstereo". "stereo" is
	just the normal independent coding of left &amp; right channels. "jstereo" 
	means the frames may use normal stereo, stereo, mid/side stereo or intensity 
	stereo. FhG only seems to use intensity stereo at bitrates of 80kbs and 
	lower. LAME does not have intensity stereo capability. In jstereo mode, the 
	encoder has to decide for each frame if it should be encoded stereo or 
	mid/side stereo.
</p>

<p>
	Mid/side stereo encodes the mid and side channels instead of left and right.
	It allocates more bits to the mid channel than the side channel. For signals
	without a lot of stereo seperation, there will be very little information in 
	the side channel and this trick will improve bandwidth. If the left &amp; 
	right channels differ by a lot, then the side channel will contain a lot of
	information. Errors encoding this information will show up as noise in
	<em>both</em> the left and right channels after decoding.
</p>

<p>
	The LAME mid/side switching criterion, and mid/side masking thresholds are
	taken from <cite>Johnston and Ferreira, <u>Sum-Difference Stereo Transform
	Coding,</u> Proc. IEEE ICASSP (1992) p 569-571.</cite>
</p>

<p>
	The MPEG AAC standard claims to use mid/side encoding based on this paper.
</p>

<h3>LAME Mid/Side switching criterion</h3>

<p>
	The new ms_stereo switch uses mid/side stereo only when the difference
	between L &amp; R masking thresholds (averaged over all scalefactors) is less
	then 5db. In several test samples it does an amazing job mimicking the FhG
	encoder (see below).
</p>

<p>
	I believe the idea behind this is the following: If one channel has much less
	noise masking in a certain band, than masked noise in one channel that is spread
	to the other channel (by mid/side stereo) may no longer be masked. If both
	channels have the same masking, then the noise spread between both channels will
	be equally well masked.
</p>

<pre>regular stereo frames:
Fools.wav:  (1180 frames)
FhG                             frames 793-804,902
new LAME                        frames 793-803,869,902,966,1017
old LAME                        over 500 frames used regular stereo

IfYouCould.wav: (80 frames)
FhG                             43,51,60
new LAME                        42,43,51,60       (like FhG, 1 extra)
old LAME                        33,62,65,66       (completely unlike FhG)

mstest.wav: (156 frames)
FhG:                            138 frames use regular stereo
new LAME                        137 frames use regular stereo
old LAME                          8 frames use regular stereo

t1.wav: (160 frames)
FhG:                            39-42, 80-83, 121-124, 144-150
new LAME:                       38-41, 79-82, 120-124,
old LAME:                       constant inappropriate toggling of ms_stereo

track7.wav (146 frames)
FhG:                            0, 2-15, 21-66, 69-80, 83-146
new LAME:

Castanets.wav:  (253 frames)
All encoders use all ms_stereo for all frames

else3.wav: 217 frames
All encoders use all ms_stereo for all frames</pre>

<h3>Mid/Side Stereo Masking Thresholds</h3>

<p>
	There is a problem for true jstereo, where you need to turn ms_stereo on and
	off on a frame by frame basis. Some frames will need masking thresholds from 
	L/R channels, and some for Mid/Side channels. But since the masking thresholds
	depend on previous (and following) frames, you can only compute the masking 
	for a given granule if you've computed it for the 2 previous granules. Thus to
	implement Mid/Side masking into the jstereo mode, we would need to compute, for
	all frames, L,R, Mid and Side masking thresholds in l3psycho_anal. This would
	not be as expensive as it sounds since the FFTs only need to be called on the 
	L &amp; R channels. The energy and phase from Mid &amp; Side channels can be
	computed f rom the L &amp; R FFT output. But it would be a major code change.
	<br/> <i>(Note: this is now done in LAME 3.21 with the -h option. It will
	eventually become the default).</i>
</p>

<p>
	What's done right now? Without the -h option, LAME jstereo only computes L &amp; 
	R masking thresholds. If it is encoding a non ms_stereo frame, no problem. If 
	it is encoding Mid &amp; Side channels, then we have to be a little careful.
	We are quantizing Mid/Side channels, but the masking (allowed distortion) is
	given on L &amp; R channels. Thus the computation of the audible distortion 
	has to be done on the L &amp; R channels too. This just involves reforming 
	the L/R MDCT coefficients and the de-quantized L/R coefficients, and is done 
	in calc_noise2.
</p>

</div>
<?php include("footer.html") ?>
</div>

</body>
</html>