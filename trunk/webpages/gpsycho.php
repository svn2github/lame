<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>
<head>
	<title>LAME MP3 Encoder :: GPSYCHO - A LGPL'd Psycho-Acoustic Model</title>
	<meta name="author" content="Roberto Amorim - roberto@rjamorim.com" />
	<meta name="generator" content="jEdit 4.2" />
	<meta name="cvs-version" content="$Id: gpsycho.php,v 1.3 2007-01-13 00:18:17 rjamorim Exp $" />
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
	<h1>GPSYCHO - A LGPL'd Psycho-Acoustic Model</h1>
</div>

<p>
	GPSYCHO is an open source psycho-acoustic and noise shaping model for ISO
	based MP3 encoders. GPSYCHO fixes some substantial bugs in the ISO demonstration
	source psycho-acoustic model (ISO psy-model). In addition, GPSYCHO adds mid/side
	stereo, real bit reservoir control, much improved critical band bit allocation
	routines, variable bit rate (optional) and very good pre-echo control. At
	128kbs, the quality is significantly better than that produced by the ISO
	psy-model (as found in almost all other free encoders). An example of these
	improvements is shown in <a href="screenshots.php">Screenshots</a>. GPSYCHO is
	close to the quality of the FhG encoder, but there is still room for
	improvement. Read on if you want to help!
</p>

<p>
	As this code is released under the LGPL, it can be used in any projects, even
	commercial ones. I would also encourage others to help improve GPSYCHO. Some
	things that would help:
</p>

<ul>

	<li>
		Find (and send me) samples where your favorite encoder does a better job
		than GPSYCHO.
	</li>
	
	<li>
		Run your own listening tests and try tuning some of the algorithms below.
		Most have parameters that are set via trial and error.
	</li>
	
	<li>
		Try out new algorithms!
	</li>

</ul>

<p>
	The GPSYCHO algorithms are rigorously tested through listening tests. You can
	find more infomation <a href="quality.php">here</a>.
</p>

<p>
	GPSYCHO operates in a number of modes. Notes are available to provide
	information about each of them:
</p>

<ul>

	<li>
		<a href="vbr.php">GPSYCHO variable bit rate (VBR).</a>
	</li>
	
	<li>
		<a href="abr.php">GPSYCHO average bit rate (ABR).</a>
	</li>
	
	<li>
		<a href="ms_stereo.php">GPSYCHO Mid/Side Stereo.</a>
	</li>
	
	<li>
		<a href="outer_loop.php">GPSYCHO outer_loop quantization algorithm (CBR
			noise shaping).</a>
	</li>

</ul>

<h3>New Features (which may need some tuning):</h3>

	<ul>
		<li>
			Bit allocation outer loop improved based on ideas in an MPEG2 J. Audio
			Eng. Soc. 1997 paper. The ISO demonstration source outer loop can produce
			some very poor quality frames in certain situations.
		</li>
		
		<li>
			VBR (variable bit rate) is now working! See the VBR link above for
			details.
		</li>
		
		<li>
			<code>MS_STEREO</code> switch. ISO formula is primitive. I use a switch
			described <a href="ms_stereo.php"> here.</a>
		</li>
		
		<li>
			<code>MS_STEREO</code> ISO sparsing formula does not work. It will 
			remove 95% of the side channel coefficients. GPSYCHO does not sparse 
			the side channel at all, but allocate less bits for encoding. <a
			href="mailto:e9427483@student.tuwien.ac.at">Martin Weghofer</a> has a 
			coder which does effectively use side channel sparsing, but the 
			algorithm does not work well with the LAME quantization procedure. This 
			is an area that needs further work.
		</li>
		
		<li>
			<code>MS_STEREO</code> now uses ideas in a Johnson ICASSP 1992 paper to
			compute true Mid and Side thresholds which compensate for stereo 
			de-masking. Similar to that used in PAC and AAC.
		</li>
		
		<li>
			Bit reservoir use. Again the ISO formula performs poorly. At 128kbs, it
			always thinks it needs to drain the reservoir, and thus the reservoir 
			can never build up. It will also use up all the bits for the left 
			channel before even looking at the right channel.
		</li>
		
		<li>
			Mid/Side bit allocation. GPSYCHO allocate bits based on the differences
			between left and right masking thresholds. Anyone have a better idea?
		</li>
		
		<li>
			Lowpass filtering based on the compression ratio. For high compression
			ratios, low pass filtering will improve the results. The exact amount 
			of filtering needed depends on the music and personal preferences - the 
			formula to decide how much lowpass filtering to use may need some 
			tuning. At 256kbs, no filterings is done. At 128kbs, the lowpass filter 
			is around 15.5khz.
		</li>
		
		<li>
			Improved shortblock switching. It is now based on surges in PE or large
			fluctuations in energy within a single granule. These improvements 
			trigger some critical window switching that LAME used to miss.
		</li>
	
	</ul>

<h3>Features to try out:</h3>

<ul>

	<li>
		Add a high-pass filter. 20Hz?
	</li>
	
	<li>
		Shorter FFT for the long block noise threshold calculation. A 768 FFT
		centered over the 576 sample granule would be more accurate for the high
		frequency energies than the 1024 FFT. This should also improve the 
		perceptual entropy (pe) calculation since there will be less interference 
		from data outside the granule. Another advantage might be for the 
		applaud.wav test - see the <a href="quality.php">Quality</a> section for 
		details. It will of course make the low frequency energy estimates less 
		accurate.
	</li>
	
	<li>
		Subblock_gain. This seems to be important. FhG uses it for most short
		blocks. LAME and other dist10 based codes do not make any use of this. VBR
		modes will use subblock gain.
	</li>

</ul>

<h3>Things we've learned from analyzing FhG (mp3enc3.1) produced .mp3 frames:</h3>

<ul>

	<li>
		FhG uses mixed_blocks only if specified as an option.
	</li>
	
	<li>
		FhG uses intensity stereo only at lower bitrates.
	</li>
	
	<li>
		FhG does not seem to use scsfi &lt;&gt; 0.
	</li>
	
	<li>
		Removes data in scalefactor band 21 at 128kbs.
	</li>
	
	<li>
		Almost always uses ms_stereo. Does not use ISO formula for ms_stereo
		switching.
	</li>
	
	<li>
		More sophisticated mid/side bit allocation.
	</li>
	
	<li>
		Excellent short block detection.
	</li>
	
	<li>
		Good bit reservoir use. Not totally based on pe, since they often
		allocate extra bits to long blocks.
	</li>

</ul>

</div>
<?php include("footer.html") ?>
</div>

</body>
</html>