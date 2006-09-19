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
	<?php include("menu.html") ?>
	<div id="container">
		<div id="content">
			<div align="center"><img src="./images/logo.gif" width="358" height="231" alt="LAME Official logo"></img>
			<h1>MP3x Screenshots</h1></div>
			<p>
				MP3x is an graphical frame analyzer for both MP3 and unencoded audio files. It displays frame 
				header information, original and decoded pcm data, MDCT spectrum, distortion, allowable 
				distortion and other psycho-acoustic information. MP3x is distributed with the LAME source 
				code - see the file INSTALL for information about compiling MP3x.
			</p>
			<p>
				The following screenshots are from MP3x analyzing the file <a 
				href="http://www-ccrma.stanford.edu/~bosse/">castanets.wav</a>, or in the case of the 
				FhG encoder and Mpecker, the frame analyzer was run directly on the castanets.mp3 file 
				produced by these encoders.
			</p>
			<h3><a href="images/gpsycho.gif">Castanets.wav using GPSYCHO (LAME 3.10)</a></h3>
				<ul>
					<li>
						Analysis with mp3x using the GPSYCHO psycho-acoustic model.
					</li>
					<li>
						GPSYCHO almost completely removes the pre-echo effect in both granules.
					</li>
					<li>
						GPSYCHO makes good use of the bit reservoir - 1708 bits are used to code the mid 
						channel for this granule, and 956 for the side channel.
					</li>
					<li>
						Unencoded MDCT coefficients are shown (the display can be toggled between encoded 
						and unencoded.)
					</li>
				</ul>
			<h3><a href="images/fhg.gif">Castanets.mp3 from FhG MP3Encoder</a></h3>
				<ul>
					<li>
						Analysis of an FhG encoded .mp3 file with mp3x. Since we are analyzing an already 
						encoded file, there is no psycho-acoustic information nor can the original .wav file data
						or original MDCT coefficients be plotted.
					</li>
					<li>
						Note the almost complete lack of pre-echo.
					</li>
					<li>
						MP3Encoder is making use of MS_STEREO and has almost doubled the number of bits used 
						to encode 	the granule with the sharp attack.
					</li>
					<li>
						The attack occurs in frame 39 instead of frame 38 as it does in the other screenshots. This 
						is because the FhG encoder has an extra filter bank delay as compared to the ISO encoder 
						(perhaps coming from a high-pass filter?). I added padding to the castanets.wav to make this 
						delay exactly one frame.
					</li>
				</ul>
			<h3><a href="images/iso.gif">Castanets.wav using ISO psy-model</a></h3>
				<ul>
					<li>
						Analysis with mp3x using the original ISO psycho-acoustic model.
					</li>
					<li>
						Most freely available encoders will produce identical results (LAME 2.1, 8hz-mp3, 
						BlaceEnc, CDex with ISO .dll)
					</li>
					<li>
						Note the pre-echo introduced throughout the entire frame, and the fact that the algorithm 
						failed to switch to short blocks for the sharp attack of the castanets. It will switch on the 
						next frame, one granule too late.
					</li>
					<li>
						Only 771 bits is used to encode this granule - it is not taking bits from the bit reservoir, 
						nor is it exploting extra bits from mid/side stereo.
					</li>
					<li>
						Unencoded MDCT coefficients are shown (the display can be toggled between 
						encoded and 	unencoded)
					</li>
				</ul>
			<h3><a href="images/mpecker.gif">Castanets.wav using MPECKER</a></h3>
				<ul>
					<li>
						Analysis with mp3x of MPECKER encoded castanests.mp3. Since we are analyzing an 
						already encoded file, there is no psycho-acoustic information, nor can the original .wav 
						data or original MDCT coefficients be plotted.
					</li>
					<li>
						Mpecker has many strenghts, but does not do well with pre-echo control. It does not 
						switch to 	short blocks, nor does it make any use of the bit reservoir. Future versions 
						may overcome this with variable bit rate.
					</li>
					<li>
						Mpecker does make use of mid/side stereo, allocating 896 bits for the midchannel of this 
						granule and 688 for the side channel.mid/side stereo.
					</li>
				</ul>
		</div>
	<?php include("footer.html") ?>
	</div>
</body>
</html>