<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>
<head>
	<title>LAME MP3 Encoder :: Speed Benchmarks</title>
	<meta name="author" content="Roberto Amorim - roberto@rjamorim.com" />
	<meta name="generator" content="jEdit 4.2" />
	<meta name="cvs-version" content="$Id: speed.php,v 1.3 2007-01-13 00:22:20 rjamorim Exp $" />
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
	<h1>Speed Benchmarks</h1>
</div>

<pre>January 2000:
From: "Peter Olufsen"  - po@dsinet.dk

Processor:     600Mhz pentium III
Platform:      Win-98
WAV file:      262sec. song (45.2Mb).

 CBR, high quality (-h / qual 9), full freq. range (-k)

                   Mp3Enc31    BladeEnc     Lame3.60
192 (-ms)            586         166          60
128 (-mj)            795         182          90</pre>

<hr />

<pre>November  1999:
From: "Ethan Yeo" - ewyeo@bigfoot.com

Processor:    AMD Athlon 650
Platform:     Microsoft Windows NT 4.0 SP6
WAV File:     Track 1 from "The Prince of Egypt" OST

Encoder          Timing*       Size
-----------------------------------------------
Track1.wav       304 sec  53,712,668 Bytes
BladeEnc 0.82    153 sec   4,871,604 Bytes   bladeenc track1.wav track1.mp3
LAME 3.51         80 sec   4,872,440 Bytes   lame -h track1.wav track1.mp3
LAME 3.57beta     75 sec   4,872,022 Bytes   lame -h track1.wav track1.mp3

LAME 3.51         70 sec   4,872,440 Bytes   lame -h -m s track1.wav track1.mp3
LAME 3.57beta     64 sec   4,872,022 Bytes   lame -h -m s track1.wav track1.mp3</pre>

<hr />

<pre>July 1999:  LAME 3.13 timings from  Alex Nemirovsky - alexn@transport.com


WAV file:   284 seconds


266 Mhz Pentium, 66 Mhz FSB, 512 KB cache at 66Mhz   encoding time:       295 sec,
233 Mhz Pentium II/ 66Mhz FSB, 512 KB cache at 115Mhz encoding time:      234 sec,
200 Mhz Pentium Pro/66Mhz FSB, 256KB cache at 200Mhz encoding time:       257 sec,
300 Mhz Celeron 300A/66Mhz FSB, 128KB cache at 300Mhz encoding time:      180 sec,
300 Mhz Pentium II/66Mhz FSB, 512 KB cache at 150Mhz, encoding time:      176 sec,</pre>

</div>
<?php include("footer.html") ?>
</div>

</body>
</html>