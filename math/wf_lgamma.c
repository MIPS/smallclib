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

/* wf_lgamma.c -- float version of w_lgamma.c.
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
 *
 */

#include "low/_math.h"
#include <reent.h>
#include <errno.h>

#ifdef __STDC__
	float lgammaf(float x)
#else
	float lgammaf(x)
	float x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_lgammaf_r(x,&(_REENT_SIGNGAM(_REENT)));
#else
        float y;
        y = __ieee754_lgammaf_r(x,&(_REENT_SIGNGAM(_REENT)));
        if(!finitef(y)&&finitef(x)) {
#ifndef HUGE_VAL 
#define HUGE_VAL inf
	    double inf = 0.0;
	    SET_HIGH_WORD(inf,0x7ff00000);	/* set inf to infinite */
#endif
	    if(floorf(x)==x&&x<=(float)0.0) {
		/* lgammaf(-integer) */
		   errno = EDOM;
            } else {
		/* lgammaf(finite) overflow */
		   errno = ERANGE;
            }
            return (float)HUGE_VAL; 
        } else
            return y;
#endif
}             

#ifdef _DOUBLE_IS_32BITS

#ifdef __STDC__
	double lgamma(double x)
#else
	double lgamma(x)
	double x;
#endif
{
	return (double) lgammaf((float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
