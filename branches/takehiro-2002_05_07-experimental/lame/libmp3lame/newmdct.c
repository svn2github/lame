/*
 *	MP3 window subband -> subband filtering -> mdct routine
 *
 *	Copyright (c) 1999 Takehiro TOMINAGA
 *
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 *         Special Thanks to Patrick De Smet for your advices.
 */

/* $Id$ */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#ifdef WITH_DMALLOC
# include <dmalloc.h>
#endif

#include "encoder.h"
#include "tables.h"
#include "newmdct.h"

static const FLOAT enwindow[] = 
{
      -4.77e-07*0.740951125354959/2.384e-06,  1.03951e-04*0.740951125354959/2.384e-06,  9.53674e-04*0.740951125354959/2.384e-06, 2.841473e-03*0.740951125354959/2.384e-06,
  3.5758972e-02*0.740951125354959/2.384e-06, 3.401756e-03*0.740951125354959/2.384e-06,  9.83715e-04*0.740951125354959/2.384e-06,   9.9182e-05*0.740951125354959/2.384e-06, /* 15*/
     1.2398e-05*0.740951125354959/2.384e-06,  1.91212e-04*0.740951125354959/2.384e-06, 2.283096e-03*0.740951125354959/2.384e-06,1.6994476e-02*0.740951125354959/2.384e-06,
 -1.8756866e-02*0.740951125354959/2.384e-06,-2.630711e-03*0.740951125354959/2.384e-06, -2.47478e-04*0.740951125354959/2.384e-06,  -1.4782e-05*0.740951125354959/2.384e-06,
      9.063471690191471e-01,
      1.960342806591213e-01,


      -4.77e-07*0.773010453362737/2.384e-06,  1.05858e-04*0.773010453362737/2.384e-06,  9.30786e-04*0.773010453362737/2.384e-06, 2.521515e-03*0.773010453362737/2.384e-06,
  3.5694122e-02*0.773010453362737/2.384e-06, 3.643036e-03*0.773010453362737/2.384e-06,  9.91821e-04*0.773010453362737/2.384e-06,   9.6321e-05*0.773010453362737/2.384e-06, /* 14*/
     1.1444e-05*0.773010453362737/2.384e-06,  1.65462e-04*0.773010453362737/2.384e-06, 2.110004e-03*0.773010453362737/2.384e-06,1.6112804e-02*0.773010453362737/2.384e-06,
 -1.9634247e-02*0.773010453362737/2.384e-06,-2.803326e-03*0.773010453362737/2.384e-06, -2.77042e-04*0.773010453362737/2.384e-06,  -1.6689e-05*0.773010453362737/2.384e-06,
      8.206787908286602e-01,
      3.901806440322567e-01,


      -4.77e-07*0.803207531480645/2.384e-06,  1.07288e-04*0.803207531480645/2.384e-06,  9.02653e-04*0.803207531480645/2.384e-06, 2.174854e-03*0.803207531480645/2.384e-06,
  3.5586357e-02*0.803207531480645/2.384e-06, 3.858566e-03*0.803207531480645/2.384e-06,  9.95159e-04*0.803207531480645/2.384e-06,   9.3460e-05*0.803207531480645/2.384e-06, /* 13*/
     1.0014e-05*0.803207531480645/2.384e-06,  1.40190e-04*0.803207531480645/2.384e-06, 1.937389e-03*0.803207531480645/2.384e-06,1.5233517e-02*0.803207531480645/2.384e-06,
 -2.0506859e-02*0.803207531480645/2.384e-06,-2.974033e-03*0.803207531480645/2.384e-06, -3.07560e-04*0.803207531480645/2.384e-06,  -1.8120e-05*0.803207531480645/2.384e-06,
      7.416505462720353e-01,
      5.805693545089249e-01,


      -4.77e-07*0.831469612302545/2.384e-06,  1.08242e-04*0.831469612302545/2.384e-06,  8.68797e-04*0.831469612302545/2.384e-06, 1.800537e-03*0.831469612302545/2.384e-06,
  3.5435200e-02*0.831469612302545/2.384e-06, 4.049301e-03*0.831469612302545/2.384e-06,  9.94205e-04*0.831469612302545/2.384e-06,   9.0599e-05*0.831469612302545/2.384e-06, /* 12*/
      9.060e-06*0.831469612302545/2.384e-06,  1.16348e-04*0.831469612302545/2.384e-06, 1.766682e-03*0.831469612302545/2.384e-06,1.4358521e-02*0.831469612302545/2.384e-06,
 -2.1372318e-02*0.831469612302545/2.384e-06, -3.14188e-03*0.831469612302545/2.384e-06, -3.39031e-04*0.831469612302545/2.384e-06,  -1.9550e-05*0.831469612302545/2.384e-06,
      6.681786379192989e-01,
      7.653668647301797e-01,


      -4.77e-07*0.857728610000272/2.384e-06,  1.08719e-04*0.857728610000272/2.384e-06,  8.29220e-04*0.857728610000272/2.384e-06, 1.399517e-03*0.857728610000272/2.384e-06,
  3.5242081e-02*0.857728610000272/2.384e-06, 4.215240e-03*0.857728610000272/2.384e-06,  9.89437e-04*0.857728610000272/2.384e-06,   8.7261e-05*0.857728610000272/2.384e-06, /* 11*/
      8.106e-06*0.857728610000272/2.384e-06,   9.3937e-05*0.857728610000272/2.384e-06, 1.597881e-03*0.857728610000272/2.384e-06,1.3489246e-02*0.857728610000272/2.384e-06,
 -2.2228718e-02*0.857728610000272/2.384e-06,-3.306866e-03*0.857728610000272/2.384e-06, -3.71456e-04*0.857728610000272/2.384e-06,  -2.1458e-05*0.857728610000272/2.384e-06,
      5.993769336819237e-01,
      9.427934736519954e-01,


      -4.77e-07*0.881921264348355/2.384e-06,  1.08719e-04*0.881921264348355/2.384e-06,   7.8392e-04*0.881921264348355/2.384e-06,  9.71317e-04*0.881921264348355/2.384e-06,
  3.5007000e-02*0.881921264348355/2.384e-06, 4.357815e-03*0.881921264348355/2.384e-06,  9.80854e-04*0.881921264348355/2.384e-06,   8.3923e-05*0.881921264348355/2.384e-06, /* 10*/
      7.629e-06*0.881921264348355/2.384e-06,   7.2956e-05*0.881921264348355/2.384e-06, 1.432419e-03*0.881921264348355/2.384e-06,1.2627602e-02*0.881921264348355/2.384e-06,
 -2.3074150e-02*0.881921264348355/2.384e-06,-3.467083e-03*0.881921264348355/2.384e-06, -4.04358e-04*0.881921264348355/2.384e-06,  -2.3365e-05*0.881921264348355/2.384e-06,
      5.345111359507916e-01,
      1.111140466039205e+00,


      -9.54e-07*0.903989293123443/2.384e-06,  1.08242e-04*0.903989293123443/2.384e-06,  7.31945e-04*0.903989293123443/2.384e-06,  5.15938e-04*0.903989293123443/2.384e-06,
  3.4730434e-02*0.903989293123443/2.384e-06, 4.477024e-03*0.903989293123443/2.384e-06,  9.68933e-04*0.903989293123443/2.384e-06,   8.0585e-05*0.903989293123443/2.384e-06, /* 9*/
      6.676e-06*0.903989293123443/2.384e-06,   5.2929e-05*0.903989293123443/2.384e-06, 1.269817e-03*0.903989293123443/2.384e-06,1.1775017e-02*0.903989293123443/2.384e-06,
 -2.3907185e-02*0.903989293123443/2.384e-06,-3.622532e-03*0.903989293123443/2.384e-06, -4.38213e-04*0.903989293123443/2.384e-06,  -2.5272e-05*0.903989293123443/2.384e-06,
      4.729647758913199e-01,
      1.268786568327291e+00,


      -9.54e-07*0.92387953251128675613/2.384e-06,  1.06812e-04*0.92387953251128675613/2.384e-06,  6.74248e-04*0.92387953251128675613/2.384e-06,   3.3379e-05*0.92387953251128675613/2.384e-06,
  3.4412861e-02*0.92387953251128675613/2.384e-06, 4.573822e-03*0.92387953251128675613/2.384e-06,  9.54151e-04*0.92387953251128675613/2.384e-06,   7.6771e-05*0.92387953251128675613/2.384e-06,
      6.199e-06*0.92387953251128675613/2.384e-06,   3.4332e-05*0.92387953251128675613/2.384e-06, 1.111031e-03*0.92387953251128675613/2.384e-06,1.0933399e-02*0.92387953251128675613/2.384e-06,
 -2.4725437e-02*0.92387953251128675613/2.384e-06,-3.771782e-03*0.92387953251128675613/2.384e-06, -4.72546e-04*0.92387953251128675613/2.384e-06,  -2.7657e-05*0.92387953251128675613/2.384e-06,
      4.1421356237309504879e-01,  /* tan(PI/8) */
      1.414213562373095e+00,


      -9.54e-07*0.941544065183021/2.384e-06,  1.05381e-04*0.941544065183021/2.384e-06,  6.10352e-04*0.941544065183021/2.384e-06, -4.75883e-04*0.941544065183021/2.384e-06,
  3.4055710e-02*0.941544065183021/2.384e-06, 4.649162e-03*0.941544065183021/2.384e-06,  9.35555e-04*0.941544065183021/2.384e-06,   7.3433e-05*0.941544065183021/2.384e-06, /* 7*/
      5.245e-06*0.941544065183021/2.384e-06,   1.7166e-05*0.941544065183021/2.384e-06,  9.56535e-04*0.941544065183021/2.384e-06,1.0103703e-02*0.941544065183021/2.384e-06,
 -2.5527000e-02*0.941544065183021/2.384e-06,-3.914356e-03*0.941544065183021/2.384e-06, -5.07355e-04*0.941544065183021/2.384e-06,  -3.0041e-05*0.941544065183021/2.384e-06,
      3.578057213145241e-01,
      1.546020906725474e+00,


      -9.54e-07*0.956940335732209/2.384e-06,  1.02520e-04*0.956940335732209/2.384e-06,  5.39303e-04*0.956940335732209/2.384e-06,-1.011848e-03*0.956940335732209/2.384e-06,
  3.3659935e-02*0.956940335732209/2.384e-06, 4.703045e-03*0.956940335732209/2.384e-06,  9.15051e-04*0.956940335732209/2.384e-06,   7.0095e-05*0.956940335732209/2.384e-06, /* 6*/
      4.768e-06*0.956940335732209/2.384e-06,     9.54e-07*0.956940335732209/2.384e-06,  8.06808e-04*0.956940335732209/2.384e-06, 9.287834e-03*0.956940335732209/2.384e-06,
 -2.6310921e-02*0.956940335732209/2.384e-06,-4.048824e-03*0.956940335732209/2.384e-06, -5.42164e-04*0.956940335732209/2.384e-06,  -3.2425e-05*0.956940335732209/2.384e-06,
      3.033466836073424e-01,
      1.662939224605090e+00,


     -1.431e-06*0.970031253194544/2.384e-06,   9.9182e-05*0.970031253194544/2.384e-06,  4.62532e-04*0.970031253194544/2.384e-06,-1.573563e-03*0.970031253194544/2.384e-06,
  3.3225536e-02*0.970031253194544/2.384e-06, 4.737377e-03*0.970031253194544/2.384e-06,  8.91685e-04*0.970031253194544/2.384e-06,   6.6280e-05*0.970031253194544/2.384e-06,  /* 5*/
      4.292e-06*0.970031253194544/2.384e-06,  -1.3828e-05*0.970031253194544/2.384e-06,  6.61850e-04*0.970031253194544/2.384e-06, 8.487225e-03*0.970031253194544/2.384e-06,
 -2.7073860e-02*0.970031253194544/2.384e-06,-4.174709e-03*0.970031253194544/2.384e-06, -5.76973e-04*0.970031253194544/2.384e-06,  -3.4809e-05*0.970031253194544/2.384e-06,
      2.504869601913055e-01,
      1.763842528696710e+00,


     -1.431e-06*0.98078528040323/2.384e-06,   9.5367e-05*0.98078528040323/2.384e-06,  3.78609e-04*0.98078528040323/2.384e-06,-2.161503e-03*0.98078528040323/2.384e-06,
  3.2754898e-02*0.98078528040323/2.384e-06, 4.752159e-03*0.98078528040323/2.384e-06,  8.66413e-04*0.98078528040323/2.384e-06,   6.2943e-05*0.98078528040323/2.384e-06, /* 4*/
      3.815e-06*0.98078528040323/2.384e-06,   -2.718e-05*0.98078528040323/2.384e-06,  5.22137e-04*0.98078528040323/2.384e-06, 7.703304e-03*0.98078528040323/2.384e-06,
 -2.7815342e-02*0.98078528040323/2.384e-06,-4.290581e-03*0.98078528040323/2.384e-06, -6.11782e-04*0.98078528040323/2.384e-06,  -3.7670e-05*0.98078528040323/2.384e-06,
      1.989123673796580e-01,
      1.847759065022573e+00,


     -1.907e-06*0.989176509964781/2.384e-06,   9.0122e-05*0.989176509964781/2.384e-06,  2.88486e-04*0.989176509964781/2.384e-06,-2.774239e-03*0.989176509964781/2.384e-06,
  3.2248020e-02*0.989176509964781/2.384e-06, 4.748821e-03*0.989176509964781/2.384e-06,  8.38757e-04*0.989176509964781/2.384e-06,   5.9605e-05*0.989176509964781/2.384e-06, /* 3*/
      3.338e-06*0.989176509964781/2.384e-06,  -3.9577e-05*0.989176509964781/2.384e-06,  3.88145e-04*0.989176509964781/2.384e-06, 6.937027e-03*0.989176509964781/2.384e-06,
 -2.8532982e-02*0.989176509964781/2.384e-06,-4.395962e-03*0.989176509964781/2.384e-06, -6.46591e-04*0.989176509964781/2.384e-06,  -4.0531e-05*0.989176509964781/2.384e-06,
      1.483359875383474e-01,
      1.913880671464418e+00,


     -1.907e-06*0.995184726672197/2.384e-06,   8.4400e-05*0.995184726672197/2.384e-06,  1.91689e-04*0.995184726672197/2.384e-06,-3.411293e-03*0.995184726672197/2.384e-06,
  3.1706810e-02*0.995184726672197/2.384e-06, 4.728317e-03*0.995184726672197/2.384e-06,  8.09669e-04*0.995184726672197/2.384e-06,    5.579e-05*0.995184726672197/2.384e-06,
      3.338e-06*0.995184726672197/2.384e-06,  -5.0545e-05*0.995184726672197/2.384e-06,  2.59876e-04*0.995184726672197/2.384e-06, 6.189346e-03*0.995184726672197/2.384e-06,
 -2.9224873e-02*0.995184726672197/2.384e-06,-4.489899e-03*0.995184726672197/2.384e-06, -6.80923e-04*0.995184726672197/2.384e-06,  -4.3392e-05*0.995184726672197/2.384e-06,
      9.849140335716425e-02,
      1.961570560806461e+00,


     -2.384e-06*0.998795456205172/2.384e-06,   7.7724e-05*0.998795456205172/2.384e-06,   8.8215e-05*0.998795456205172/2.384e-06,-4.072189e-03*0.998795456205172/2.384e-06,
      3.1132698e-02*0.998795456205172/2.384e-06, 4.691124e-03*0.998795456205172/2.384e-06,  7.79152e-04*0.998795456205172/2.384e-06,   5.2929e-05*0.998795456205172/2.384e-06,
      2.861e-06*0.998795456205172/2.384e-06,  -6.0558e-05*0.998795456205172/2.384e-06,  1.37329e-04*0.998795456205172/2.384e-06, 5.462170e-03*0.998795456205172/2.384e-06,
      -2.9890060e-02*0.998795456205172/2.384e-06,-4.570484e-03*0.998795456205172/2.384e-06, -7.14302e-04*0.998795456205172/2.384e-06,  -4.6253e-05*0.998795456205172/2.384e-06,
      4.912684976946725e-02,
      1.990369453344394e+00,


      3.5780907e-02 * SQRT2*0.5/2.384e-06,1.7876148e-02 * SQRT2*0.5/2.384e-06, 3.134727e-03 * SQRT2*0.5/2.384e-06, 2.457142e-03 * SQRT2*0.5/2.384e-06,
      9.71317e-04 * SQRT2*0.5/2.384e-06,  2.18868e-04 * SQRT2*0.5/2.384e-06,  1.01566e-04 * SQRT2*0.5/2.384e-06,   1.3828e-05 * SQRT2*0.5/2.384e-06,

      3.0526638e-02/2.384e-06, 4.638195e-03/2.384e-06,  7.47204e-04/2.384e-06,   4.9591e-05/2.384e-06,
      4.756451e-03/2.384e-06,   2.1458e-05/2.384e-06,  -6.9618e-05/2.384e-06,/*    2.384e-06/2.384e-06*/
};


