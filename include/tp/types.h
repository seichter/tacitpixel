/*
 * Tacit Pixel - Framework
 *
 * Copyright (c) 1999-2011 Hartmut Seichter 
 * 
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the Tacit Pixel License (TPL) version 1.0 or (at your option) 
 * any later version. The full license text is available in the LICENSE file 
 * included with this distribution, and on the technotecture.com website.
 *
 */
#ifndef _TACITPIXEL_TYPES_H_
#define _TACITPIXEL_TYPES_H_


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

// Pointers
typedef tpChar* tpCharPtr;
typedef void* tpRawPtr;


struct tpSize {
	tpUInt width, height;
};

struct tpPoint {
	tpInt x, y;
};


#if defined(__unix) || defined(__APPLE__)
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


#endif
