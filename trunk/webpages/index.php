<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
  <title>LAME MP3 Encoder</title>
  <meta name="author" content="Roberto Amorim - roberto@rjamorim.com" />
  <meta name="generator" content="jEdit 4.2" />
  <meta name="cvs-version" content="$Id: index.php,v 1.19 2017-10-14 08:05:59 aleidinger Exp $" />
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
	<h1>The LAME Project</h1>
</div>

<p>
	LAME is a high quality MPEG Audio Layer III (MP3) encoder licensed under the
	LGPL.
</p>

<h3 id="latest_release">Latest LAME release: <a href="download.php">v3.100</a>
(October 2017)</h3>

<p>
	LAME development started around mid-1998. Mike Cheng started it as a patch
	against the 8hz-MP3 encoder sources. After some quality concerns raised by
	others, he decided to start from scratch based on the dist10 sources. His
	goal was only to speed up the dist10 sources, and leave its quality untouched.
	That branch (a patch against the reference sources) became Lame 2.0, and with
	Lame 3.81 all of dist10 code was replaced, making LAME no more only a
	patch.
</p>

<p>
	The project quickly became a team project. Mike Cheng eventually left
	leadership and started working on tooLame, an MP2 encoder. Mark Taylor became
	leader and started pursuing increased quality in addition to better speed. He
	can be considered the initiator of the LAME project in its current form. He
	released version 3.0 featuring gpsycho, a new psychoacoustic model he
	developed.
</p>

<p>
	In early 2003 Mark left project leadership, and since then the project has
	been lead through the cooperation of the active developers (currently 4
	individuals).
</p>

<p>
	Today, LAME is considered the best MP3 encoder at mid-high bitrates and at
	VBR, mostly thanks to the dedicated work of its developers and the open
	source licensing model that allowed the project to tap into engineering
	resources from all around the world. Both quality and speed improvements are
	still happening, probably making LAME the only MP3 encoder still being
	actively developed.
</p>

<h3 id="quick_links">Quick Links</h3>

<ul>

   <li>
      <a href="contact.php">Contact</a> - to get in touch with LAME developers,
      with other LAME users, or to submit bug reports.
   </li>

   <li>
      <a href="download.php">Download</a> - to obtain the latest LAME source
      code.
   </li>

   <li>
      <a href="links.php">Links</a> - to get to know about software using LAME,
      obtain precompiled LAME binaries from external sites, and discover more
      information about MP3.
   </li>

   <li>
      <a href="developers.php">Developers</a> - the people behind the LAME project.
   </li>

   <li>
      <a href="https://svn.code.sf.net/p/lame/svn/trunk/lame/doc/html/history.html">History/ChangeLog</a>
      - what has been happening lately.
   </li>

</ul>

</div>
<?php include("footer.html") ?>
</div>

</body>
</html>