#define NS 12
#define NL 36

static const FLOAT win[4][NL] = {
  {
    2.382191739347913e-13,
    6.423305872147834e-13,
    9.400849094049688e-13,
    1.122435026096556e-12,
    1.183840321267481e-12,
    1.122435026096556e-12,
    9.400849094049690e-13,
    6.423305872147839e-13,
    2.382191739347918e-13,

    5.456116108943412e-12,
    4.878985199565852e-12,
    4.240448995017367e-12,
    3.559909094758252e-12,
    2.858043359288075e-12,
    2.156177623817898e-12,
    1.475637723558783e-12,
    8.371015190102974e-13,
    2.599706096327376e-13,

    -5.456116108943412e-12,
    -4.878985199565852e-12,
    -4.240448995017367e-12,
    -3.559909094758252e-12,
    -2.858043359288076e-12,
    -2.156177623817898e-12,
    -1.475637723558783e-12,
    -8.371015190102975e-13,
    -2.599706096327376e-13,

    -2.382191739347923e-13,
    -6.423305872147843e-13,
    -9.400849094049696e-13,
    -1.122435026096556e-12,
    -1.183840321267481e-12,
    -1.122435026096556e-12,
    -9.400849094049694e-13,
    -6.423305872147840e-13,
    -2.382191739347918e-13,
  },
  {
    2.382191739347913e-13,
    6.423305872147834e-13,
    9.400849094049688e-13,
    1.122435026096556e-12,
    1.183840321267481e-12,
    1.122435026096556e-12,
    9.400849094049688e-13,
    6.423305872147841e-13,
    2.382191739347918e-13,

    5.456116108943413e-12,
    4.878985199565852e-12,
    4.240448995017367e-12,
    3.559909094758253e-12,
    2.858043359288075e-12,
    2.156177623817898e-12,
    1.475637723558782e-12,
    8.371015190102975e-13,
    2.599706096327376e-13,

    -5.461314069809755e-12,
    -4.921085770524055e-12,
    -4.343405037091838e-12,
    -3.732668368707687e-12,
    -3.093523840190885e-12,
    -2.430835727329465e-12,
    -1.734679010007751e-12,
    -9.748253656609281e-13,
    -2.797435120168326e-13,

    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    -2.283748241799531e-13,
    -4.037858874020686e-13,
    -2.146547464825323e-13,
  },
  {
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    0.000000000000000e+00,
    2.283748241799531e-13,
    4.037858874020686e-13,
    2.146547464825323e-13,

    5.461314069809755e-12,
    4.921085770524055e-12,
    4.343405037091838e-12,
    3.732668368707687e-12,
    3.093523840190885e-12,
    2.430835727329466e-12,
    1.734679010007751e-12,
    9.748253656609281e-13,
    2.797435120168326e-13,

    -5.456116108943413e-12,
    -4.878985199565852e-12,
    -4.240448995017367e-12,
    -3.559909094758253e-12,
    -2.858043359288075e-12,
    -2.156177623817898e-12,
    -1.475637723558782e-12,
    -8.371015190102975e-13,
    -2.599706096327376e-13,

    -2.382191739347913e-13,
    -6.423305872147834e-13,
    -9.400849094049688e-13,
    -1.122435026096556e-12,
    -1.183840321267481e-12,
    -1.122435026096556e-12,
    -9.400849094049688e-13,
    -6.423305872147841e-13,
    -2.382191739347918e-13,
  },
  {
    1.316524975873958e-01, /* win[SHORT_TYPE] */
    4.142135623730950e-01,
    7.673269879789602e-01,

    1.091308501069271e+00, /* tantab_l */
    1.303225372841206e+00,
    1.569685577117490e+00,
    1.920982126971166e+00,
    2.414213562373094e+00,
    3.171594802363212e+00,
    4.510708503662055e+00,
    7.595754112725146e+00,
    2.290376554843115e+01,

    0.98480775301220802032, /* cx */
    0.64278760968653936292,
    0.34202014332566882393,
    0.93969262078590842791,
    -0.17364817766693030343,
    -0.76604444311897790243,
    0.86602540378443870761,
    0.500000000000000e+00,

    -5.144957554275265e-01, /* ca */
    -4.717319685649723e-01,
    -3.133774542039019e-01,
    -1.819131996109812e-01,
    -9.457419252642064e-02,
    -4.096558288530405e-02,
    -1.419856857247115e-02,
    -3.699974673760037e-03,

     8.574929257125442e-01, /* cs */
     8.817419973177052e-01,
     9.496286491027329e-01,
     9.833145924917901e-01,
     9.955178160675857e-01,
     9.991605581781475e-01,
     9.998991952444470e-01,
     9.999931550702802e-01,
  }
};

