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

#ifndef TP_TYPES_H
#define TP_TYPES_H


#include <stddef.h>
#include <stdlib.h>

// some wrappers for basic types
typedef void tpVoid;

typedef int tpInt;
typedef unsigned int tpUInt;

typedef float tpFloat;
typedef double tpDouble;

typedef char tpChar;
typedef unsigned char tpUChar;

typedef char tpByte;
typedef unsigned char tpUByte;

typedef long tpLong;
typedef unsigned long tpULong;

typedef short tpShort;
typedef unsigned short tpUShort;

typedef size_t tpSizeT;


#if defined(HAVE_WCHAR_H)
typedef wchar_t tpWChar;
#endif



/*
#ifdef bool
	typedef bool tpBool;
#else
	typedef unsigned char tpBool;
#endif

#ifndef FALSE
const tpBool FALSE = (tpBool)0;
#endif

#ifndef TRUE
const tpBool TRUE = (tpBool)1;
#endif

*/

// Pointers
typedef tpChar* tpCharPtr;
typedef void* tpRawPtr;


struct tpSize {
	tpUInt width, height;

	inline
	tpSize(tpUInt w, tpUInt h)
		: width(w)
		, height(h)
	{}
};

struct tpPoint {
	tpInt x, y;

	inline
	tpPoint(tpInt x_, tpInt y_)
		: x(x_)
		, y(y_)
	{}
};

struct tpRect {
	tpPoint origin;
	tpSize size;

	inline
	tpRect(tpInt x, tpInt y, tpUInt w, tpUInt h)
		: origin(x,y)
		, size(w,h)
	{}
};


#if defined(__unix) || defined(__APPLE__) || defined(__BEOS__) || defined(__HAIKU__)
typedef int tpHandle;
#elif defined(_WIN32)
typedef void* tpHandle;
#endif

#undef TP_DOUBLE
#define TP_SINGLE

#if defined(TP_DOUBLE)
typedef tpDouble tpReal;
#elif defined(TP_SINGLE)
typedef tpFloat tpReal;
#endif


#if defined(_MSC_VER)

typedef __int64 tpInt64;
typedef unsigned __int64 tpUInt64;

typedef __int32 tpInt32;
typedef unsigned __int32 tpUInt32;

typedef __int16 tpInt16;
typedef unsigned __int16 tpUInt16;

typedef __int8 tpInt8;
typedef unsigned __int8 tpUInt8;

#else

#include <stdint.h>
typedef uint64_t tpUInt64;
typedef int64_t tpInt64;

typedef uint32_t tpUInt32;
typedef int32_t tpInt32;

typedef uint16_t tpUInt16;
typedef int16_t tpInt16;

typedef uint8_t tpUInt8;
typedef int8_t tpInt8;


#endif


#endif
