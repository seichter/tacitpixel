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

#ifndef TPFIXEDPOINT_H
#define TPFIXEDPOINT_H

/**
 * \file tpFixed.h
 *
 * Provides a wrapper around floating point to fixed point conversion which
 * is mostly needed on mobile devices.
 */

#include <tp/globals.h>
#include <tp/types.h>

typedef tpLong tpFixedType; //<! type definition for internal fixed point rep

#define TP_INT_TO_FIXED(x)         ((x) << 16)
#define TP_DOUBLE_TO_FIXED(x)      ((tpLong)(x * 65536.0 + 0.5))
#define TP_FIXED_TO_INT(x)         ((x) >> 16)
#define TP_FIXED_TO_DOUBLE(x)      (((tpDouble)x) / 65536.0)
#define TP_ROUND_FIXED_TO_INT(x)   (((x) + 0x8000) >> 16)

#define TP_FIXED_ONE       TP_INT_TO_FIXED(1)
#define TP_FIXED_PI        205887L
#define TP_FIXED_2PI       411775L
#define TP_FIXED_E         178144L
#define TP_FIXED_ROOT2      74804L
#define TP_FIXED_ROOT3     113512L
#define TP_FIXED_GOLDEN    106039L


template <typename T>
struct tpFixedBase {
	
	tpFixedBase() : m(T(0)) {}
	tpFixedBase(const float& f) { set(f); }
	tpFixedBase(const double& d) { set(d); }
	tpFixedBase(const int& i) { set(i); }
	
	const static int _bits = sizeof(T) << 3;
	const static int _bits_half  = sizeof(T) << 2;
	const static int _bits_half_val  = (1 << _bits_half);
	
	void set(const int& v) { m = v << _bits_half; }
	void set(const double& v) { m = (T)(v * _bits_half_val + 0.5); }
	void set(const float& v) { m = (T)(v * _bits_half_val + 0.5f); }
	
	void get(double& v) const { v = ((double) m) / (double)_bits_half_val; }
	void get(float& v) const { v = ((float) m) / (float)_bits_half_val; }
	void get(int& v) const { v = m >> _bits_half; }
	
	int getInt() const { int v; get(v); return v; }
	double getDouble() const { double v; get(v); return v;  }
	float getFloat() const { float v; get(v); return v;  }
	
	void mult(const tpFixedBase<T>& r)
	{
		m = ((tpInt64)m * r.m) >> r._bits_half; 		
	}
	
	T m;
};


typedef tpFixedBase<tpInt32> tpFixed32;



/**
 * Conversion from actual integer to internal fixed point
 *
 * \param val value in real world integer
 * \return value in fixed point notation
 */
inline tpFixedType tpFixedFromInt( tpInt val) { return TP_INT_TO_FIXED(val); }

/**
 * Conversion from real world double to fixed point
 *
 * \param val value in double
 * \return value in fixed point notation
 */
inline tpFixedType tpFixedFromDouble( tpDouble val ) { return TP_DOUBLE_TO_FIXED(val); }

/**
 * Conversion from fixed point to real world double
 *
 * \param val_fp value in fixed point notation
 * \return value in double notation
 */
inline tpDouble tpFixedToDouble( tpFixedType val_fp ) { return TP_FIXED_TO_DOUBLE(val_fp); }

/**
 * Conversion from fixed point to real world float
 *
 * \param val_fp value in fixed point notation
 * \return value in floating point notation
 */
inline tpFloat tpFixedToFloat( tpFixedType val_fp ) { return float( val_fp ) / 65536.0f; }

/**
 * Conversion from fixed point to real world integer
 *
 * \param val_fp value in fixed point notation
 * \param roundUp flag for rounding up
 * \return value in real world integer
 */
inline tpInt tpFixedToInt( tpFixedType val_fp, bool roundUp = false ) { 
	return  (val_fp + ((roundUp) ? 0x0800 : 0xffff)) >> 16; 
}

/**
 * Conversion from float to fixed point
 *
 * \param val value in float 
 * \return value in fixed point notation
 */
inline tpFixedType tpFixedFromFloat( tpFloat val ) 
{ 
	return static_cast<int>(val * static_cast<tpFloat>(TP_FIXED_ONE)); 
}

//inline tpInt tpMultiplyFixed(tpInt v1, tpInt v2) {return (v1 * v2) >> TP_FP_PRECISION; }

/**
 * Multiplication between two fixed point values 
 *
 * \param val_fp_a term a fixed point notation
 * \param val_fp_b term b fixed point notation
 * \return result in fixed point notation
 */
tpFixedType tpMultiplyFixed(tpFixedType val_fp_a, tpFixedType val_fp_b) 
{
	return ((tpInt64)val_fp_a * val_fp_b) >> 16; 
}

class TP_API tpFixed {	
public:

	tpFixedType val;

	tpFixed();
	
	tpFixed(const tpFixed& f);
	tpFixed(int v);
	
	int		toInt(bool roundUp = false) const;
	double	toDouble() const;
	float	toFloat() const;
	
	tpFixed mult(tpFixed f);
	
	tpFixed abs() const;
	
	tpFixed& operator += (const tpFixed& rhs) { val += rhs.val; return *this; }
	tpFixed& operator += (const int rhs) { val += tpFixedFromInt(rhs); return *this; }
	tpFixed& operator += (const float rhs) { val += tpFixedFromFloat(rhs); return *this; }
	tpFixed& operator += (const double rhs) { val += tpFixedFromDouble(rhs); return *this; }

	tpFixed& operator *= (const tpFixed& rhs) { val = tpMultiplyFixed(val,rhs.val); return *this; }
	tpFixed& operator *= (const int rhs) { val *= tpFixedFromInt(rhs); return *this; }
	tpFixed& operator *= (const float rhs) { val *= tpFixedFromFloat(rhs); return *this; }
	tpFixed& operator *= (const double rhs) { val *= tpFixedFromDouble(rhs); return *this; }

};


#endif
