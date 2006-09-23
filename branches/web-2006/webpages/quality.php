<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>
<head>
  <title>LAME MP3 Encoder :: Quality and Listening Test Information</title>
  <meta name="author" content="Roberto Amorim - roberto@rjamorim.com" />
  <meta name="generator" content="jEdit 4.2" />
  <meta name="cvs-version" content="$Id: quality.php,v 1.1.2.2 2006-09-23 18:39:47 kylev Exp $" />
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
  <h1>Quality and Listening Test Information</h1>
</div>

<h3>Tuning by Listening Tests</h3>

<ul>

  <li>Most improvements in GPSYCHO require detailed listening tests. The best
  way to go about this is to find a sample where GPSYCHO does something bad.
  Then see if you can figure out which algorithm/tuning is at fault, and how it
  can be improved without breaking something else!</li>
  
  <li>MPEG likes the "ABC hidden reference test". Signal A is always the
  original .wav file. B and C are the encoded and the original signal, in a
  random order. Listen to ABC three times, always in that order, and rate B and
  C on a scale from 1-5, 5 being for the signal you perceive as the
  riginal.</li>

</ul>

<hr />
                        
<p><a href="preecho.php">Tuning by reverse engineering:</a> Here is a detailed
example on how the pre-echo algorithm in LAME 3.10 was tuned and dramatically
improved by doing a frame by frame comparison with the FhG encoder. First a
sample is found where LAME produces noticeably worse results than the
state-of-the-art FhG encoder. Listening tests are used to determine which frames
are causing most of the problems. MP3x (the frame analyzer) is then used to
compare the troublesome frames produced by LAME to those produced by the FhG
encoder. In the case presented, the problem was because LAME was not switching
to short blocks when it should have.</p>
                        
<h2>Some Test Samples</h2>

<p>The LAME test samples have been encoded with <a
href="http://www.wavpack.com"> WavPack</a>, a free, open source and
multiplatform lossless audio compressor.</p>

<p>Check out <a href="http://sound.media.mit.edu/mpeg4/audio/sqam">SQAM - Sound
Quality Assessment Material.</a> I haven't had a chance to try these yet, but if
you find samples where another encoder does noticeably better than LAME, I would
be very interested.</p>

<h2>Test cases which need work</h2>

<h4><a href="http://lame.sourceforge.net/download/samples">Roel's infamous
velvet.wv</a> (1.3M - 12 seconds)</h4>

<p>Good ATH/Joint Stereo test case. This sample has been used to check validity
of the ATH formula introduced in LAME 3.88</p>
                                
<h4><a href="http://lame.sourceforge.net/download/samples">vbrtest.wv</a> "God
loves his children" with Lou Reid and Carolina. This sample fools some VBR
modes. (0.4M - 5 seconds)</h4>

<p>VBR algorithms based on psycho acoustics (rather than perceptual entropy)
have trouble with this sample. VBR modes encode this sample at about 20kbs less
then average. But this is not a case were VBR is getting the same quality at a
lower bitrate! From <a href="http://home.bip.net/ove.rung">Ove</a>: "Back in
'the old days' I was using tompg.exe for my encoding until I discovered a really
hard-to-encode tune. I still use that tune for testing new encoders, and [MP+]
produces heavy artifacts."</p>

<h4><a href="http://lame.sourceforge.net/download/samples">hihat.wv</a> pre-echo
from a hi-hat. (0.15M - 3 seconds)</h4>

<p>Zia Mazhar writes: "It's a simple sound of a hi-hat. I tried with BladeEnc,
Xing, GOGO-no-coda ver. 2.26, FhG [all versions], and of course, the latest
version of LAME. But none sounds like the original. FhG sounds best, though.
Other encoders add a amplified 'hissing sound' to the file. You can see the
difference even in the <i>WinAmp eq-visualization</i> display. It seems that the
original frequency changes a lot."</p>

<h4><a href="http://lame.sourceforge.net/download/samples">youcantdothat.wv</a>
(3.5M - 26 seconds)</h4>

<p>From Dr. David J M Robinson - DavidR at europe.com. From vinyl - the results
aren't too impressive using any encoder at 128kbps. On the initial guitar riff,
LAME does better than either the old FhG routine in mp3 Prod Pro, or the fast
routine available in Cool Edit Pro. The others have more difficulty with the
echo in the right channel. BUT when the vocals come in, the other codecs create
an undercurrent of noise (which is bad enough) while LAME seems to blast almost
white noise along with the vocal track, which sounds much worse IMHO.</p>

<h4><a href="http://lame.sourceforge.net/download/samples">pipes.wv</a> (1.4M -
15 seconds)</h4>

