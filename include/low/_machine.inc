/*******************************************************************************
 *
 * Copyright 2014-2015, Imagination Technologies Limited and/or its
 *                      affiliated group companies.
 * All rights reserved.
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

/******************************************************************************
*                 file : $RCSfile: _machine.inc,v $ 
*               author : $Author Imagination Technologies Ltd
*    date last revised : $
*      current version : $
******************************************************************************/

/* Bit descriptors for floating point status/control registers */

#define FCSR_ENABLE_BITS		0x000007C0
#define FCSR_ENABLE_SHIFT 	7

#define FCSR_ENABLE_INEXACT_BIT		0x00000080
#define FCSR_ENABLE_UNDERFLOW_BIT 	0x00000100
#define FCSR_ENABLE_OVERFLOW_BIT 	0x00000200
#define FCSR_ENABLE_DIV0_BIT 		0x00000400
#define FCSR_ENABLE_INVALIDOP_BIT 	0x00000800

#define FCSR_FLAG_BITS 	      0x0000007C
#define FCSR_FLAG_SHIFT       2

#define FCSR_FLAG_INEXACT_BIT	0x00000004
#define FCSR_FLAG_UNDERFLOW_BIT 0x00000008
#define FCSR_FLAG_OVERFLOW_BIT 	0x00000010
#define FCSR_FLAG_DIV0_BIT 	0x00000020
#define FCSR_FLAG_INVALIDOP_BIT 0x00000040


#define FCSR_CAUSE_BITS		0x0003F000
#define FCSR_CAUSE_SHIFT 	12

#define FCSR_CAUSE_INEXACT_BIT		0x00001000
#define FCSR_CAUSE_UNDERFLOW_BIT 	0x00002000
#define FCSR_CAUSE_OVERFLOW_BIT 	0x00004000
#define FCSR_CAUSE_DIV0_BIT 		0x00008000
#define FCSR_CAUSE_INVALIDOP_BIT 	0x00010000
#define FCSR_CAUSE_UNIMPL_BIT 		0x00020000

#define FCSR_ROUND_BITS		0x00000003
#define FCSR_ROUND_SHIFT 	0

#define FCSR_ROUND_TO_NEAREST	0x0
#define FCSR_ROUND_TO_ZERO	0x1
#define FCSR_ROUND_TO_PLUSINF	0x2
#define FCSR_ROUND_TO_MINUSINF	0x3
