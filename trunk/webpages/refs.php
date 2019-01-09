<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
    <title>LAME MP3 Encoder :: References</title>
    <meta name="author" content="Roberto Amorim - rjamorim@yahoo.com" />
    <meta name="generator" content="jEdit 5.4" />
    <meta name="cvs-version" content="$Id: refs.php,v 1.7 2009-11-03 16:11:01 rjamorim Exp $" />
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
    <h1>References</h1>
</div>

<p>
    In addition to the ISO reference documentation ($120 from ANSI in the US),
    here are some less detailed but more infomative articles.
</p>

<p>
    You can find links to lots of documentation at <a 
    href="http://www.mp3-tech.org">www.mp3-tech.org</a>'s programmers' corner.
</p>

<p>
    <a href="http://www.eas.asu.edu/~speech/ndtc/">NDTC Speech and Audio Coding
    Page</a> contains a detailed and very well written review article on 
    psycho-acoustics.
</p>

<p>
    Davis Pan has <a href="doc/vol5num2art3.pdf">a couple of useful overviews</a>
    of MPEG audio encoding.
</p>

<p>
    <a href="http://www.tnt.uni-hannover.de/papers/view.php?ind=Baumgarte">
    Several papers by Frank Baumgarte</a>. The paper by Baumgarte, Ferekidis and 
    Fuchs describes an alternative psycho-acoustic model for MPEG encoding.
</p>

<p>
    <a href="http://www-ccrma.stanford.edu/~bosse/">A complete (non MPEG) MDCT 
    based audio encoder</a> (MUS420 class project). The assocated paper gives 
    some good information on audio encoding.
</p>

<p>
    A good paper on mid/side stereo encoding:<br /> Johnston and Ferreira,
    Sum-Difference Stereo Transform Coding, Proc. IEEE ICASSP (1992) p 569-571.
</p>

<p>
    A lot in the MPEG2-AAC can also be used in MP3:<br /> Bosi et al. "ISO/IEC
    MPEG-2 AAC", J. Audio Eng. Soc. 45 (1997) p 789-814.
</p>

<p>
    And the original MPEG1 reference:<br /> Brandenburg &amp; Stoll, "ISO-MPEG-1
    Audio: A Generic Standard for Coding of High-Quality Digital Audio", J. Audio
    Eng. Soc 42 (1994) p 780-792.
</p>

<p>
    Some original papers on the psycho-acoustics used by MPEG:<br /> Johnston,
    "Transform Coding of Audio Signals Using Perceptual Noise Criteria", IEEE J.
    Selected Areas Communications, (1988).<br /> Brandenburg and Johnston, 
    "Second Generation Perceptual Audio coding: The Hybrid Coder", AES 89th 
    Convention, 1990.
</p>

<p>
    Shape of masking curve for tonal sounds (used in ISO model2):<br /> Schroeder,
    Atal &amp; Hall, "Optimizing digital speech coders by exploiting masking
    properties of the human ear", JASA Vol.66 N&deg;6 (1979) p 1647-1652
</p>

<p>
    Tonality estimation used in ISO model 1,<br /> ATH shape (sometimes 
    incorrectly reffered as Painter &amp; Spanias formula):<br /> Terhardt &amp; 
    Stoll, "Algorithm for extraction of pitch and pitch salience from complex 
    tonal signals", JASA Vol.71 N&deg;3 (1982) p 679-688
</p>

<p>
    Shape of masking curve depending upon stimulus intensity:<br /> Sporer &amp;
    Brandenburg, "Constraints of filter banks used for perceptual measurement", 
    JAES Vol.43 N&deg;3 (1995) p 107-115
</p>

</div>
<?php include("footer.html") ?>
</div>

</body>
</html>

