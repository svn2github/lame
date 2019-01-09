<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
    <title>LAME MP3 Encoder :: Inside</title>
    <meta name="author" content="Roberto Amorim - rjamorim@yahoo.com" />
    <meta name="generator" content="jEdit 5.4" />
    <meta name="cvs-version" content="$Id: inside.php,v 1.7 2009-11-03 16:11:01 rjamorim Exp $" />
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
    <h1>Inside LAME</h1>
</div>

<ul>
    <li>
        <a href="download.php">Download the source code (SVN and .tar packages available).</a>
    </li>

    <li>
        <a href="developers.php">Who is The LAME Project</a>
    </li>

    <li>
        <a href="https://svn.code.sf.net/p/lame/svn/trunk/lame/doc/html/history.html">History/ChangeLog</a>
    </li>

    <li>
        <a href="refs.php">References</a>
    </li>
</ul>

<h3>LAME internal features :</h3>

<ul>
    <li>
        Many improvements in quality in speed over ISO reference software. See <a
        href="https://svn.code.sf.net/p/lame/svn/trunk/lame/doc/html/history.html">history</a>.
    </li>

    <li>
        MPEG1, MPEG2 and MPEG2.5 layer III encoding.
    </li>

    <li>
        CBR (constant bitrate) and two types of variable bitrate, <a
        href="gpsycho.php">VBR and ABR</a>.
    </li>

    <li>
        Free format encoding and decoding.
    </li>

    <li>
        <a href="gpsycho.php">GPSYCHO</a>: an LGPL'd psycho acoustic and noise
        shaping model.
    </li>

    <li>
        <a href="screenshots.php">MP3x:</a> a GTK/X-Window MP3 frame analyzer for
        both .mp3 and unencoded audio files.
    </li>
</ul>

</div>
<?php include("footer.html") ?>
</div>

</body>
</html>