#define tantab_l (win[SHORT_TYPE]+3)
#define cx (win[SHORT_TYPE]+12)
#define ca (win[SHORT_TYPE]+20)
#define cs (win[SHORT_TYPE]+28)

/************************************************************************
*
* window_subband()
*
* PURPOSE:  Overlapping window on PCM samples
*
* SEMANTICS:
* 32 16-bit pcm samples are scaled to fractional 2's complement and
* concatenated to the end of the window buffer #x#. The updated window
* buffer #x# is then windowed by the analysis window #c# to produce the
* windowed sample #z#
*
************************************************************************/

/*
 *      new IDCT routine written by Takehiro TOMINAGA
 */
/* returns sum_j=0^31 w[j]*a[j]*cos(PI*j*(k+1/2)/32), 0<=k<32 */
inline static void
window_subband(FLOAT coef[SBLIMIT], const sample_t *x1, FLOAT a[SBLIMIT])
{
    int i;
    FLOAT const *wp = enwindow+10;

    const sample_t *x2 = &x1[238-14-286];

    for (i = -15; i < 0; i++) {
	FLOAT w, s, t;

	w = wp[-10]; s = x2[-224] * w; t  = x1[ 224] * w;
	w = wp[-9]; s += x2[-160] * w; t += x1[ 160] * w;
	w = wp[-8]; s += x2[- 96] * w; t += x1[  96] * w;
	w = wp[-7]; s += x2[- 32] * w; t += x1[  32] * w;
	w = wp[-6]; s += x2[  32] * w; t += x1[- 32] * w;
	w = wp[-5]; s += x2[  96] * w; t += x1[- 96] * w;
	w = wp[-4]; s += x2[ 160] * w; t += x1[-160] * w;
	w = wp[-3]; s += x2[ 224] * w; t += x1[-224] * w;

	w = wp[-2]; s += x1[-256] * w; t -= x2[ 256] * w;
	w = wp[-1]; s += x1[-192] * w; t -= x2[ 192] * w;
	w = wp[ 0]; s += x1[-128] * w; t -= x2[ 128] * w;
	w = wp[ 1]; s += x1[- 64] * w; t -= x2[  64] * w;
	w = wp[ 2]; s += x1[   0] * w; t -= x2[   0] * w;
	w = wp[ 3]; s += x1[  64] * w; t -= x2[- 64] * w;
	w = wp[ 4]; s += x1[ 128] * w; t -= x2[-128] * w;
	w = wp[ 5]; s += x1[ 192] * w; t -= x2[-192] * w;

	/*
	 * this multiplyer could be removed, but it needs more 256 FLOAT data.
	 * thinking about the data cache performance, I think we should not
	 * use such a huge table. tt 2000/Oct/25
	 */
	s *= wp[6];
	w = t - s;
	a[30+i*2] = t + s;
	a[31+i*2] = wp[7] * w;
	wp += 18;
	x1--;
	x2++;
    }
    {
	FLOAT s,t,u,v;
	t  =  x1[- 16] * wp[-10];              s  = x1[ -32] * wp[-2];
	t += (x1[- 48] - x1[ 16]) * wp[-9];    s += x1[ -96] * wp[-1];
	t += (x1[- 80] + x1[ 48]) * wp[-8];    s += x1[-160] * wp[ 0];
	t += (x1[-112] - x1[ 80]) * wp[-7];    s += x1[-224] * wp[ 1];
	t += (x1[-144] + x1[112]) * wp[-6];    s -= x1[  32] * wp[ 2];
	t += (x1[-176] - x1[144]) * wp[-5];    s -= x1[  96] * wp[ 3];
	t += (x1[-208] + x1[176]) * wp[-4];    s -= x1[ 160] * wp[ 4];
	t += (x1[-240] - x1[208]) * wp[-3];    s -= x1[ 224];

	u = s - t;
	v = s + t;

	t = a[14];
	s = a[15] - t;

	a[31] = v + t;   /* A0 */
	a[30] = u + s;   /* A1 */
	a[15] = u - s;   /* A2 */
	a[14] = v - t;   /* A3 */
    }
{
    FLOAT xr;
    xr = a[28] - a[ 0]; a[ 0] += a[28]; a[28] = xr * wp[-2*18+7];
    xr = a[29] - a[ 1]; a[ 1] += a[29]; a[29] = xr * wp[-2*18+7];

    xr = a[26] - a[ 2]; a[ 2] += a[26]; a[26] = xr * wp[-4*18+7];
    xr = a[27] - a[ 3]; a[ 3] += a[27]; a[27] = xr * wp[-4*18+7];

    xr = a[24] - a[ 4]; a[ 4] += a[24]; a[24] = xr * wp[-6*18+7];
    xr = a[25] - a[ 5]; a[ 5] += a[25]; a[25] = xr * wp[-6*18+7];

    xr = a[22] - a[ 6]; a[ 6] += a[22]; a[22] = xr * SQRT2;
    xr = a[23] - a[ 7]; a[ 7] += a[23]; a[23] = xr * SQRT2 - a[ 7];
    a[ 7] -= a[ 6];
    a[22] -= a[ 7];
    a[23] -= a[22];

    xr = a[ 6]; a[ 6] = a[31] - xr; a[31] = a[31] + xr;
    xr = a[ 7]; a[ 7] = a[30] - xr; a[30] = a[30] + xr;
    xr = a[22]; a[22] = a[15] - xr; a[15] = a[15] + xr;
    xr = a[23]; a[23] = a[14] - xr; a[14] = a[14] + xr;

    xr = a[20] - a[ 8]; a[ 8] += a[20]; a[20] = xr * wp[-10*18+7];
    xr = a[21] - a[ 9]; a[ 9] += a[21]; a[21] = xr * wp[-10*18+7];

    xr = a[18] - a[10]; a[10] += a[18]; a[18] = xr * wp[-12*18+7];
    xr = a[19] - a[11]; a[11] += a[19]; a[19] = xr * wp[-12*18+7];

    xr = a[16] - a[12]; a[12] += a[16]; a[16] = xr * wp[-14*18+7];
    xr = a[17] - a[13]; a[13] += a[17]; a[17] = xr * wp[-14*18+7];

    xr = -a[20] + a[24]; a[20] += a[24]; a[24] = xr * wp[-12*18+7];
    xr = -a[21] + a[25]; a[21] += a[25]; a[25] = xr * wp[-12*18+7];

    xr = a[ 4] - a[ 8]; a[ 4] += a[ 8]; a[ 8] = xr * wp[-12*18+7];
    xr = a[ 5] - a[ 9]; a[ 5] += a[ 9]; a[ 9] = xr * wp[-12*18+7];

    xr = a[ 0] - a[12]; a[ 0] += a[12]; a[12] = xr * wp[-4*18+7];
    xr = a[ 1] - a[13]; a[ 1] += a[13]; a[13] = xr * wp[-4*18+7];
    xr = a[16] - a[28]; a[16] += a[28]; a[28] = xr * wp[-4*18+7];
    xr = -a[17] + a[29]; a[17] += a[29]; a[29] = xr * wp[-4*18+7];

    xr = SQRT2 * (a[ 2] - a[10]); a[ 2] += a[10]; a[10] = xr;
    xr = SQRT2 * (a[ 3] - a[11]); a[ 3] += a[11]; a[11] = xr;
    xr = SQRT2 * (-a[18] + a[26]); a[18] += a[26]; a[26] = xr - a[18];
    xr = SQRT2 * (-a[19] + a[27]); a[19] += a[27]; a[27] = xr - a[19];

    xr = a[ 2]; a[19] -= a[ 3]; a[ 3] -= xr; a[ 2] = a[31] - xr; a[31] += xr;
    xr = a[ 3]; a[11] -= a[19]; a[18] -= xr; a[ 3] = a[30] - xr; a[30] += xr;
    xr = a[18]; a[27] -= a[11]; a[19] -= xr; a[18] = a[15] - xr; a[15] += xr;

    xr = a[19]; a[10] -= xr; a[19] = a[14] - xr; a[14] += xr;
    xr = a[10]; a[11] -= xr; a[10] = a[23] - xr; a[23] += xr;
    xr = a[11]; a[26] -= xr; a[11] = a[22] - xr; a[22] += xr;
    xr = a[26]; a[27] -= xr; a[26] = a[ 7] - xr; a[ 7] += xr;

    xr = a[27]; a[27] = a[ 6] - xr; a[ 6] += xr;

    xr = SQRT2 * (a[ 0] - a[ 4]); a[ 0] += a[ 4]; a[ 4] = xr;
    xr = SQRT2 * (a[ 1] - a[ 5]); a[ 1] += a[ 5]; a[ 5] = xr;
    xr = SQRT2 * (a[16] - a[20]); a[16] += a[20]; a[20] = xr;
    xr = SQRT2 * (a[17] - a[21]); a[17] += a[21]; a[21] = xr;

    xr = -SQRT2 * (a[ 8] - a[12]); a[ 8] += a[12]; a[12] = xr - a[ 8];
    xr = -SQRT2 * (a[ 9] - a[13]); a[ 9] += a[13]; a[13] = xr - a[ 9];
    xr = -SQRT2 * (a[25] - a[29]); a[25] += a[29]; a[29] = xr - a[25];
    xr = -SQRT2 * (a[24] + a[28]); a[24] -= a[28]; a[28] = xr - a[24];

    xr = a[24] - a[16]; a[24] = xr;
    xr = a[20] - xr;    a[20] = xr;
    xr = a[28] - xr;    a[28] = xr;

    xr = a[25] - a[17]; a[25] = xr;
    xr = a[21] - xr;    a[21] = xr;
    xr = a[29] - xr;    a[29] = xr;

    xr = a[17] - a[ 1]; a[17] = xr;
    xr = a[ 9] - xr;    a[ 9] = xr;
    xr = a[25] - xr;    a[25] = xr;
    xr = a[ 5] - xr;    a[ 5] = xr;
    xr = a[21] - xr;    a[21] = xr;
    xr = a[13] - xr;    a[13] = xr;
    xr = a[29] - xr;    a[29] = xr;

    xr = a[ 1] - a[ 0]; a[ 1] = xr;
    xr = a[16] - xr;    a[16] = xr;
    xr = a[17] - xr;    a[17] = xr;
    xr = a[ 8] - xr;    a[ 8] = xr;
    xr = a[ 9] - xr;    a[ 9] = xr;
    xr = a[24] - xr;    a[24] = xr;
    xr = a[25] - xr;    a[25] = xr;
    xr = a[ 4] - xr;    a[ 4] = xr;
    xr = a[ 5] - xr;    a[ 5] = xr;
    xr = a[20] - xr;    a[20] = xr;
    xr = a[21] - xr;    a[21] = xr;
    xr = a[12] - xr;    a[12] = xr;
    xr = a[13] - xr;    a[13] = xr;
    xr = a[28] - xr;    a[28] = xr;
    xr = a[29] - xr;    a[29] = xr;

    xr = a[ 0]; a[ 0] = (a[31]+xr)*coef[ 0]; a[31] = (a[31]-xr)*coef[31];
    xr = a[ 1]; a[ 1] = (a[30]-xr)*coef[30]; a[30] = (a[30]+xr)*coef[ 1];
    xr = a[16]; a[16] = (a[15]-xr)*coef[29]; a[15] = (a[15]+xr)*coef[ 2];
    xr = a[17]; a[17] = (a[14]+xr)*coef[ 3]; a[14] = (a[14]-xr)*coef[28];
    xr = a[ 8]; a[ 8] = (a[23]+xr)*coef[ 4]; a[23] = (a[23]-xr)*coef[27];
    xr = a[ 9]; a[ 9] = (a[22]-xr)*coef[26]; a[22] = (a[22]+xr)*coef[ 5];
    xr = a[24]; a[24] = (a[ 7]-xr)*coef[25]; a[ 7] = (a[ 7]+xr)*coef[ 6];
    xr = a[25]; a[25] = (a[ 6]+xr)*coef[ 7]; a[ 6] = (a[ 6]-xr)*coef[24];
    xr = a[ 4]; a[ 4] = (a[27]+xr)*coef[ 8]; a[27] = (a[27]-xr)*coef[23];
    xr = a[ 5]; a[ 5] = (a[26]-xr)*coef[22]; a[26] = (a[26]+xr)*coef[ 9];
    xr = a[20]; a[20] = (a[11]-xr)*coef[21]; a[11] = (a[11]+xr)*coef[10];
    xr = a[21]; a[21] = (a[10]+xr)*coef[11]; a[10] = (a[10]-xr)*coef[20];
    xr = a[12]; a[12] = (a[19]+xr)*coef[12]; a[19] = (a[19]-xr)*coef[19];
    xr = a[13]; a[13] = (a[18]-xr)*coef[18]; a[18] = (a[18]+xr)*coef[13];
    xr = a[28]; a[28] = (a[ 3]-xr)*coef[17]; a[ 3] = (a[ 3]+xr)*coef[14];
    xr = a[29]; a[29] = (a[ 2]+xr)*coef[15]; a[ 2] = (a[ 2]-xr)*coef[16];
}

}


