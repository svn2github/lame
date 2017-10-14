<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<title>LAME MP3 Encoder :: GPSYCHO - Average Bit Rate (ABR)</title>
	<meta name="author" content="Roberto Amorim - roberto@rjamorim.com" />
	<meta name="generator" content="jEdit 4.2" />
	<meta name="cvs-version" content="$Id: abr.php,v 1.5 2009-11-03 16:11:01 rjamorim Exp $" />
	<meta http-equiv="content-type" content="text/html; charset=UTF-8" />
	<link rel="stylesheet" type="text/css" href="styles/lame.css" />
	<!--[if IE 6]>
	<link rel="stylesheet" type="text/css" href="styles/ie.css" />
	<![endif]-->
</head>
<body>

<?php include("menu.html") ?>

<div id="container">
<div id="content">

<div align="center">
	<img src="images/logo.gif" width="358" height="231" alt="LAME Official Logo" />
	<h1>GPSYCHO - Average Bit Rate (ABR)</h1>
</div>


<h3>
	A variable bit-rate algorithm which uses GPsycho's time tested CBR noise
	shaping code
</h3>

<p>Suggested usage:</p>

<blockquote class="code">lame --abr 128 input.wav output.mp3</blockquote>
			
<p>
	ABR mode is like CBR, but with an unlimited bit reservoir. When using ABR,
	GPSYCHO will use the CBR algorithm to compute the number of bits needed to
	encode each frame. If the number of bits is greater than the target bitrate, 
	the CBR algorithm has to use bits from the bit reservoir, and we just have to 
	hope that the bitreservoir has enought bits!
</p>

<p>
	With ABR, GPSYCHO does not rely on the bit reservoir at all, but each frame
	just uses the smallest possible bitrate which can encode the frame with the
	desired number of bits.
</p>

<p>
	The difference between ABR and true VBR is in how the desired number of bits
	is chosen. The true VBR mode determines the number of bits based on the
	quantization noise. VBR figures out how many bits are needed so that the
	quantization noise is less than the allowed masking.
</p>

<p>
	ABR mode uses the CBR formula to determine the desired number of bits. This
	formulas is based on the perceptual entropy, which is a rough measure of how
	difficult the frame is to encode.
</p>

</div>
<?php include("footer.html") ?>
</div>

</body>
</html>