<p>From magnus at alum.mit.edu. Bagpipes. It's mainly that the pipes are
"muted". There's a certain "crispness" to the original that is lost after
encoding. I'm not exactly sure how else to explain it.</p>

<h4><a href="http://lame.sourceforge.net/download/samples">goldc.wv</a> "The
Ecstasy Of Gold" by Ennio Morricone (1.0M - 10sec, very complex part!)</h4>
			
<p>Daniel Wronski writes: The Encoders: lame 3.70; lame 3.81BETA; FHG 3.1DEMO;
SCMPX 1.51; (constant bitrate; 128kb/sec; highest possible quality) Personal
rating (sound quality only):</p>

<ul>

  <li>1st place: The original sample.</li>

  <li>2nd place: FHG MP3ENC 3.1 Demo (-qual 9) - This is the one and only
  encoder I found, which does not make any additional noise. However the bells
  sound a little bit different (not a LAME problem).</li>

  <li>3rd place: LAME 3.81 BETA (-h) - (no real difference to v3.83) Best
  quality! But there are some high frequency metallic vibrations (at
  ~6kHz?).</li>

  <li>4th place: LAME 3.70 (-h) - More high frequency metallic vibrations.
  Sounds like low cost trumpets are used. (resonance between the metal parts of
  the trumpets) These vibrations even occur, if the trumpets are not
  playing.</li>

  <li>5th place: SCMPX 1.51 - This one even makes additional noise.</li>

</ul>


<h4><a href="http://lame.sourceforge.net/download/samples">ftb_samp.wv</a>
Obvious differences between FhG and LAME. (3.0M - 26 seconds)</h4>

<p>This is a great test case found by jodaman at cegt201.bradley.edu. It is a
selection from Metallica's Fade To Black. At 128kbs, the difference between LAME
and FhG (mp3enc 3.1) is clearly noticable. There are problems with the vocal 's'
sounds, and there is a slight tinny underwater sound. The old FhG encoder l3enc
also has the same problems.</p>
                                
<ul>

  <li><a href="http://cegt201.bradley.edu/~jodaman/dev/mp3">Joda's Space</a> The
  originator of this ftb_test.wv, and a rare example of the right way to test
  encoders!</li>
  
</ul>

<h4><a href="http://lame.sourceforge.net/download/samples">testsignal2.wv</a>
Subtle pre-echo test case. (0.3M - about 5 seconds)</h4>

<p>This is a very nice pre-echo test case from Jan Rafaj - rafaj at
cedric.vabo.cz. It has some clear, isolated drums. If your MP3 encoder does not
switch to short blocks at the precise moment, you will have very noticeable
pre-echo. The pre-echo actually sounds like a snare, but this snare is
completely artificial - there is no trace of it in the original uncompressed
file! ISO based encoders do very poorly, mostly because the short block
switching is completely broken in the psy model (even if it detects a pre-echo
event, it will switch to short blocks 1 granule too late). LAME 3.03 does
noticeably better, but it still uses the ISO pre-echo detection criterion, and
misses ,many of the pre-echo events. If you go into l3psy.c and set switch_pe =
1000 (instead of 1800), LAME will do much better, maybe 90% as good as FhG.</p>

<p>FhG does great. They seem to have excellent pre-echo detection. I would love
to know what their algorithm is based on.</p>

<p><i>Note 5/99:</i> LAME 3.05 has a much improved pre-echo detection algorithm,
and fixes some of the above problems!</p>

<p><i>Note 7/99:</i> LAME 3.16 has a better pre-echo detection, and allocates
more bits from the reservoir.</p>

<ul>

  <li><a href="ftp://cedric.vabo.cz/pub/linux/testfiles/mp3encoders/">Jan
  Rafaj's mp3 testcases</a></li>
  
</ul>
                                
<h4><a href="http://lame.sourceforge.net/download/samples">testsignal4.wv</a>
Subtle distortion case. (0.3M - about 6 seconds)</h4>

<p>Another difficult and subtle case from Jan Rafaj - rafaj at cedric.vabo.cz. I
believe this is by Enya. There is a slight trill as the volume increases. I can
barely hear it, but the FhG encoder manages to avoid it. Using mid/side masking
thresholds seems to help a lot (-h in LAME 3.21 and higher).</p>

<h2>Test cases previously used to improve LAME</h2>

<h4><a href="http://lame.sourceforge.net/download/samples">castanets.wv</a> FhG
pre-echo reference sample (0.5M - about 7 seconds)</h4>