/*-------------------------------------------------------------------*/
/*                                                                   */
/*   Function: Calculation of the MDCT                               */
/*   In the case of long blocks (type 0,1,3) there are               */
/*   36 coefficents in the time domain and 18 in the frequency       */
/*   domain.                                                         */
/*   In the case of short blocks (type 2) there are 3                */
/*   transformations with short length. This leads to 12 coefficents */
/*   in the time and 6 in the frequency domain. In this case the     */
/*   results are stored side by side in the vector out[].            */
/*                                                                   */
/*   New layer3                                                      */
/*                                                                   */
/*-------------------------------------------------------------------*/

inline static void
mdct_short(FLOAT *inout)
{
    int l;
    for ( l = 0; l < 3; l++ ) {
	FLOAT tc0,tc1,tc2,ts0,ts1,ts2;

	ts0 = inout[2*3] * win[SHORT_TYPE][0] - inout[5*3];
	tc0 = inout[0*3] * win[SHORT_TYPE][2] - inout[3*3];
	tc1 = ts0 + tc0;
	tc2 = ts0 - tc0;

	ts0 = inout[5*3] * win[SHORT_TYPE][0] + inout[2*3];
	tc0 = inout[3*3] * win[SHORT_TYPE][2] + inout[0*3];
	ts1 = ts0 + tc0;
	ts2 = -ts0 + tc0;

	tc0 = (inout[1*3] * win[SHORT_TYPE][1] - inout[4*3]) * 2.069978111953089e-11; /* tritab_s[1] */
	ts0 = (inout[4*3] * win[SHORT_TYPE][1] + inout[1*3]) * 2.069978111953089e-11; /* tritab_s[1] */

	inout[3*0] = tc1 * 1.907525191737280e-11 /* tritab_s[2] */ + tc0;
	inout[3*5] = -ts1 * 1.907525191737280e-11 /* tritab_s[0] */ + ts0;

	tc2 = tc2 * 0.86602540378443870761 * 1.907525191737281e-11 /* tritab_s[2] */;
	ts1 = ts1 * 0.5 * 1.907525191737281e-11 + ts0;
	inout[3*1] = tc2-ts1;
	inout[3*2] = tc2+ts1;

	tc1 = tc1 * 0.5 * 1.907525191737281e-11 - tc0;
	ts2 = ts2 * 0.86602540378443870761 * 1.907525191737281e-11 /* tritab_s[0] */;
	inout[3*3] = tc1+ts2;
	inout[3*4] = tc1-ts2;

	inout++;
    }
}

