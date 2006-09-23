<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
     "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>
<head>
  <title>LAME MP3 Encoder :: GPSYCHO - Variable Bit Rate</title>
  <meta name="author" content="Roberto Amorim - roberto@rjamorim.com" />
  <meta name="generator" content="jEdit 4.2" />
  <meta name="cvs-version" content="$Id: vbr.php,v 1.2 2006-09-23 18:44:37 kylev Exp $" />
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
  <h1>GPSYCHO - Variable Bit Rate</h1>
</div>

<p>Suggested usage:</p>

<blockquote class="code">lame -v -V 2 -b 128  input.wav output.mp3</blockquote>

<p>VBR mode automatically uses the highest quality option.  So both
"-v" and "-h" are not necessary when using -V.  Options:</p>

<dl>

  <dt>-V n (where n=0..9)</dt>
  <dd>0 = highest quality<br />9 = lowest quality</dd>

  <dt>-b &lt;minimum allowed bitrate&gt;</dt>

  <dt>-B &lt;maximum allowed bitrate&gt;</dt>

</dl>

<p>Using -B with other than 320kbs is not recommended, since even a
128kbs CBR stream will sometimes use frames as large as 320kbs
via the bitreservoir.</p>

<p>Variables used in VBR code description:</p>

<dl>

  <dt><code>sfb</code></dt>

  <dd>Scale factor band index.</dd>

  <dt><code>thm[sfb]</code></dt>

  <dd>Allowed masking. <code>thm[sfb]</code> = How much noise is allowed in the
  sfb'th band, as computed by the GPSYCHO.</dd>

  <dt><code>gain[sfb]</code></dt>

  <dd>MDCT coefficents are scaled by 2^(-.25*gain) before quantizing. Smaller
  values of gain (more negative) mean that more bits are required to encode the
  coefficients, but the quantization noise will be (usually) smaller.</dd>

  <dt><code>desired_gain[sfb]</code></dt>

  <dd>The amount of gain needed so that if gain[sfb] &lt;= desired_gain[sfb],
  the quantization noise will be &lt;= thm[sfb].</dd>

</dl>

<p>An MP3 can use the following variables to achieve a given gain[sfb]. For
longblocks:</p>

<blockquote class="code">gain[sfb][i] = 2^ [ -.25 * ( global_gain -210 - ifqstep*scalefac[gr][ch].l[sfb] - ifqstep*pretab[sfb]) ]</blockquote>

<p>For shortblocks (i=0..2 for the three short blocks):</p>

<blockquote class="code">gain[sfb][i] =   2^  [ -.25*(  global_gain -210  -  8*subblock_gain[i]  - ifqstep*scalefac.s[sfb][i])   ]</blockquote>

<p>In both of the above cases, calculate <code>ifqstep</code>:</p>

<blockquote class="code">ifqstep =  scalefac_scale==0 ?  2 : 4;</blockquote>

<h3>Algorithm</h3>

<p>The VBR algorithm is as follows.</p>

<dl>

  <dt>Step 1: <code>psymodel.c</code></dt>

  <dd>Computes the allowed maskings, thm[sfb] thm[sfb] may be reduced by a few
  db depending on the quality setting. The smaller thm[sfb], the more bits will
  be required to encode the frame.</dd>

  <dt>Step 2: <code>find_scalefac()</code> in <code>vbrquantize.c</code></dt>

  <dd>Compute <code>desired_gain[sfb]</code> by iterating over the values of
  <code>sfb</code> from 0 to <code>SBMAX</code>. At each value, compute
  desired_gain[sfb] using a divide and conquer iteration so that
  <code>quantization_noise[sfb] &lt; thm[sfb]</code> . This requires 7
  iterations of <code>calc_sfb_noise()</code> which computes quantization error
  for the specified gain. This is the only time VBR needs to do any (expensive)
  quantization!</dd>

  <dt>Step 3: <code>VBR_noise_shaping()</code> in vbrquantize.c</dt>

  <dd>Find a combination of global_gain, subblock_gain, preflag, scalefac_scale,
  etc... so that: <code>gain[sfb] &lt;= desired_gain[sfb]</code></dd>


  <dt>Step 4: <code>VBR_quantize_granule()</code> in
  <code>vbrquantize.c</code></dt>

  <dd>Calculate the number of bits needed to encode the frame with
           the values computed in step 3.  Unlike CBR, VBR (usually) only
           has to do this expensive huffman bit counting stuff once!</dd>


  <dt>Step 5: <code>VBR_noise_shaping()</code> in <code>vbrquantize.c</code></dt>

  <dd>if bits &lt; minimum_bits: Repeat step 3, only with a larger value of
  global_gain. (but allow bits &lt; minimum_bits for analog silence)<br />

  if bits > maximum_bits: decrease global_gain, keeping all other scalefactors
  the same.<br />

  Usually step 5 is not necessary.</dd>

  <dt>step 6: VBR_quantize() in vbrquantize.c</dt>

  <dd>After encoding both channels and granules, check to make sure that the
  total number of bits for the whole frame does not exceed the maximum allowed.
  If it does, lower the quality and repeat steps 2,3 and 4 for the granules that
  were using lots of bits.</dd>

</dl>

<h3>Flow</h3>

<p>The actual flow chart looks something like this:</p>

<dl>

  <dt>VBR_quantize()</dt>

  <dd class="code">determine minbits, maxbits for each granule
determine max_frame_bits
adjust global quality setting based on VBR_q

do
   frame_bits=0

   loop over each channel, granule:
       compute thm[sfb]
       bits = VBR_noise_shaping():  Encodes each granule with minbits &lt;= bits &lt;= maxbits
       frame_bits += bits

   lower the global quality setting

while (frame_bits &gt; max_frame_bits)</dd>

  <dt>VBR_noise_shaping()</dt>

  <dd class="code">find_scalefac()   (computes desired_gain)
Estimate largest possible value of global_gain

do
    compute_scalefac_long/short()
    scalefacts, etc. so that gain &lt;= desired_gain)

    bits = VBR_quantize_granule()

    if (bits &lt; minbits &amp;&amp; analog silence) break;
    if (bits &gt;= minbits) break;

    decrease global_gain (which increases number of bits used)

while 1

if bits &gt; maxbits
    do
        increase global_gain
        bits = VBR_quantize_granule()
    while (bits &gt; maxbits)</dd>


  <dt><code>find_scalefac()</code></dt>

  <dd>Simple divide and conquer iteration which repeatidly calls
  calc_sfb_noise() with different values of desired_gain until it finds the
  largest desired_gain such that the quantization_noise &lt; allowed masking
  Requires 7 iterations.</dd>

</dl>

</div>
<?php include("footer.html") ?>
</div>

</body>
</html>