<p>The castanets should sound like a sharp, crisp clack. In the ISO psy-model,
they are smeared out into long, soft thwack like sounds. GPSYCHO makes a
dramatic improvement in this, which is detectable on any sound system. This is
due to correctly switching to short blocks and encoding them with extra bits
from the reservoir. The attacks are very mono in nature, so jstereo also helps
because it allows even more bits for encoding the mid channel. The sample is
very close to mono, but if you really decimate the side it will results in
noticeable artifacts.</p>

<p>The FhG encoder does an even better job on this sample, mostly because it
detects some of the later castanets. They are muffled by other sounds and
GPSYCHO fails to recognize them as needing short blocks. Latter on in the
sample, the castanets come fast and furious, and even the FhG encoder can not
maintain enough bits in the bit reservoir. VBR would be great in this situation.
It is very easy to put into an encoder, but I don't have a player to debug it
with.</p>

<p>Normally you have to perform listening tests to determine the quality of an
mp3 encoding. You can not generally say anything about the quality by looking at
the original and encoded pcm signal. Pre-echo problems like in castanets.wv are
an exception to this. In a bad encoding, the sharp attack of the castanets will
create noise that is heard <b>before</b> the actual castanets. This flaw is very
visible in the encoded pcm signal, and is shown for several different encoders
in <a href="screenshots.php">screenshots.</a></p>
                                
<p>With the castents.wv file it's easy to try out new short block detection
schemes. You dont have to rely on listening tests since the pre-echo is so easy
to see in the output pcm data. Just modify the graphical interface display the
new criterion and then go through castanets.wv frame by frame and see if it is
triggered in the correct spots. For an interesting comparison, run lame with -g
(the graphical frame analyzer) on MP3 files produced by other encoders to see
how well they do.</p>
				
<h4><a href="http://lame.sourceforge.net/download/samples">track7.wv</a> Jazz
sample (0.3M - 4 seconds)</h4>
				
<p>Sent by Naoki SHIBATA - shibatch at geocities.co.jp, see <a
href="http://www.geocities.co.jp/Technopolis/9674/lametest/index.html">lametest</a>.
Naoki can hear noise in the left channel, but only when using a good pair of
headphones. It goes away when joint stereo is not used. Comparing with FhG, it
looks like LAME is toggling back and forth between MS and regular stereo when it
should not be. As of LAME 3.83: noise is almost gone.</p>
				
<h4><a href="http://lame.sourceforge.net/download/samples">Fools.wv</a> Good
range of effects (3.0M - about 30 seconds)</h4>
				
<p>I got this off an MP3 encoder comparison web site that later vanished. It is
a section from Lemon Tree by Fool's Garden. It was heavily used to tune the LAME
3.12 <a href="ms_stereo.php"> mid/side switch.</a>. I use a mono, downsampled
version for the current MPEG2 quality improvements.</p>
				
<h4><a href="http://lame.sourceforge.net/download/samples">main_theme.wv</a>
Strange artifact, mid/side stereo test. (0.9M - about 11 seconds)</h4>
				
<p>This sample is from an old Pink Floyd song. It was found by Robert Hegemann -
Robert.Hegemann at gmx.de. In the beginning, while the foreground pans from
right to left there is a slight twinkling sound. This goes away with -X, but the
true cause and a better fix should be found.</p>

<p><i>(NOTE 11/99:</i> This problem is much improved around lame 3.50)</p>

<p>It also contains a lot of distortion if mid/side stereo is used. The new
(lame3.12) mid/side switching algorithm solves this problem and can detect that
almost none of the frames should use mid/side stereo. The FhG also does not use
mid/side encoding for this sample.</p>
				
<h4><a href="http://lame.sourceforge.net/download/samples">mstest.wv</a>
Mide/Side stereo encoding test sample (0.3M - about 5 seconds)</h4>
				
<p>A good jstereo test case sent to me by Scott Miller - scgmille at
indiana.edu. It contains some higher frequency modes which are isolated to the
left channel. LAME sounds fine in Stereo mode (-m s), but using any type of
mid/side stereo will spread these modes to the right channel. Switching between
stereo and ms_stereo will result in the annoying effect of having them turn on
and off in the right channel. The FhG encoder avoids this problem by using very
few mid/side stereo frames. But the LAME mid/side stereo switching criterion can
not detect that this sample should not be encode with mid/side stereo, and
produces too many mid/side frames. Suggestions for a better switching criterion
are welcome! I've tried a few things, but anything that works is usually too
restrictive, i.e. it will turn off mid/side stereo for half the frames in
castanets.wv, but this sample should have all frames mid/side stereo.</p>

<p><i>NOTE 6/99:</i> This problem is fixed with new <a
href="ms_stereo.php">mid/side switch</a> added to LAME 3.12!</p>
                                
