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

#ifndef TPMATH_H
#define TPMATH_H

#include <tp/types.h>

#include <math.h>

//! internal calculations need a decent PI
const tpReal TP_PI = tpReal(3.14159265358979323846);


//! transform radiant to degree
template <class T> inline T tpRad2Deg(const T& radiant)
{
	return (T)radiant * 180 / (T)TP_PI;
}

//! transform degree to radiant
template <class T> inline T tpDeg2Rad(const T& degree)
{
	return (T)degree / 180 * (T)TP_PI;
}

template <class T> inline T tpSqrt(const T& in) 
{
	return ::sqrt(in);
}



#endif
