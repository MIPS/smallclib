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

/*
FUNCTION
<<atoll>>---convert a string to a long long integer

INDEX
        atoll
INDEX
        _atoll_r

ANSI_SYNOPSIS
        #include <stdlib.h>
        long long atoll(const char *<[str]>);
        long long _atoll_r(struct _reent *<[ptr]>, const char *<[str]>);

TRAD_SYNOPSIS
        #include <stdlib.h>
        long long atoll(<[str]>)
        const char *<[str]>;

        long long _atoll_r(<[ptr]>, <[str]>)
>-------struct _reent *<[ptr]>;
        const char *<[str]>;

DESCRIPTION
The function <<atoll>> converts the initial portion of the string~
pointed to by <<*<[str]>>> to a type <<long long>>.  A call to
atoll(str) in this implementation is equivalent to~
strtoll(str, (char **)NULL, 10) including behavior on error.

The alternate function <<_atoll_r>> is a reentrant version.  The
extra argument <[reent]> is a pointer to a reentrancy structure.


RETURNS
The converted value.

PORTABILITY
<<atoll>> is ISO 9899 (C99) and POSIX 1003.1-2001 compatable.

No supporting OS subroutines are required.
*/

#include <stdlib.h>

unsigned long long _strtoxll (const char *nptr, char **endptr, int base, int uns);

long long
atoll ( const char *s)
{
  return (long long) _strtoxll (s, NULL, 10, 0);
}
