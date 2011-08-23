/* 
 * Copyright (C) 1999-2011 Hartmut Seichter
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "tp/hash.h"

/*

  This implements the Adler-32 algorithm for 
  hashing in libTAP. Please do not use this
  algorithm for anything else as it also has
  some drawbacks:
  
  http://en.wikipedia.org/wiki/Adler-32
  http://de.wikipedia.org/wiki/Adler-32 (German)

*/

tpULong tpAdler32(const tpUChar* buffer,tpULong bufsize)
{

	register tpULong _shift1 = 0x00000001 & 0x0000ffff;
	register tpULong _shift2 = (0x00000001 >> 16) & 0x0000ffff;
	
	for (tpULong i = 0; i < bufsize;i++)
	{
		_shift1 += *buffer++;

		if (0x0000fff1 <= _shift1) _shift1 -= 0x0000fff1;

		_shift2 += _shift1;

		if (0x0000fff1 <= _shift2) _shift2 -= 0x0000fff1;
	};

	return (_shift2 << 16) + _shift1;
}

	
