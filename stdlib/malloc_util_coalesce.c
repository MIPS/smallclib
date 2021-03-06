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

/******************************************************************************
*              file : $RCSfile:  $
* date last revised : $
*   current version : $
******************************************************************************/

#include <stdlib.h>
#include <low/_malloc.h>

unsigned int _coalesce (unsigned char * block)
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
