/*
 *	Machine dependent defines/includes for LAME.
 *
 *	Copyright (c) 1999 A.L. Faber
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


#ifndef LAME_MACHINE_H
#define LAME_MACHINE_H

#include        <stdio.h>
#include        <string.h>
#if defined(__riscos__) && defined(FPA10)
#include	"ymath.h"
#else
#include	<math.h>
#endif

#include        <stdlib.h>
#include		<ctype.h>
#include		<signal.h>
#include		<fcntl.h>
#include		<errno.h>

#ifdef macintosh
#include   <types.h>
#include   <stat.h>
#else
#include  <sys/types.h>
#include  <sys/stat.h>
#endif





/* three different types of pow() functions:
   1. table lookup
   2. pow()
   3. exp()   on some machines this is claimed to be faster than pow()
*/


#define POW20(x)  pow20[x]
/*
#define POW20(x)  pow(2.0,((double)(x)-210)*.25)
#define POW20(x)  exp( ((double)(x)-210)*(.25*LOG2) )
*/

#define IPOW20(x)  ipow20[x]
/*
#define IPOW20(x)  exp( -((double)(x)-210)*.1875*LOG2 )
#define IPOW20(x)  pow(2.0,-((double)(x)-210)*.1875)
*/



#if ( defined(_MSC_VER) && !defined(INLINE))
#	define INLINE _inline
#elif defined(__SASC) || defined(__GNUC__)
#	define INLINE __inline
#else
#	define INLINE
#endif

#if ( defined(_MSC_VER))
#	pragma warning( disable : 4244 )
//#	pragma warning( disable : 4305 )
#endif


#if ( defined(_MSC_VER) || defined(__BORLANDC__) || defined(__MINGW32__) )
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#else
	typedef float FLOAT;
#endif

/*

FLOAT    for variables which require at least 32bits
FLOAT8   for variables which require at least 64bits

On some machines, 64bit will be faster than 32bit.  Also, some math
routines require 64bit float, so setting FLOAT=float will result in a
lot of conversions.

   MH: the x86 asm quantization routines in quantize-pvt.c
   are designed to work only with 8-byte doubles or 4-byte
   floats. if you use a different type (e.g. 10-byte extended
   precision long doubles, as supported by ICL), you will need
   to disable the ASM routines (or fix them :) 
*/

/* NOTE: RH: 7/00:  if FLOAT8=float, it breaks resampling and VBR code */
typedef double FLOAT8;



/*
Various integer types 
 */

#if defined _WIN32 && !defined __CYGWIN__
	typedef unsigned char	u_char;
#       define int64 __int64

#elif defined __DECALPHA__
/*       do nothing */

#elif defined OS_AMIGAOS
/*       do nothing */

#elif defined __DJGPP__
	typedef unsigned char	u_char;

#elif !defined __GNUC__ || defined __STRICT_ANSI__
	typedef unsigned char	u_char;

#else
#define int64 long long
#endif





/* sample_t must be floating point, at least 32 bits */
typedef FLOAT sample_t;
typedef sample_t          stereo_t [2];



#endif
