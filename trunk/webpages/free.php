<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>
<head>
	<title>LAME MP3 Encoder :: A Free Audio Compression Format?</title>
	<meta name="author" content="Roberto Amorim - roberto@rjamorim.com" />
	<meta name="generator" content="jEdit 4.2" />
	<meta name="cvs-version" content="$Id: free.php,v 1.3 2007-01-13 00:17:53 rjamorim Exp $" />
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
	<h1>A Free Audio Compression Format?</h1>
</div>

<p>Related Links:</p>

<ul>

	<li>
		<a href="http://www-ccrma.stanford.edu/~bosse">The MUS420 project</a>.
	</li>
	
	<li>
		<a href="http://www.xiph.org">Ogg/Vorbis</a>: A new format from the
		creators of cdparanoia.
	</li>
	
	<li>
		<a href="http://www.audiocoding.com">The FAAC project</a>: An open source
		AAC implementation.
	</li>

</ul>

<p>
	Based on what I've learned working with <a href="index.php">LAME</a> and <a
	href="gpsycho.php">GPSYCHO</a>, I believe it would not be too difficult to
	develop an independent audio codec of slightly better quality than LAME, (and
	thus comparable to the best commercial MP3 codecs). Yes, it wont be as good as
	AAC, but think of this: how many people would use a proprietary compression 
	code which was slightly better than gzip?
</p>

<p>
	The following is an outline of such an audio codec. It is based on general
	published ideas which form the basis of several encoders (MP3, MPEG4-AAC,
	AT&amp;T PAC). It includes most of the ideas that were used by the MP3 format,
	but removes many of the components in MP3 which are inherited from layer I 
	and layer II. I have also added some newer ideas that were utilized by AAC 
	and PAC. Some of the more sophisticated ideas such as temporal noise shaping 
	and predictive coding are not included.
</p>

<p>
	The problem with all of this: many of these fundamental ideas are patented in
	countries which allow patents on algorithms. There is still a difference 
	between ideas and algorithms, so it may be possible to implement this codec 
	using different algorithms for the same ideas. It will require a significant 
	amount of legal work to make this determination. If your beliefs do not 
	coincide with the patent holder's beliefs, you could be sued and the courts 
	will decide. If you dont have the money for such a law suit, then that is the 
	end of the project!
</p>

<p>
	Just a cursory patent search will yield dozens of patents on every aspect of
	audio compression. Below I have referenced some of these patents along with 
	my uninformed interpretation of what they claim.
</p>

<h3>Frame/Window types</h3>

<ul>

	<li>
		1024 and 128 (for pre-echo) sample windows. MP3 uses 576 &amp; 192 sample
		windows. AAC uses 1024 and 128 sample windows. (Brandenburg &amp; Stoll 
			1994, Bosi et al. 1997 in <a href="refs.php">References</a>).
	</li>
	
	<li>
		Spectral coefficients computed from overlapping MDCT coefficients (lossless). 
		MP3 and AAC apply the MDCT only after first splitting the signal into 
		frequency bands with windowed filterbanks.
	</li>
	
	<li>
		Pre-echo detection from the <a href="gpsycho.php">GPSYCHO</a> algorithm.
		The GPSYCHO pre-echo detection algorithm is truely original, although it 
		is such a simple concept that I'm sure someone has patented it.
	</li>

</ul>

<p>
	The very concept of using spectral transforms applied to frames of PCM
	samples seems to be patented (US5579430). But I believe spectral transforms 
	(or filterbanks) must be used because psycho acousticinformation is given in 
	terms of spectral coefficients (the frequency domain). The majority of audio
	compression comes from allocating bits between different frequency bands 
	based on psycho acoustic information.
</p>

<p>
	The concept of window switching to reduce pre-echo effects is patented in
	US5285498.
</p>

<h3>Critical Bands</h3>

<ul>

  <li>
	  Group coefficients in critical bands. MP3 uses 21 for long windows, 12 for
	  short. AAC uses 49 for long windows, 14 for short.
  </li>

  <li>
	  Allow option of <a href="ms_stereo.php">mid/side encoding</a> for each
	  critical band. MP3 does not allow mid/side encoding on a band by band basis.
	  AT&amp;T PAC does. (Johnston &amp; Ferrera 1992 <a
	  href="refs.php">References)</a>.
  </li>

</ul>

<p>
	Critical bands are a way to group frequency bands which better mimics the
	response of the human ear. The concept is old, but there may be patents on 
	the use of critical bands for audio compression. The concept of mid/side 
	encoding is patented in US5481614.
</p>

<h3>Quantization of MDCT coefficients</h3>

<ul>

  <li>
	  Associated to each critical band is a scale factor. The larger the scale
	  factor, the more bits allocated to this critical bands.
  </li>

  <li>
	  Truncate MDCT coefficients *scalefactor to integers. This is all that is
	  meant by Quantization.
  </li>

  <li>
	  Choose scale factors so quantization distortion in each critical band is
	  less than the masking computed by the psycho-acoustic model.
  </li>

  <li>
	  If more compression is desired (with some distortion) choose scale factors
	  with <a href="outer_loop.php">GPSYCHO algorithm</a>. Compression can be
	  controlled to produce a given bitrate, or given quality.
  </li>

</ul>

<p>
	The use of scale factors to control the allocation of bits between scale
	factor bands is patented. Even worse, just the concept of allocating bits 
	among critical bands based on any set of external requirements is patented
	(US5579430).
</p>

<h3>Lossless compression of quantized MDCT coefficients</h3>

<p>
	Some type of lossless compression and encoding of quantized data. MP3 uses
	Huffman coding with precomputed tables each assigned a unique code.
</p>

<p>
	The type of Huffman coding used in MP3 is patented (US5579430). Are there
	other types of Huffman coding which we could use? Is the concept of precomputed
	tables patentable? Or are just the tables themselves patented? A version of the
	algorith in gzip, optimized for audio frames would probably be the best. But
	just the very fact of using optimized encoding is claimed to be patented!
	(US5579430).
</p>

<h3>Psycho-acoustic model (output used during quantization step)</h3>

<ul>

	<li>
		Masking given by a linear function expressed in critical bands.
	</li>

	<li>
		Strength of masking given from tonality of signal.
	</li>
	
	<li>
		Tonality estimated by a measure of the predictability of the signal.
	</li>
	
	<li>
		<cite>Johnson (1988) and Brandenburg &amp; Johnston (1990)</cite>, <a
		href="refs.php">References</a>
	</li>

</ul>

<p>
	The algebraic formulas for these quantities are in the published literature.
	The tonality formula is patented in US5040217.
</p>

</div>
<?php include("footer.html") ?>
</div>

</body>
</html>