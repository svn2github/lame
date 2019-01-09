<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
    <title>LAME MP3 Encoder :: About</title>
    <meta name="author" content="Roberto Amorim - rjamorim@yahoo.com" />
    <meta name="generator" content="jEdit 5.4" />
    <meta name="cvs-version" content="$Id: about.php,v 1.7 2009-11-03 16:11:00 rjamorim Exp $" />
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
    <img src="images/logo.gif" width="358" height="231" alt="LAME Official logo" />
    <h1>About LAME</h1>
</div>

<p>
    Following the great history of GNU naming, LAME originally stood for <b><i>
    <font color="#3366ff">L</font>AME <font color="#3366ff">A</font>in't an <font
    color="#3366ff">M</font>p3 <font color="#3366ff">E</font>ncoder</i></b>. LAME
    started life as a GPL'd patch against the dist10 ISO demonstration source, and
    thus was incapable of producing an mp3 stream or even being compiled by itself.
    But in May 2000, the last remnants of the ISO source code were replaced, and now
    LAME is the source code for a fully LGPL'd MP3 encoder, with speed and quality
    to rival and often surpass all commercial competitors.
</p>

<p>
    LAME is an educational tool to be used for learning about MP3 encoding. The
    goal of the LAME project is to use the open source model to improve the psycho
    acoustics, noise shaping and speed of MP3. LAME is not for everyone - it is
    distributed as source code only and requires the ability to use a C compiler.
    However, many popular ripping and encoding programs include the LAME encoding
    engine, see: <a href="links.php">Software which uses LAME</a>.
</p>

<h3>LAME features:</h3>

<ul>
    <li>
        Quality better than <b>all</b> other encoders at most bitrates.
    </li>

    <li>
        Fast! Encodes faster than real time on a PII 266 at highest quality mode.
    </li>

    <li>
        Many improvements in quality in speed over ISO reference software. See <a
        href="https://svn.code.sf.net/p/lame/svn/trunk/lame/doc/html/history.html">
        history</a>.
    </li>

    <li>
        MPEG1, MPEG2 and MPEG2.5 layer III encoding.
    </li>

    <li>
        CBR (constant bitrate) and two types of variable bitrate, <a
        href="gpsycho.php">VBR and ABR</a>.
    </li>

    <li>
        Encoding engine can be compiled as a shared library (Linux/UNIX), DLL ,
        Directshow filter or ACM codec (Windows).
    </li>

    <li>
        Free format encoding and decoding.
    </li>

    <li>
        <a href="gpsycho.php">GPSYCHO:</a> a GPL'd psycho acoustic and noise
        shaping model.
    </li>

    <li>
        Powerful and easy to use presets.
    </li>

    <li>
        <a href="screenshots.php">MP3x:</a> a GTK/X-Window MP3 frame analyzer for
        both .mp3 and unencoded audio files.
    </li>
</ul>

<h3>Information about LAME:</h3>

<ul>
    <li>
        <a href="links.php#Contents">Software which uses LAME</a>.
    </li>

    <li>
        <a href="links.php#MP3_general">MP3 Related Links</a>.
    </li>

    <li>
        <a href="developers.php">Who is the LAME Project</a> and how to <a
        href="contact.php">contact</a> them.
    </li>

    <li>
        <a
        href="https://svn.code.sf.net/p/lame/svn/trunk/lame/doc/html/history.html">
        History and changelog</a>
    </li>

    <li>
        <a href="rationale.php">Rationale, by Mike Cheng, Sept. 1998</a>
    </li>

    <li>
        <a href="http://toolame.sourceforge.net/">TooLAME (layer II encoding)</a>
    </li>
</ul>

</div>
<?php include("footer.html") ?>
</div>

</body>
</html>
