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

static unsigned int _coalesce (unsigned char * block)
{
    unsigned char *nblock = NULL;
    unsigned int info = 0, ninfo = 0, nsize = 0, tsize = 0;

    info = get_info (block);
    tsize = get_block_size (info);
    
    if (is_last (info) || ! is_free (info))
      return tsize;

    for (;;)
      {
        nblock = next_block (block);
        ninfo = get_info (nblock);

        if (is_last (ninfo) || ! is_free (ninfo))
          return tsize;
            
        nsize = get_block_size (ninfo);
        tsize = tsize + nsize + ADMIN_SIZE;	/* use space required for bookkeeping as well */
        set_info (block) = tsize | BLOCK_FREE;
      }/* coalesce until we hit non-free block */

    return tsize;
}/* _coalesce */

void free (void *ptr)
{
  unsigned char *block = NULL;
  unsigned int info = 0;

  if (ptr == NULL)
    return;

  /* mark current block as free */
  block = ((unsigned char *) ptr) - ADMIN_SIZE;
  info = get_info (block);
  set_info (block) = get_block_size (info) | BLOCK_FREE;

  /* coalesce adjacent free blocks */
  _coalesce (block);

  return;
}/* free */

#else /* ! __mips_clib_tiny */

/* List of free chunks */
chunk *__malloc_free_list = NULL;

void free (void *ptr)
{
  chunk *block, *nblock;

  if (ptr == NULL)
    return;

  /* Get chunk from ptr */
  block = (chunk *) (ptr - ADMIN_SIZE);

  /* Skip the padding area */
  if (block->size < 0)
    block = (chunk *)(((char *) block) + block->size);

  /* Mark it as free */
  block->size &= ~CHUNK_USED;

  /* Create list of free chunks */
  if (__malloc_free_list == NULL || block < __malloc_free_list)
    __malloc_free_list = block;

  /* Coalesce adjacent free chunks */
  block = __malloc_free_list;
  for (;;)
    {
      nblock = block->next;
      if (nblock && IS_FREE (nblock)
          && (char*) nblock == ((char*) block) + block->size)
        {
          block->size += nblock->size;
          block->next = nblock->next;
          continue;
        }
      break;
    }

  return;
}/* free */

#endif /* __mips_clib_tiny */
