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

#ifndef TPSTRINGUTILS_H
#define TPSTRINGUTILS_H

#include <stdio.h>

template <typename T>
unsigned int tpStrLen(const T* str)
{
	unsigned int ret = 0;
	for(; str && *str != T(0); str++) { ret++; }
	return ret;
}

template <typename T>
inline int tpStrCmp(const T* str1,const T* str2)
{
	if (str1 == 0 && str2) return  1;
	if (str2 == 0 && str1) return -1;

	while ((*str1 == *str2) && *str1 && *str2) str1++, str2++;

	if (*(str1) > *(str2)) return 1;
	if (*(str1) < *(str2)) return -1;

	return 0;
}

template <typename T>
inline int tpStrNCmp(const T* str1, const T* str2, tpSizeT n)
{
	while (0 < n--)
	{
		T c1 = *str1++;
		T c2 = *str2++;
		if (c1 != c2)
		{
			return c1 - c2;
		}
		if (c1 == (T)0)
		{
			return 0;
		}
	}
	return 0;
}

template <typename T>
inline T* tpStrDup(const T* src)
{
	tpSizeT _size = tpStrLen(src) * sizeof(T) + 1;
	T* res = (T*)malloc(_size);
	memcpy(res,src,_size);
	if (_size) res[_size-1] = (T)0;
	return res;
}

template <typename T>
inline const T* tpStrRChr(const T* str,T ch)
{
	const T* res = 0;
	do {
		if (ch == *str) res = str;
	} while (*str++);
	return res;
}

template <typename T>
inline const T* tpStrChr(const T* str,T ch)
{
	do {
		if (*str == ch)
		{
			return str;
		}
	} while (*str++);
	return 0;
};


template <typename T>
inline const T* tpStrStr( const T* s1, const T* s2 )
{
	const T* p = s1;
	tpUInt len = tpStrLen( s2 );
	for (; (p = tpStrChr(p, *s2)) != 0; p++)
	{
		if (tpStrNCmp( p, s2, len ) == 0)
		{
			return p;
		}
	}
	return 0;
}

#endif
