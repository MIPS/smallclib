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
*              file : $RCSfile: malign.c,v $
*            author : $Author Imagination Technologies Ltd
* date last revised : $
*   current version : $
******************************************************************************/

#include <stdlib.h>
#include <low/_malloc.h>

#ifndef MAX
#define MAX(a,b) ((a) >= (b) ? (a) : (b))
#endif

#ifdef __mips_clib_tiny

void *memalign(size_t align, size_t size)
{
  return NULL;
}

#else /* ! __mips_clib_tiny */

void *memalign(size_t align, size_t size)
{
  chunk * chunk_p;
  size_t ma_size, size_with_padding, offset;
  char *allocated, *aligned_p;

  /* Return NULL if align isn't power of 2 */
  if ((align & (align - 1)) != 0)
    return NULL;

  align = MAX(align, MALLOC_ALIGN);
  ma_size = ALIGN_TO(size, CHUNK_ALIGN);
  size_with_padding = ma_size + align - MALLOC_ALIGN;

  allocated = malloc(size_with_padding);
  if (allocated == NULL)
    return NULL;

  /* Get chunk from ptr */
  chunk_p = (chunk *) (allocated - ADMIN_SIZE);

  /* Skip the padding area */
  if (chunk_p->size < 0)
    chunk_p = (chunk *)(((char *) chunk_p) + chunk_p->size);

  aligned_p = (char *)ALIGN_TO(
                (unsigned long)((char *)chunk_p + ADMIN_SIZE),
                (unsigned long)align);

  offset = aligned_p - ((char *)chunk_p + ADMIN_SIZE);
  if (offset)
    *(int *)((char *) chunk_p + ADMIN_SIZE) = -offset;

  return aligned_p;
}
#endif