inline static void
mdct_long(FLOAT *out, FLOAT *in)
{
    FLOAT ct,st;
    FLOAT t1, t2, t3, t4, t5, t6, t7, t8;
    /* 1,2, 5,6, 9,10, 13,14, 17 */
    t1 = in[17]-in[ 9];
    t3 = in[15]-in[11];
    t4 = in[14]-in[12];
    t5 = in[ 0]+in[ 8];
    t6 = in[ 1]+in[ 7];
    t7 = in[ 2]+in[ 6];
    t8 = in[ 3]+in[ 5];

    out[17] = (t5+t7-t8)-(t6-in[4]);
    st = (t5+t7-t8)*cx[7]+(t6-in[4]);
    ct = (t1-t3-t4)*cx[6];
    out[5] = ct+st;
    out[6] = ct-st;

    t2 = (in[16]-in[10])*cx[6];
    t6 = t6*cx[7] + in[4];
    ct =  t1*cx[0] + t2 + t3*cx[1] + t4*cx[2];
    st = -t5*cx[4] + t6 - t7*cx[5] + t8*cx[3];
    out[1] = ct+st;
    out[2] = ct-st;

    ct =  t1*cx[1] - t2 - t3*cx[2] + t4*cx[0];
    st = -t5*cx[5] + t6 - t7*cx[3] + t8*cx[4];
    out[ 9] = ct+st;
    out[10] = ct-st;

    ct = t1*cx[2] - t2 + t3*cx[0] - t4*cx[1];
    st = t5*cx[3] - t6 + t7*cx[4] - t8*cx[5];
    out[13] = ct+st;
    out[14] = ct-st;

    t1 = in[ 8]-in[ 0];
    t3 = in[ 6]-in[ 2];
    t4 = in[ 5]-in[ 3];
    t5 = in[17]+in[ 9];
    t6 = in[16]+in[10];
    t7 = in[15]+in[11];
    t8 = in[14]+in[12];

    out[0]  = (t5+t7+t8)+(t6+in[13]);
    ct = (t5+t7+t8)*cx[7]-(t6+in[13]);
    st = (t1-t3+t4)*cx[6];
    out[11] = ct+st;
    out[12] = ct-st;

    t2 = (in[7]-in[1])*cx[6];
    t6 = in[13] - t6*cx[7];
    ct = t5*cx[3] - t6 + t7*cx[4] + t8*cx[5];
    st = t1*cx[2] + t2 + t3*cx[0] + t4*cx[1];
    out[3] = ct+st;
    out[4] = ct-st;

    ct = -t5*cx[5] + t6 - t7*cx[3] - t8*cx[4];
    st =  t1*cx[1] + t2 - t3*cx[2] - t4*cx[0];
    out[7] = ct+st;
    out[8] = ct-st;

    ct = -t5*cx[4] + t6 - t7*cx[5] - t8*cx[3];
    st =  t1*cx[0] - t2 + t3*cx[1] - t4*cx[2];
    out[15] = ct+st;
    out[16] = ct-st;
}


