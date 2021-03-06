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
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <low/_stdio.h>

static int fflush_stream (__smFILE *);

/*
 * Flush buffered file output
*/
int fflush (FILE *afp)
{
  __smFILE *fp = (__smFILE*) afp;

  if (fp == NULL)
    {
#ifdef __STATIC_IO_STREAMS__
  int i, ret = 0;

  for (i = 0; i < (FOPEN_MAX - 3); i++)
      ret |= fflush_stream (&__io_streams[i]);
      
#else
  int ret = 0;
  __stream_list *tmp;
  
  for (tmp = __io_streams; tmp != NULL; tmp = tmp->next)
      ret |= fflush_stream (tmp->fp);
#endif    
      return ret;
      
    }
  else
    return fflush_stream (fp);
    
  return 0;
}/* fflush */

static int fflush_stream (__smFILE *fp)
{
  int n, t;
  unsigned char *p;

  if (fp->_flags == 0)
    return 0;   /* nothing to flush */
  
  if ((fp->_flags & __SWR) == 0)
    {
      if (fp->_rsize > 0 || fp->_ursize > 0)
      {
        _fpos_t curoff;

        /* Get the physical position we are at in the file.  */
        if (fp->_flags & __SOFF)
          curoff = fp->_offset;
        else
          {
            /* We don't know current physical offset, so ask for it. */
            curoff = low_seek (fp, 0, SEEK_CUR);
            
            if (curoff == -1L)
              {
                fp->_flags |= __SERR;
                return EOF;
              }
          }
          
        /* Compensate for characters not yet read */
        if (fp->_flags & __SRD)
          {
            curoff -= fp->_rsize;
            curoff -= fp->_ursize;
          }
          
        /* Now physically seek to after byte last read */
        curoff = low_seek (fp, curoff, SEEK_SET);
        
        if (curoff != -1L)
          {
            fp->_rsize = 0;
            fp->_cptr = fp->_base;
            fp->_flags |= __SOFF;
            fp->_offset = curoff;
            fp->_ursize = 0;   /* clear ungetc buffer */
            fp->_ucptr = 0;   /* clear ungetc buffer */
          }
        else
          {
            fp->_flags |= __SERR;
            return EOF;
          }
      }/* if unread chars are in the buffer */
      
      return 0;
    }/* if not writing */
    
  /* Nothing to flush. */
  if ((p = fp->_base) == NULL)
      return 0;
    
  /* Write this much */
  n = fp->_cptr - p;

  /* Reset the read/write pointer and write size */
  fp->_cptr = p;
  fp->_wsize = fp->_flags & __SNBF ? 0 : fp->_bsize;

  while (n > 0)
    {
      t = low_write (fp, (char *) p, n);
      if (t <= 0)
        {
          fp->_flags |= __SERR;
          return EOF;
        }
      p += t;
      n -= t;
    }
    
  return 0;
}/* fflush_stream */