<h4><a href="http://lame.sourceforge.net/download/samples">t1.wav</a> Dire
Straits sample (1.4M - about 9 seconds)</h4>
                                
<p>This case has some subtle pre-echos that were missed by older versions of
LAME, and it greatly confused the old LAME mid/side stereo switching criterion.
It was found by Nils Faerber - Nils.Faerber at unix-ag.org. It was heavily used
to tune the LAME 3.12 <a href="ms_stereo.php">mid/side switch,</a> and for more
fine tuning of the pre-echo detection algorithm in LAME 3.15. Nils reports that
with LAME 3.12, the quality is now very close to the FhG encoder.</p>

<p>This sample was also used to tune the auto ATH adjustment introduced in
3.88.</p>
				
<h4><a href="http://lame.sourceforge.net/download/samples">else3.wv</a> Bit
allocation tests. (0.6M - about 6 seconds)</h4>
				
<p>A sample from Sarah McLachlan's "Elsewhere". I first checked out an MP3 of
this song from the Internet (a very high quality encoding). Later I bought the
CD and encoded it my self with an ISO based encoder, and was surprised at the
difference in quality. This is what motivated me to start looking at the encoder
source.</p>
                                 
<p>This song contains a lot of very tonal piano music for which even the ISO
encoder usually does ok. But in certain situations it produces very noticeable
distortion in the piano notes (Particularly in frames 50-70). GPSYCHO fixes this
mostly due to the improved outer_loop in the bit allocation subroutine. This
sample also has some attacks (drums) that are greatly improved with GPSYCHO. I
cannot detect a difference between GPSYCHO and FhG for this sample.</p>

                                 
<h2>Test Cases Where LAME beats FhG!</h2>

<h4><a href="http://lame.sourceforge.net/download/samples">KMFDM-Dogma.wv</a>
(0.6M - about 6 seconds)</h4>
                                
<p>Found by Kevin Burtch - kburtch at bellsouth.net.</p>
				
<h4><a href="http://lame.sourceforge.net/download/samples">iron.wv</a> (3.5M, 35
seconds)</h4>
				
<p>Found by Jee J C" - jeejc at hotmail.com. A short sample from the
Cardigans.</p>
				
<h4><a href="http://lame.sourceforge.net/download/samples">spahm.wv</a> (2.4M -
about 25 seconds.) Similar to fatboy.wv, but FhG does much worse for some
reason.</h4>
				
<p>From spahm at hotmail.com</p>
				
<h4><a href="http://lame.sourceforge.net/download/samples">BlackBird.wv</a>
(0.4M, about 6 seconds)</h4>
				
<p>From davel at caffeine.co.uk: The opening bars from the Beautiful South track
'Blackbird on the Wire' - you will notice it has some very sudden synthesized
drums in it. I encoded this track using both Lame and FhG, and found that FhG
just couldn't handle the first drum hit (it even had trouble with subsequent
ones which are less apparent on this particular example) and actually drops out
for a fraction of a second before the drum hit. I have tried many settings on
both decoders and Lame seems to handle it no matter what, whereas FhG always
trips up, even with the highest quality setting. Bitrate is immaterial.</p>

<h2>Other Test Cases</h2>
				
<h4><a href="http://lame.sourceforge.net/download/samples">60.wv</a> Simple two
tone test case usefull for debugging. (0.1M - 5 seconds)</h4>

<p>Sent by Naoki SHIBATA, shibatch at geocities.co.jp, see <a
href="http://www.geocities.co.jp/Technopolis/9674/lametest/index.html">lametest</a>.</p>

<h4><a href="http://lame.sourceforge.net/download/samples">fatboy.wv</a> All
encoders have trouble with this one. (0.4M - about 5 seconds)</h4>

<p>Found by Jake Hamby - jehamby at anobject.com</p>

<h4><a href="http://lame.sourceforge.net/download/samples">applaud.wv</a> (1.0M,
about 9 seconds)</h4>

<p>This is a very difficult test sample because of the lack on tonality and all
the sharp attacks. All encoders produce results noticeably different than the
original, but the FhG encoder still has the edge. The extra quality of the FhG
encoder is not due to simple fixes like better use of short blocks and the bit
reservoir. They do switch to ms_stereo, (and GPSYCHO does not), but forcing
GPSYCHO into ms_stereo doesn't improve things. Information on the applaud.wv
test sample can be found with <a href="http://geek.rcc.se/mp3encoder">Jan
Peman's applaus test case and results from many other encoders</a>.</p>
			

</div>
<?php include("footer.html") ?>
</div>

</body>
</html>