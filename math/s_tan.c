/*******************************************************************************
 *
 * Copyright (C) 2014-2018 Wave Computing, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/

/* From Newlib 2.0. Modified to generate EDOM for infinite argument */

/* @(#)s_tan.c 5.1 93/09/24 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */


/*

FUNCTION
        <<tan>>, <<tanf>>---tangent

INDEX
tan
INDEX
tanf

ANSI_SYNOPSIS
        #include <math.h>
        double tan(double <[x]>);
        float tanf(float <[x]>);

TRAD_SYNOPSIS
        #include <math.h>
        double tan(<[x]>)
        double <[x]>;

        float tanf(<[x]>)
        float <[x]>;


DESCRIPTION
<<tan>> computes the tangent of the argument <[x]>.  
Angles are specified in radians.  

<<tanf>> is identical, save that it takes and returns <<float>> values.

RETURNS
The tangent of <[x]> is returned. 

PORTABILITY
<<tan>> is ANSI. <<tanf>> is an extension.
*/

/* tan(x)
 * Return tangent function of x.
 *
 * kernel function:
 *	__kernel_tan		... tangent function on [-pi/4,pi/4]
 *	__ieee754_rem_pio2	... argument reduction routine
 *
 * Method.
 *      Let S,C and T denote the sin, cos and tan respectively on 
 *	[-PI/4, +PI/4]. Reduce the argument x to y1+y2 = x-k*pi/2 
 *	in [-pi/4 , +pi/4], and let n = k mod 4.
 *	We have
 *
 *          n        sin(x)      cos(x)        tan(x)
 *     ----------------------------------------------------------
 *	    0	       S	   C		 T
 *	    1	       C	  -S		-1/T
 *	    2	      -S	  -C		 T
 *	    3	      -C	   S		-1/T
 *     ----------------------------------------------------------
 *
 * Special cases:
 *      Let trig be any of sin, cos, or tan.
 *      trig(+-INF)  is NaN, with signals;
 *      trig(NaN)    is that NaN;
 *
 * Accuracy:
 *	TRIG(x) returns trig(x) nearly rounded 
 */

#include "low/_math.h"
#ifdef __MATH_MATCH_x86__
#include <errno.h>
#endif /* __MATH_MATCH_x86__ */

#ifdef __MATH_FULLRANGE_REDUCTION__
/* Full range-reduction is both big and expensive. If it is enabled,
   we will forego this optimization since takes a whole lot of time
   and doesn't save that much(relatively speaking) space */
#define __MATH_SPEED_OVER_SIZE__
#endif /* __MATH_FULLRANGE_REDUCTION */

#ifndef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double tan(double x)
#else
	double tan(x)
	double x;
#endif
{
	double y[2];
	__int32_t n,ix;

    /* High word of x. */
	GET_HIGH_WORD(ix,x);

    /* |x| ~< pi/4 */
	ix &= 0x7fffffff;

#ifdef __MATH_NONFINITE__
    /* tan(Inf or NaN) is NaN */
	if (ix>=0x7ff00000) {
#ifdef __MATH_MATCH_x86__
	    __uint32_t lx;
	    GET_LOW_WORD(lx,x);
	    /* Domain error when input is infinity */
	    if ((ix==0x7ff00000) && (lx==0))
		errno=EDOM;
#endif /* __MATH_MATCH_x86__ */
	    return x-x;
	} else 
#endif /* __MATH_NONFINITE__ */

    /* argument reduction needed */
	{
#ifdef __MATH_SPEED_OVER_SIZE__
	    y[0]=x; y[1]=0.0;n=0;
	    if (ix>0x3fe921fb)
#endif /* __MATH_SPEED_OVER_SIZE__ */
	    n = __ieee754_rem_pio2(x,y);
	    return __kernel_tan(y[0],y[1],1-((n&1)<<1)); /*   1 -- n even
							-1 -- n odd */
	}
}

#endif /* _DOUBLE_IS_32BITS */
