/*******************************************************************************
 *
 * Copyright (C) 2014-2018 MIPS Tech, LLC
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

#include <stdlib.h>
#include <low/_malloc.h>

#ifdef __mips_clib_tiny

size_t malloc_usable_size(void * ptr)
{
  unsigned char *block = NULL;
  unsigned int info = 0;
  block = ((unsigned char *) ptr) - ADMIN_SIZE;
  info = get_info (block);
  return get_block_size (info);
}

#else /* ! __mips_clib_tiny */

size_t malloc_usable_size(void * ptr)
{
  chunk *block;
  int size_or_offset;

  /* Get chunk from ptr */
  block = (chunk *) (ptr - ADMIN_SIZE);
  size_or_offset = (block->size & ~CHUNK_USED);

  if (size_or_offset < 0)
  {
      block = (chunk *)((char *)block + block->size);
      return (block->size & ~CHUNK_USED) - ADMIN_SIZE + size_or_offset;
  }

  return (block->size & ~CHUNK_USED) - ADMIN_SIZE;
}

#endif
