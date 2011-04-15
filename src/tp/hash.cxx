/*
 * Twisted Pair Visualization Engine
 *
 * Copyright (c) 1999-2009 Hartmut Seichter 
 * 
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the Twisted Pair License (TPL) version 1.0 or (at your option) 
 * any later version. The full license text is available in the LICENSE file 
 * included with this distribution, and on the technotecture.com website.
 *
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

	