void
mdct_sub48(lame_t gfc, int ch)
{
    int gr, k;

    /* thinking cache performance, ch->gr loop is better than gr->ch loop */
    for (gr = 0; gr < gfc->mode_gr; gr++) {
	gr_info *gi = &gfc->tt[gr][ch];
	int type = gi->block_type, band = 0;
	FLOAT *mdct_enc = gi->xr, *endband = &gi->xr[gfc->xrNumMax_longblock];
	if (gfc->xrNumMax_longblock < 576-18)
	    endband += 18;
	/*
	 * Perform imdct of
	 *  (18 previous subband samples) + (18 current subband samples)
	 */
	do {
	    FLOAT *prev = &gfc->w.sb_smpl[ch][gr  ][0][mdctorder[band]];
	    FLOAT *next = &gfc->w.sb_smpl[ch][gr+1][0][mdctorder[band]];
	    if (type != SHORT_TYPE || (gi->mixed_block_flag && band < 2)) {
		FLOAT work[18];
		for (k = -NL/4; k < 0; k++) {
		    FLOAT a, b;
		    a = win[type][k+27] * next[(k+9)*32]
		      + win[type][k+36] * next[(8-k)*32];
		    b = win[type][k+ 9] * prev[(k+9)*32]
		      - win[type][k+18] * prev[(8-k)*32];
		    work[k+ 9] = a - tantab_l[k+9] * b;
		    work[k+18] = a * tantab_l[k+9] + b;
		}

		mdct_long(mdct_enc, work);
		if (band == 0)
		    continue;
		/*
		 * Perform aliasing reduction butterfly
		 */
		for (k = 7; k >= 0; --k) {
		    FLOAT bu,bd;
		    bu = mdct_enc[k] * ca[k] + mdct_enc[-1-k] * cs[k];
		    bd = mdct_enc[k] * cs[k] - mdct_enc[-1-k] * ca[k];

		    mdct_enc[-1-k] = bu;
		    mdct_enc[k]    = bd;
		}
	    } else {
		for (k = -NS/4; k < 0; k++) {
		    int j = k+NS/4;
		    FLOAT w = win[SHORT_TYPE][j];
		    mdct_enc[j*3   ] = prev[( 6+j)*32] * w - prev[(11-j)*32];
		    mdct_enc[j*3+ 9] = prev[(17-j)*32] * w + prev[(12+j)*32];
		    mdct_enc[j*3+ 1] = prev[(12+j)*32] * w - prev[(17-j)*32];
		    mdct_enc[j*3+10] = next[( 5-j)*32] * w + next[(   j)*32];
		    mdct_enc[j*3+ 2] = next[(   j)*32] * w - next[( 5-j)*32];
		    mdct_enc[j*3+11] = next[(11-j)*32] * w + next[( 6+j)*32];
		}
		mdct_short(mdct_enc);
	    }
	} while (++band, (mdct_enc += 18) < endband);
	if (gfc->filter_type == 1) {
	    mdct_enc = gi->xr;
	    k = 0;
	    do {
		mdct_enc[0] *= gfc->amp_filter1[k];
		mdct_enc[1] *= gfc->amp_filter1[k];
		mdct_enc[2] *= gfc->amp_filter1[k];
		mdct_enc += 3;
		k++;
	    } while (mdct_enc < endband);
	}
	memset(mdct_enc, 0, sizeof(FLOAT)*(SBLIMIT-band)*18);
    }
}

void
subband(lame_t gfc, const sample_t *wk, FLOAT *samp)
{
    int k;
    wk += 286 + FFTOFFSET;
    for (k = 0; k < (18 / 2) * gfc->mode_gr; k++) {
	int band;
	window_subband(gfc->amp_filter0, wk, samp);
	window_subband(gfc->amp_filter0, wk + 32, samp + 32);
	samp += 64;
	wk += 64;
	for (band = -16; band < 0; band++)
	    /* Compensate for inversion in the analysis filter */
	    samp[band] *= (FLOAT)-1.0;
    }
}
