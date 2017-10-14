<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<title>LAME MP3 Encoder :: Rationale for LAME Development</title>
	<meta name="author" content="Roberto Amorim - roberto@rjamorim.com" />
	<meta name="generator" content="jEdit 4.2" />
	<meta name="cvs-version" content="$Id: rationale.php,v 1.5 2009-11-03 16:11:01 rjamorim Exp $" />
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
	<h1>Rationale for LAME Development</h1>
</div>


<h3>by Mike Cheng, September 1998</h3>

<p>
	A few years back I started hearing about this fantastic new sound compression
	technology; mpeg layer 3. I had used layer 2 before and was very impressed, 
	and so I started to look for layer3 software.
</p>

<h3>The Amiga</h3>

<p>
	At this time I was a dedicated Amiga user, and I went about finding source
	code to port over to the amiga. I spent much time optimizing the dist10 
	decoder so that I could at least play back these files. Then AMP came along 
	and I gave up optimizing and just ported AMP to the Amiga. Then Stephane 
	Tavenard came along and made a superfast m68 asm mp3 player for the Amiga 
	and I bowed to his greater programming prowess. However, there was an encoder 
	in the dist10 source as well, and I set about porting this to the Amiga.
</p>

<p>
	Now the amiga is a good machine with a fab OS, but it just doesn't have the
	computational grunt to do mp3 encoding at a reasonable speed. I spent lots of
	1997 just playing with the dist10 source code, optimizing little bits and 
	making it faster. This optimizing effort appeared as an amiga-only program.
</p>

<h3>Changing Times</h3>

<p>
	Early this year a group called <b>8hz</b> announced that they were going to
	start a concentrated effort to speed up the ISO source code. I immediately
	signed up. Within a week I had sped up the already fast 8hz effort by
	incorporating my speedups into their code. Over the next few months, I'd
	improved on the 8hz source code - speeding it up by about 45%. All these 
	changes I made public and mailed back to the 8hz guys. It was good to be part 
	of this net effort.
</p>

<h3>Fraunhofer Crackdown</h3>

<p>
	In September 1998, Fraunhofer started cracking down on use of their freely
	available source code (don't ask me why), and the 8hz effort, and a number of
	other free encoders based on ISO sources were also halted. That sucked.
</p>

<h3>Now..</h3>

<p>
	Now I've released a patch file to the mpeg ISO source code. And this is
	called <b>LAME</b> which is gnu-speak for <b>L</b>AME <b>A</b>in't an 
	<b>M</b>p3 <b>E</b>ncoder. [For non-english speakers, "ain't" is a way of 
	saying "is not"]. I haven't released an mp3 encoder. I've released a patch
	file for the dist10 source code. The file I am releasing is totally incapable 
	of producing an mpeg layer 3 file. The file I am releasing cannot be 
	compiled, and is not an executable. You <i>must</i> use the ISO mpeg source 
	code in order to get a functional mpeg layer 3 encoder.
</p>

<h3>Open Source</h3>

<p>
	The <a href="http://slashdot.org/">SlashDot</a> community has got it right.
	Open source is the way to go. BladeEnc's closed source annoys me, because the
	author is not sharing his ideas. [Ed. Note: BladeEnc is now under the LGPL] 
	Due to licensing arrangements I doubt he can make money on BladeEnc, so why 
	not release the source? The claim that it will lead to versions with inferior
	quality is bogus. If you leave yourself as the authoritative source for the
	code, it doesn't matter what everyone else does with it - so why not release 
	it? It can only lead to better software...
</p>

<h3>To the future...</h3>

<p>
	I've been thinking about an open source community effort to develop a
	compressed audio format equivalent/better than mpeg layer 3. In thinking 
	about it, I simplified it down to make it seem like an achievable task:
</p>

<ul>

	<li>
		Only 44.1khz samples. This is CD quality. Don't bother with 32 or 48khz.
	</li>
	
	<li>
		Only 128kbit/s bit rate. The mpeg standard tries to cover bitrates from
		16kbit/s up to 320. That's a huge range and requires different acoustic and
		computational models. 128kbit/s produces CD-quality sound for mpeg layer3, 
		AAC and VQF.
	</li>

</ul>

<p>
	Anyone who has ideas on this is welcome to get in contact with me.
</p>

<p>
	<b><a href="mailto:mikecheng@cryogen.com">mike/cstar</a></b> 
</p>

</div>
<?php include("footer.html") ?>
</div>

</body>
</html>