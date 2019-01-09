<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
    <title>LAME MP3 Encoder</title>
    <meta name="author" content="Roberto Amorim - rjamorim@yahoo.com" />
    <meta name="generator" content="jEdit 5.4" />
    <meta http-equiv="Content-Style-Type" content="text/css" />
    <meta http-equiv="content-type" content="text/html; charset=UTF-8" />
    <link rel="stylesheet" type="text/css" href="./styles/lame.css" />
    <!--[if IE 6]>
      <link rel="stylesheet" type="text/css" href="./styles/ie.css" />
    <![endif]-->
</head>
<body>
    <?php include("menu.html") ?>
    <div id="container">
        <div id="content">
            <div align="center">
            <img src="./images/logo.gif" width="358" height="231" alt="LAME Official logo"></img>
            <h1>LAME FAQ version 0.00002</h1></div>

            <p>
                Japanese version written by Takehiro TOMINAGA<br />
                And translated by Jadawin
            </p>

            <p>
                Note: This is very preliminary FAQ and far from perfect.
            </p>

            <h2>1. About LAME itself </h2>

            <h5>* What does the name "LAME" stand for ?</h5>

            <p>
                LAME: Lame Ain't MP3 Encoder; In another words, the progam that is not the MP3 encoder. To be more
                precise, it is the source code of the program that "may" encode/decode the MP3 data.
            </p>

            <h5>* Ooops! Is it delivered only in the source code form? I can't execute it.</h5>

            <p>
                That's right. Officially to say, LAME developping team delivers the source code only. But for the
                certain environment, you can fetch the binary from somewhere on the net.
            </p>

            <h5>* Then how can I get the source code?</h5>

            <p>
                From <a href="download.php">download page</a> on our website.
            </p>

            <h5>* How do you pronounce "LAME"? </h5>

            <p>
                If you prefer French, you can call it [l ae' m e : ]. If you prefer English, you can call it
                [l ei' m]. If you call it by French, that implies "things that glitter", "cheap" or something else.
                If you call it by English, it implies "defective", "inability", "bad sound", "badly made", "cutting
                corner" or other words.
            </p>

            <h5>* Who's developping the LAME</h5>

            <p>
                Many people in the whole world are the author at their leisure. If you count the people who had
                report a bug or wrote just a few lines of code, the number will be over 100.
            </p>

            <p>
                For more details, see <a href="developers.php">Who is The LAME Project</a> in our website.
            </p>

            <h5>* Which version of LAME should I use? There are many of them.</h5>

            <p>
                If you are new to use LAME, you should try the version that ends with '-Stable' at first time. The
                alpha or beta version are mainly for developpers. So, if you are not the person of the much curious
                and the great bravery, you'd better be off.
            </p>

            <h5>* Tell me the history of LAME. </h5>

            <p>
                The legendary person "Mike Chen" seemed to start the project. The rational and the initial intent
                of the project will be found at <a href="rationale.php">this page</a>.
            </p>

            <p>
                As seen in the document, LAME start with the aggregation of the codes from the encoders around,
                such as 8Hz, and made the independent improvement of them. At first, it was delivered as the patch
                of ISO code so as to avoid the patent issues.
            </p>

            <p>
                As the project making the progress, the codes was rewritten from scratch and the patch size was too
                large for handling with ease. Furthermore, the patent problem seemed to be harmless, if we distribute
                it in source form. So, we decided to distribute the source code.
            </p>

            <p>
                But still we DO NOT distribute binary files. All the binaries you can find are the UNOFFICIAL one.
            </p>

            <h5>* I heard LAME is licensed under GNU-LGPL. Is it right ? </h5>

            <p>
                Yes.
            </p>

            <h5>* Can I use LAME in my commercial program ? </h5>

            <p>
                Yes. See <a href="license.txt">this license text</a> for more details.
            </p>

            <h5>* Why do many people say "Use LAME!"? </h5>

            <p>
                Hmmm. How do you think?
            </p>

            <h5>* How is the sound quality of the LAME, in frank words? </h5>



            <h2>2. About MP3 format </h2>

            <h5>* What is MP3? </h5>

            <p>
                In the standards MPEG1, MPEG2(that are the standards of movie), ISO has adopt the audio compression
                technique which is refered to as "layer 3". In the broader sense, the independent extension technique
                called "MPEG2.5 Layer 3" that is developped by the German company "Fraunhofer" could be included.
            </p>

            <h5>* What is "ISO" or "MPEG"? </h5>

            <p>
                ISO is the standardization body of the international standard, and the MPEG is the working group
                that is dedicated to the movie or audio codec standard.
            </p>

            <h5>* What is the MPEG2.5? </h5>

            <h5>* How can I get the MP3 specification? </h5>

            <h5>* Why the quality of the MP3 is so different from the files? </h5>

            <p>
                f you encode the PCM audio into the MP3, many paramaters, such as sampling rate, bit rates and
                etc. will affect the quality of the output.
            </p>

            <p>
                Even if the paramaters were the same, the sound output of the
            </p>

            <h5>* Why different encoder/decoder makes sounds different? </h5>

            <h5>* I have heard multi-channels are supported with MP3...? </h5>

            <p>
                Sure there are. But no encoders/decoders support the multi-channel format, nor do by LAME.
            </p>

            <h5>* What is the "Free format" of the MP3? </h5>

            <p>
                The specifications that enables to use the NON-STANDARD bit rates. As this specification is optional,
                not all of the MP3 decoders can decode it.
            </p>

            <h5>* What is VBR? </h5>

            <h5>* What is the sfb21 problem? </h5>

            <h5>* In the encoded data, the sound over the 16KHz seemed to be cut. Why? </h5>

            <h2>3. About the usage of LAME </h2>

            <h5>* How to use for quick start? </h5>

            <h5>* Why are there so many options? </h5>

            <h5>* What options do we need? </h5>

            <h5>* I want to encode many files at once. </h5>

            <h5>* The encoding is so slow. Do you any clue? </h5>

            <h5>* Does LAME decode the MP3? </h5>

            <p>
                Absolutely yes. Try to use '--decode' and other options.
            </p>

            <h2>4. Hey, Is it a BUG???</h2>

            <h5>* I can't even start-up LAME at all? What the heck!?? </h5>

            <p>
                Calm down, please. Some of the machines that is installed the VIA or Cyrix CPU may have this
                trouble. Try to use '--noasm' option.
            </p>

            <p>
                Because some CPU have trouble on executing the CPUID command. Please, yell to the CPU/BIOS maker. :)
            </p>

            <h5>* The message "Internal buffer inconsistency. flushbits &lt;&gt; ResvSize" was showed in so many
                times. And the result of the encode was very noisy. What's wrong?</h5>

            <p>
                If you use the stable verion of LAME, it almost seemed to be the hardware trouble. Please check the
                temparature of the CPU or memory. And if you are over-clocking, try to resume the normal-clock.
            </p>

            <p>
                Even if your machine works with the other benchmarks, LAME could reveal the weak point of your
                system. The cause of trouble was assumed to be LAME's nature to use the whole functionality of the CPU,
                such as the usage of MMX and SSE or access to the external bus, the actual reason is UNKNOWN.
            </p>

            <p>
                After you have checked the hardware and found the LAME strange behavior, it could be the BUG of
                LAME. Please report the BUG.
            </p>

            <h5>* Then, how can I report BUG? </h5>

            <p>
                See <a href="contact.php">Contact page</a>.
            </p>

            <h5>* Eh!? I can't be such a GURU. How do I? </h5>

            <p>
                Hmmm. How can we?
            </p>

            <h5>* Why is this FAQ so useless? </h5>

            <p>
                Because we don't have staff to write documents. If you help us to write documents, we will
                appreciate that.
            </p>

            <h5>* After encoding the audio into MP3, the silent times are added to the start and the end of
                the data. Why?  </h5>

            <p>
                That could be the right result. (SEE. What is gap-less mode?)
            </p>

            <h5>* The encoded files sounds worse than original. </h5>

            <p>
                MP3 is not reversible. So it is. When you encode the audio into MP3 and decode that, you will get
                the different result from original if it is not the silent.
            </p>

            <p>
                And in the 99.9% case, the sound quality will be changed to worse.
            </p>

            <h2>5. About Audio Compression </h2>

            <h5>* What is pre-echo? </h5>

            <h5>* Why LAME uses low-pass filter? </h5>

            <h5>* How is the sound quality if I disabled the low-pass filter? </h5>

            <h5>* What is the CLIPPING problem? </h5>

            <h5>* The quality of compression and how to check it? </h5>

            <p>
                You can check the compression quality with the sample audio(Killer
                Sample) files that are known to be hard to compress.
            </p>

            <h5>* How to get "Killer Samples"? </h5>

            <h2>6. The issues that is specific to the playing environment </h2>

            <h5>* My DVD player does not understand the MP3 CD-ROM. </h5>

            <h5>* WMP6.4 does not play MP3 files. </h5>

            <h2>7. ETC. </h2>

            <h5>* What means by "Strictly comformant to the ISO standard." </h5>

            <h5>* TAG topics </h5>

            <h5>* Issues on the availble bit rates </h5>

        </div>
    <?php include("footer.html") ?>
    </div>
</body>
</html>