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



/* wf_j1.c -- float version of w_j1.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

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
 * wrapper of j1f,y1f 
 */

#include "low/_math.h"
#include <errno.h>


#ifdef __STDC__
	float j1f(float x)		/* wrapper j1f */
#else
	float j1f(x)			/* wrapper j1f */
	float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_j1f(x);
#else
	float z;
	z = __ieee754_j1f(x);
	if(fabsf(x)>(float)X_TLOSS) 
  {
	    /* j1f(|x|>X_TLOSS) */
    errno = ERANGE;
    return 0.0; 
	}
  else
	  return z;
#endif
}

#ifdef __STDC__
	float y1f(float x)		/* wrapper y1f */
#else
	float y1f(x)			/* wrapper y1f */
	float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_y1f(x);
#else
	float z;
	z = __ieee754_y1f(x);
  if(x <= (float)0.0)
  {
	    /* y1f(0) = -inf or y1f(x<0) = NaN */
#ifndef HUGE_VAL 
#define HUGE_VAL inf
	  double inf = 0.0;

	  SET_HIGH_WORD(inf,0x7ff00000);	/* set inf to infinite */
#endif
    errno = EDOM;
    return (float)-HUGE_VAL;              
  }
	if(x>(float)X_TLOSS)
  {
	    /* y1f(x>X_TLOSS) */
    errno = ERANGE;
    return (float)0.0; 
	} 
  else
	  return z;
#endif
}

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double j1(double x)
#else
	double j1(x)
	double x;
#endif
{
	return (double) j1f((float) x);
}

#ifdef __STDC__
	double y1(double x)
#else
	double y1(x)
	double x;
#endif
{
	return (double) y1f((float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
