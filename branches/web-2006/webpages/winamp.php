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
			<h1>The Winamp/100Hz Decoding Bug</h1></div>
			<h3>UPDATE 10/2/00: Winamp Bug Fixed!</h3>
			<p>
				This probably shouldn't be called a Winamp bug or a <a href="http://www.mp3dev.org">
				LAME</a> bug, but instead a flaw in the MP3 ISO specification.
			</p>
			<p>
				It was caused by assuming that the Huffman "big_values" data is at most 8191 (2^13 -1). 
				13 bits are used to encode these numbers, but in some cases during decoding you need to
				add 15 to the result. Thus the effective range is 0 through 8206. However, the ISO MP3 spec 
				says that the maximum value should be 8191, and it is not clear if they are refering to the 
				maximum value before or after adding the 15, and the ISO demonstration source (dist10)
				uses values up to 8206. Imposing a maximum value of 8191 is a completely unneeded 
				restriction which results in a (very tiny) loss of quality.
			</p>
			<p>
				The 8191/8206 issue was first described by Rob Leslie, author of the 
				<a href="http://www.mars.org/home/rob/proj/mpeg/">MAD decoder</a>. It was placed on 
				the todo list to decide if LAME should use values up to 8206 or limit them to 8191. It took 
				several months for us to realize that this issue was triggering the "Winamp bug".
			</p>
			<p>
				Developers for Winamp and Sonique have already produced versions of their software which 
				can decode 100only.mp3. So this should be fixed in the releases dated after 10/2/00.
			</p>
			<p>
				LAME 3.88 will either be set to not encode big_values greater than 8191, or this limit will 
				be enforced if the "--strictly-enforce-ISO" option is used.
			</p>
			<p>
				The LAME project often receives reports of this bug. The problem seems to be that the LAME 
				produced mp3 files of simple harmonics like the pure 100Hz tone shown below is decoded 
				incorrectly with Winamp/Nitrane. Testing shows that this is in fact not a bug in the LAME 
				produced mp3 file, but a bug in the Nitrane decoding engine used by Winamp.
			</p>
			<p>
				The following players will decode the MP3 file with no problems: mpg123, xmms, freeamp 
				(Xing engine), musicmatch jukebox, windows media player, sonique 1.30.4, UltraPlayer 1.0.
			</p>
			<p>
				More evidence of bugs in Nitrane can be found in <a 
				href="http://www.mail-archive.com/mp3encoder@geek.rcc.se/msg02361.html">Matthew
				Loyd's post in the mp3encoder mailing list.</a>
			</p>
			<p>
				Naoki Shibata has also documented some bugs in Nitrane, and has produced a Winamp 
				decoder plugin based on mpg123. This plugin replaces Nitrane. See <a 
				href="http://www.geocities.co.jp/Technopolis/9674/in_mpg123.html">Naoki's web site.</a>
			</p>
			<p>
				Even more MP3 decoder bugs can be found at 
				<a href="http://players.shoutclub.net/wavcompare/">MP3 Player Quality Comparson 
				Site.</a> and <a href="http://privatewww.essex.ac.uk/~djmrob/mp3decoders/">MP3
			decoder tests.</a> Both sites document severe flaws in several popular decoders.</p>
<pre>Test samples:
<a href="100only.wav">100only.wav</a>        A pure 100 Hz sine wave
<a href="100only.mp3">100only.mp3</a>        Encoded with "lame -h"
<a href="sweep.wav">sweep.wav</a>          A sine wav sweep from 20Hz-20,000Hz
<a href="sweep.mp3">sweep.mp3</a>          Encoded with "lame -h"</pre>
			<p>
				Here are some illustrations showing two Nitrane bugs when decoding the 100only.mp3 
				file.<br />
				(Similar problems show up with sweep.mp3).
			</p>
			<h3>Periodic drop out. This dropout occurs periodically in the Nitrane decoded file (about once per 
			second)</h3>
			Here is a picture of the wav form around sample 2500, of the Nitrane decoded 100only.mp3:
			<p>
				<img src="images/100only.winamp.time2500.gif" width="547" height="349" alt="nitrane at sample 2500" />
			</p>
			<p>
				And here is the same picture, only using mpg123 to decode the mp3 file: <br />
				(similar correct results are obtained with other decoders such as MusicMatch and Sonique)<br />
				<img src="images/100only.mpg123.time2500.gif" width="547" height="349" alt="mpg123 at sample 2500" />
			</p>
			<h3>Nitrane glitch at beginning of file:</h3>
			Here is the original .wav file, starting at time=0s:
			<p>
				<img src="images/100only.time0.gif" width="547" height="349" alt="wav file at beginning" />
			</p>
			<p>
				Here is the .wav file produced by Nitrane, from 100only.mp3, at time 0: (note that all encoders 
				and decoders introduce a delay, in this case around 1000 samples)<br />
				<img src="images/100only.winamp.time0.gif" width="547" height="349" alt="nitrane at beginning" />
			</p>
			<p>
				And here is the .wav file produced my mpg123, from 100only.mp3, at time 0:<br />
				<img src="images/100only.mpg123.time0.gif" width="547" height="349" alt="mpg123 at beginning" />
			</p>
			<hr></hr>
			<p>
				Wave form plots produced by <a href="http://sweep.sourceforge.net">sweep</a>.<br />
				Captured and converted to gif by <a href="http://www.trilon.com/xv/">xv</a>.
			</p>
		</div>
	<?php include("footer.html") ?>
	</div>
</body>
</html>