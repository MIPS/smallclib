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

/* Copyright (C) 2002 by  Red Hat, Incorporated. All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software
 * is freely granted, provided that this notice is preserved.
 */

/* same as newlib 2.0 */

#include <argz.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

error_t
_DEFUN (argz_create, (argv, argz, argz_len),
       char *const argv[] _AND
       char **argz _AND
       size_t *argz_len)
{
  int argc = 0;
  int i = 0;
  int len = 0;
  char *iter;

  *argz_len = 0;

  if (*argv == NULL)
    {
      *argz = NULL;
      return 0;
    }

  while (argv[argc])
    {
      *argz_len += (strlen(argv[argc]) + 1);
      argc++;
    }

  /* There are argc strings to copy into argz. */
  if(!(*argz = (char *)malloc(*argz_len)))
    return ENOMEM;

  iter = *argz;
  for(i = 0; i < argc; i++)
    {
      len = strlen(argv[i]) + 1;
      memcpy(iter, argv[i], len);
      iter += len;
    }
  return 0;
}
