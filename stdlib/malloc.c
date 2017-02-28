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
*              file : $RCSfile: malloc.c,v $
*            author : $Author Imagination Technologies Ltd
* date last revised : $
*   current version : $
******************************************************************************/

#include <stdlib.h>
#include <low/_malloc.h>

/* List of allocated chunks for mstats */
chunk *__malloc_chunk_list = NULL;

/* Previous allocated chunk */
static chunk *prev_chunk = NULL;

static void *sbrk_aligned (size_t s)
{
  char *p, *align_p;

  p = (char *) sbrk (s);

  if (p == (void *)-1)
      return p;

  align_p = (char *) ALIGN_TO ((unsigned long) p, CHUNK_ALIGN);

  /* p is not aligned, ask for a few more bytes */
  if (align_p != p)
    {
      p = (char *) sbrk (align_p - p);
      if (p == (void *)-1)
        return p;
    }

  return align_p;
}

void *malloc (size_t size)
{
  int offset;
  char *ptr, *align_ptr;
  chunk *block, *nblock;
  size_t alloc_size, rem_size;

  alloc_size = ALIGN_TO(size, CHUNK_ALIGN);
  alloc_size += MALLOC_PADDING;
  alloc_size += ADMIN_SIZE;

  block = __malloc_free_list;
  while (block)
    {
      if (IS_FREE(block))
        {
          rem_size = block->size - alloc_size;
          if ((long)rem_size >= 0)
            {
              /* Split this chunk */
              if (rem_size >= MALLOC_MINCHUNK)
                {
                  nblock = (chunk *)((char *) block + alloc_size);
                  nblock->size = rem_size;
                  nblock->next = block->next;
                  if (nblock->next == NULL)
                    prev_chunk = nblock;
                  block->size = alloc_size;
                  block->next = nblock;
                }

              /* We have found a free chunk */
              break;
            }
        }
      block = block->next;
    }

  /*
   * If the return address (user_space) is naturally aligned to MALLOC_ALIGN then 
   * the chunk looks like:
   *
   *                 -----------------------
   * size (4-bytes)  | size of the chunk   |
   *                 -----------------------
   * next (4-bytes)  | ptr to next chunk   |
   *                 -----------------------
   * user_space      | user space...       |
   *                 |                     |
   *                 |                     |
   *                 -----------------------
   *
   * Note that user_space is not counted in ADMIN_SIZE.
   *
   * When return address is not aligned to MALLOC_ALIGN then extra bytes are
   * allocated and the size of padding reqruied for alignment is kept at -ADMIN_SIZE
   * from the start of user_space pointer.
   *
   *                 -----------------------
   * size (4-bytes)  | size of the chunk   |
   *                 -----------------------
   * next (4-bytes)  | ptr to next chunk   |
   *                 -----------------------
   * psize           | -8 (4-bytes)        |
   *                 -----------------------
   *                 | padding (4-bytes)   |
   *                 -----------------------
   * user_space      | user space...       |
   *                 |                     |
   *                 -----------------------
   *
   * In this case, start of a chunk is calculated by user_space + psize.
   * As we have aligned the chunk at 8-byte boundary, we will need 0 or 8-bytes
   * to align user_space to 16-byte boundary. Thus padding is always of 8-bytes
   * when present.
  */

   /* Create a new chunk */
   if (block == NULL)
     {
       block = (chunk *) sbrk_aligned (alloc_size);
       if (block == (void *)-1)
         return NULL;

       block->next = NULL;

       /* Create chain of allocated chunks */
       if (prev_chunk)
         prev_chunk->next = block;
       else
         __malloc_chunk_list = block;
       prev_chunk = block;
     }

  /* Calculate return address */
  ptr = (char *) block + ADMIN_SIZE;

  /* Align return address to MALLOC_ALIGN */
  align_ptr = (char *) ALIGN_TO((unsigned long) ptr, MALLOC_ALIGN);
  offset = align_ptr - ptr;
  if (offset)
    *(int *)((char *) block + ADMIN_SIZE) = -offset;

  block->size = alloc_size | CHUNK_USED;

  return align_ptr;
}
