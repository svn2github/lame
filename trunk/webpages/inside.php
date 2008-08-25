<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
	<title>LAME MP3 Encoder :: Inside</title>
	<meta name="author" content="Roberto Amorim - roberto@rjamorim.com" />
	<meta name="generator" content="jEdit 4.2" />
	<meta name="cvs-version" content="$Id: inside.php,v 1.6 2008-08-25 02:24:41 rjamorim Exp $" />
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
	<h1>Inside LAME</h1>
</div>

<ul>
	<li>
		<a href="download.php">Download the source code (CVS, .tar and source RPMS
		available).</a>
	</li>
	
	<li>
		<a href="developers.php">Who is The LAME Project</a>
	</li>
	
	<li>
		<a
		href="http://lame.cvs.sourceforge.net/*checkout*/lame/lame/doc/html/history.html?revision=HEAD">History/ChangeLog</a>
	</li>
	
	<li>
		<a href="refs.php">References</a>
	</li>
</ul>

<h3>LAME internal features :</h3>

<ul>
	<li>
		Many improvements in quality in speed over ISO reference software. See <a
		href="http://lame.cvs.sourceforge.net/*checkout*/lame/lame/doc/html/history.html?revision=HEAD">history</a>.
	</li>
	
	<li>
		MPEG1,2 and 2.5 layer III encoding.
	</li>
	
	<li>
		CBR (constant bitrate) and two types of variable bitrate, <a
		href="gpsycho.php">VBR and ABR</a>.
	</li>
	
	<li>
		Free format encoding and decoding.
	</li>
	
	<li>
		<a href="gpsycho.php">GPSYCHO:</a> a GPL'd psycho acoustic and	noise 
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