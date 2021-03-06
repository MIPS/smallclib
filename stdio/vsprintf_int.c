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

/*****************************************************************************//**
@File         vsprintf.c

@Title        write the formatted data to the char buffer


@


@Platform     Any

@Synopsis     #include <stdio.h>
              int vsprintf(char *s, const char *fmt, va_list ap)

@Description  <<vsprintf>> is like <<sprintf>>, except that output is directed
              to the buffer <[str]>, and a terminating NUL is output.Behavior
              is undefined if more output is generated than the buffer can 
              hold.

@DocVer       1.0 1st Release
********************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <low/_stdio.h>

int vsprintf_int (char *str, const char *fmt, va_list ap)
{
  int ret;
  ReadWriteInfo rw;
  rw.m_handle = str;
  rw.m_fnptr = __low_sprintf;
  ret = _format_parser_int(&rw, fmt, &ap);
  
  /* terminate the string */
  if (rw.m_handle)
    *((char *)rw.m_handle) = '\0';
  return ret;
}

