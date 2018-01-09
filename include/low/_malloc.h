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

/******************************************************************************
*                 file : $RCSfile: _malloc.h,v $ 
*    date last revised : $
*      current version : $
******************************************************************************/

#ifndef __LOW_MALLOC_H_
#define __LOW_MALLOC_H_

#ifdef __mips_clib_tiny

#define ALLOC_SIZE  (1024*4)
#define ADMIN_SIZE  (16)
#define ALIGN_SIZE  (16)

#define BLOCK_FREE  1
#define BLOCK_LAST  2

#define is_free(INFO)     ((((INFO) & 3) & BLOCK_FREE) == BLOCK_FREE)
#define is_last(INFO)     ((((INFO) & 3) & BLOCK_LAST) == BLOCK_LAST)
#define get_block_size(INFO)  (((INFO) & (~3)))
#define set_info(BLOCK)       (*((unsigned int *)(BLOCK)))
#define get_info(BLOCK)       (*((unsigned int *)(BLOCK)))
#define next_block(BLOCK)     ((BLOCK) + get_block_size (get_info ((BLOCK))) + ADMIN_SIZE)

extern void *__smlib_heap_start;

#else /* !__mips_clib_tiny */

#define CHUNK_FREE      0
#define CHUNK_USED      1

#define IS_USED(BLOCK)  ((BLOCK)->size & CHUNK_USED)
#define IS_FREE(BLOCK)  (!IS_USED ((BLOCK)))

/*
 * Last two bits of the size are used as flags. CHUNK_ALIGNED is used to
 * indicate that the user_space is aligned to MALLOC_ALIGN by adding 8-bytes
 * of padding.
*/
typedef struct malloc_chunk
{
  long size;                    /* Size of the chunk including padding, bookkeeping */
  struct malloc_chunk *next;    /* Pointer to next allocated chunk */
  void *user_space;             /* Start of user space (not counted in ADMIN_SIZE) */
} chunk;

#define MALLOC_ALIGN            (16U)
#define CHUNK_ALIGN             (8U)
#define ADMIN_SIZE              (8U)
#define ALIGN_TO(SIZE, ALIGN)   (((SIZE) + (ALIGN) - 1) & ~((ALIGN) - 1))

/* 
 * If we split a chunk into two, then each chunk must contian space
 * to accommodate padding bytes reqruied for MALLOC_ALIGN. As we
 * have aligned the chunk to 8-byte boundary we need max 8 bytes
 * to align the user space to MALLOC_ALIGN. The newly created chunk
 * should have atleast 1 byte for user space. Max 7-bytes to align
 * the chunk it self.
*/
#define MALLOC_PADDING          (8U)
#define MALLOC_MINCHUNK         (24U)   /* 8 + 7 + 8 + 1 */

/* List of free chunks */
extern chunk *__malloc_free_list;

/* List of allocated chunks for mstats */
extern chunk *__malloc_chunk_list;

void * sbrk (size_t incr);

#endif /* __mips_clib_tiny */

#endif /* __LOW_MALLOC_H_ */

