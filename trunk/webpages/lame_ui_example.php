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
			<h1>LAME User Interface Guidelines</h1></div>
			<p>
				The following screen is a suggestion of an ideal user interface if you wish to include 
				LAME in your program:
			</p>
			<p align="center">
				<img src="images/lame_ui_example.png" width="487" height="380"  
			alt="The ideal LAME user interface"></img>
			</p>
			<p>
				You should try to provide an user interface as simple as possible, avoiding 
				too many options.
			</p>
			<ul>
				<li>
					&quot;target&quot; bitrate/quality allows choice betwen VBR or CBR/ABR. 
					According to selection, CBR/ABR or VBR options should be grayed out.
				</li>
				<li>
					&quot;encoding engine quality&quot; allows 3 choices: fast, standard or 
					high. This option is not mandatory.
				</li>
				<li>
					a checkbox allows to encode in mono (LAME will downsample). There is no 
					choice between different stereo modes, as the default mode should be optimal. 
					Other modes are likely to decrease quality.
				</li>
				<li>
					CBR/ABR: a slider allows bitrate selection. LAME will use CBR only if the 
					specific checkbox is selected. Default mode when targetting a specific bitrate 
					whould be ABR, as it provides better quality than CBR.
				</li>
				<li>
					VBR: a slider allows quality selection between 10 and 100. This slider is 
					specifically aligned with the CBR/ABR one in order to give a visual indication 
					of the resulting bitrate of the different VBR quality levels.<br />
					A menu is allowing choice between the two VBR modes of LAME 3.x.
				</li>
				<li>
					A small note is indicating that the encoder used is LAME (as required by 
					the LGPL license). If the name &quot;LAME&quot; is problematic by itself, 
					something similar to &quot;encoding technology by Mp3Dev.org&quot; could also 
					be suitable, although specifically mentionning LAME is prefered.
				</li>
			</ul>
			<p>
				In most cases, there should be no need to provide other options. The ones offered 
				in this example screen should already provide optimal results in the vast majority 
				of situations.
			</p>
		</div>
	<?php include("footer.html") ?>
	</div>
</body>
</html>