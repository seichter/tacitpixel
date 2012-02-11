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

#ifndef TPUTILS_H
#define TPUTILS_H


template <typename T> inline const T& tpMax(const T& value1,const T& value2)
{
	return (value1 > value2) ? value1 : value2;
}

template <typename T> inline const T& tpMin(const T& value1,const T& value2)
{
	return (value1 < value2) ? value1 : value2;
}

template <typename T> inline const T& tpClamp(const T& value,const T& min,const T& max)
{
	return (value > min) ? (value < max) ? value : max : min;
}

template <typename T> inline const T& tpBounce(const T& val,const T& min,const T& max,T& var)
{
	if (val < min) var = abs(var);
	if (val > max) var = -abs(var);
	return (val += var);
}

template <typename T> inline
void tpSwap(T& value1, T& value2)
{
	T _temp = value1;
	value1 = value2;
	value2 = _temp;
}

/*
template <> void inline tpSwap(char& x, char& y)
{
	x = x ^ y;
	y = x ^ y;
	x = x ^ y;
}
*/

template <typename T> inline bool tpBetween(const T& value,const T& border1,const T& border2)
{
	return (border2 > border1) ? ((value < border2) && (value > border1)) :
		((value > border2) && (value < border1));
}

template <typename T> inline T tpNextPowerOfTwo(T v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	++v;
	return v;
}

template <class T>
inline T tpNearestPowerOfTwo(T v)
{
		int k;
		if (v == 0)
				return 1;
		for (k = sizeof(T) * 8 - 1; ((static_cast<T>(1U) << k) & v) == 0; k--);
		if (((static_cast<T>(1U) << (k - 1)) & v) == 0)
				return static_cast<T>(1U) << k;
		return static_cast<T>(1U) << (k + 1);
}


#endif
