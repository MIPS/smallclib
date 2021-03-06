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
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <stdio.h>
#include <sys/types.h>
#include <low/_stdio.h>

/* Return position in a stream or file */
off_t ftello (FILE * afp)
{
  _fpos_t pos;
  __smFILE *fp = (__smFILE*) afp;

  /* 
   * Find offset of underlying I/O object, then adjust for buffered
   * bytes. Flush a write stream, since the offset may be altered if
   * the stream is appending. Do not flush a read stream, since we
   * must not lose the ungetc buffer.  
  */
  if (fp->_flags & __SWR)
    fflush ((FILE*) fp);
    
  if (fp->_flags & __SOFF)
    pos = fp->_offset;
  else
    {
      pos = low_seek (fp, 0, SEEK_CUR);
      if (pos == -1L)
          return pos;
    }
    
  if (fp->_flags & __SRD)
    {
      /*
       * Reading. Any unread characters (including
       * those from ungetc) cause the position to be
       * smaller than that in the underlying object.
       */
      pos -= fp->_rsize;
      pos -= fp->_ursize;
    }
  else if ((fp->_flags & __SWR) && fp->_cptr != NULL)
    {
      /*
       * Writing. Any buffered characters cause the
       * position to be greater than that in the
       * underlying object.
       */
      pos += fp->_cptr - fp->_base;
    }

  return pos;
}/* ftello */

