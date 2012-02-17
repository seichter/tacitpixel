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

#ifndef TP_VEC_H
#define TP_VEC_H

/*!
	\class tpVec
	\brief base class for vectors

	This is the base class for vector
	calculations.
*/

#include <tp/math.h>

template <class T, tpUInt N> class tpVec
{
public:

	typedef T value_type;
	const static tpUInt dimensions = N;

	//! c'tor initializes all to 0
	tpVec();

	//! copy constructor
	tpVec(const tpVec<T,N>& v);

	//! d'tor
	~tpVec();

	//! get the dimension of a vector
	tpUInt dimension() const;

	//! get the raw vector data
	const T* getData() const { return &vec[0]; }

	//! get the raw vector data
	T* getData() { return &vec[0]; }

	//! substract a vector
	const tpVec operator -(const tpVec& rs) const;
	//! add a vector
	const tpVec operator +(const tpVec& rs) const;

	//! substract from self vector
	tpVec& operator -= (const tpVec& v);
	//! add to the vector
	tpVec& operator += (const tpVec& v);

	//! copy data from other representations
	void set(const tpVec& v);

	//! copy data from simple buffer
	void set(const T* buf);

	//! dot product with other vector
	T dot(const tpVec<T,N>& v) const;

	//! get the angle between this and another vector
	T getAngle(tpVec<T,N> v) const;

	//! normalizes this vector
	void normalize();

	//! return length
	T getLength() const;

	//! returns squared length
	T getSquareLength() const;

	//! uniformly scales the vector
	tpVec<T,N>& scaleUniform(T scale);

	//! another uniform scale
	tpVec<T,N> operator* (T scale) const;

	//! return the component at position i
	T& operator [] (const tpUInt& i) { return vec[i]; }

	//! return the component at position i
	const T& operator [] (const tpUInt& i) const { return vec[i]; }


	//! swap components in the vector
	tpVec<T,N>& swapComponent(tpUInt c1,tpUInt c2);

	inline const tpVec<T,N> operator - () const
	{
		tpVec<T,N> r;
		for (int i = 0; i < N; ++i) { r.vec[i] = -this->vec[i]; }
		return r;
	}

	tpVec<T,N>& operator -= (const tpVec<T,N>& rhs) const
	{
		for (int i = 0; i < N; ++i) { this->vec[i] -= rhs.vec[i]; }
		return *this;
	}

	tpVec<T,N>& operator += (const tpVec<T,N>& rhs) const
	{
		for (int i = 0; i < N; ++i) { this->vec[i] += rhs.vec[i]; }
		return *this;
	}


	//! assignment operator
	tpVec& operator = (const tpVec& rs);

	//! assignment operator using raw data
	tpVec& operator = (const T* rs);

	//! comparison
	bool operator == (const tpVec<T,N>& rs) const;

	//! inverted comparison
	bool operator != (const tpVec<T,N>& rs) const;


protected:
	T vec[N];
};


// Vec -----------------------------------------------------------------------------------------

template <class T,tpUInt N> inline
tpVec<T,N>::tpVec()
{
	for (tpUInt i = 0; i < N; i++) vec[i] = T(0);
}

template <class T,tpUInt N> inline
tpVec<T,N>::tpVec(const tpVec& v)
{
	for (tpUInt i = 0; i < N; i++) this->vec[i] = v.vec[i];
}


template <class T,tpUInt N> inline
tpVec<T,N>::~tpVec()
{
}

template <class T,tpUInt N> inline
tpVec<T,N>& tpVec<T,N>::operator -= (const tpVec<T,N>& v)
{
	for (tpUInt i = 0; i < N; i++) vec[i] -= v.vec[i];
	return *this;
}

template <class T,tpUInt N> inline
tpVec<T,N>& tpVec<T,N>::operator += (const tpVec& v)
{
	for (tpUInt i = 0; i < N; i++) vec[i] += v.vec[i];
	return *this;
}

template <class T,tpUInt N> inline
void tpVec<T,N>::set(const tpVec<T,N>& v)
{
	for (tpUInt i = 0; i < N; i++) vec[i] = v.vec[i];
}

template <class T,tpUInt N> inline
void tpVec<T,N>::set(const T* buf)
{
	for (tpUInt i =0; i < N; ++i) vec[i] = *buf[i];
}

template <class T,tpUInt N> inline
T tpVec<T,N>::dot(const tpVec<T,N>& v) const
{
	T res = 0;
	for (tpUInt i = 0; i < N; i++) res = res + (vec[i] * v.vec[i]);
	return res;
}

template <class T,tpUInt N> inline
T tpVec<T,N>::getAngle(tpVec<T,N> v) const
{
	tpVec<T,N> nself = *this;
	tpVec<T,N> nothr = v;
	nself.normalize();
	nothr.normalize();
	return acos( nothr.dot(nself) );
}

template <class T,tpUInt N>
inline tpVec<T,N>& tpVec<T,N>::operator = (const tpVec& rs)
{
	for (tpUInt i = 0; i < N; i++) this->vec[i] = rs.vec[i];
	return *this;
}

template <class T,tpUInt N> inline tpVec<T,N>& tpVec<T,N>::operator = (const T* rs)
{
	this->vec = rs;
	return *this;
}

template <class T,tpUInt N> inline bool tpVec<T,N>::operator == (const tpVec<T,N>& rs) const
{
	if (this == &rs) return true;
	for (tpUInt i = 0; i < N; i++) if (rs.vec[i] != vec[i]) return false;
	return true;
}

template <class T,tpUInt N> inline bool tpVec<T,N>::operator != (const tpVec<T,N>& rs) const
{
	return ! operator==(rs);
}

template <class T,tpUInt N> inline
void tpVec<T,N>::normalize()
{
	T _length = getLength();
	if (_length > 0)
	{
		for (register tpUInt i = 0; i < N; i++) vec[i] /= _length;//vec[i] = T(vec[i] / _length);
	}
}

template <class T,tpUInt N> inline
T tpVec<T,N>::getLength() const
{
	return sqrt(getSquareLength());
}

template <class T,tpUInt N> inline
T tpVec<T,N>::getSquareLength() const
{
	T sum = 0;
	for (register tpUInt i = 0; i < N; i++) sum += (vec[i] * vec[i]);
	return sum;
}

template <class T,tpUInt N> inline tpVec<T,N>& tpVec<T,N>::scaleUniform(T scale)
{
	for (tpUInt i = 0; i < N; i++) vec[i] = vec[i] * scale;
	return *this;
}

template <class T,tpUInt N> inline
tpVec<T,N> tpVec<T,N>::operator* (T scale) const
{
	tpVec<T,N> v = *this;
	v.scaleUniform(scale);
	return v;
}

template <class T,tpUInt N> inline
tpVec<T,N>& tpVec<T,N>::swapComponent(tpUInt c1,tpUInt c2)
{
	T _swp = vec[c1];
	vec[c1] = vec[c2];
	vec[c2] = _swp;

	return *this;
}

// Generic Operators ---------------------------------------------------------

template <class T, tpUInt N> inline
const tpVec<T,N> tpVec<T,N>::operator + (const tpVec<T,N>& r) const
{
	return tpVec<T,N>(*this)+=r;
}

template <class T, tpUInt N> inline
const tpVec<T,N> tpVec<T,N>::operator - (const tpVec<T,N>& r) const
{
	return tpVec<T,N>(*this)-=r;
}



// Vec3 -----------------------------------------------------------------------

template <class T>
class tpVec3 : public tpVec<T,3>
{
public:

	tpVec3() { this->vec[0] = T(); this->vec[1] = T(); this->vec[2] = T(); }

	tpVec3(const tpVec<T,3>& in) { this->vec[0] = in[0]; this->vec[1] = in[1]; this->vec[2] = in[2]; }

	tpVec3(T c1, T c2, T c3) {this->vec[0] = c1; this->vec[1] = c2; this->vec[2] = c3;}

	void set(T c1, T c2, T c3) { this->vec[0] = c1; this->vec[1] = c2; this->vec[2] = c3; }

	tpVec3 cross(const tpVec3<T>&) const;
};

// Vec3 Implementation

template <class T> inline tpVec3<T> tpVec3<T>::cross(const tpVec3<T>& vec2) const
{
	T _v1 = this->vec[1] * vec2.vec[2] - vec2.vec[1] * this->vec[2];
	T _v2 = this->vec[2] * vec2.vec[0] - vec2.vec[2] * this->vec[0];
	T _v3 = this->vec[0] * vec2.vec[1] - vec2.vec[0] * this->vec[1];

	return tpVec3<T>(_v1,_v2,_v3);
}



// Vec2x -----------------------------------------------------------------------

/*!	\brief a two dimensional vector
*/
template <class T> class tpVec2 : public tpVec<T,2>
{
public:

	tpVec2()
	{
		this->vec[0] = this->vec[1] = (T)0;
	}

	tpVec2(T v1,T v2)
	{
		this->vec[0] = v1; this->vec[1] = v2;
	}

	void set(T v1,T v2)
	{
		this->vec[0] = v1; this->vec[1] = v2;
	}
};

typedef tpVec2<tpDouble> tpVec2d;
typedef tpVec2<tpFloat> tpVec2f;
typedef tpVec2<tpReal> tpVec2r;
typedef tpVec2<tpInt> tpVec2i;






typedef tpVec3<tpReal> tpVec3r;
typedef tpVec3<tpDouble> tpVec3d;
typedef tpVec3<tpFloat> tpVec3f;
typedef tpVec3<tpInt> tpVec3i;
typedef tpVec3<tpUInt> tpVec3ui;



// Vec4 -----------------------------------------------------------------------

template <typename T> class tpVec4 : public tpVec<T,4>
{
public:

	tpVec4()
	{
		this->vec[0] = this->vec[1] = this->vec[2] = this->vec[3] = T(0);
	}

    tpVec4(const tpVec<T,3>& rv,T pad = T(0))
	{
        this->vec[0] = rv[0]; this->vec[1] = rv[1]; this->vec[2] = rv[2]; this->vec[3] = pad;
	}

	tpVec4(T v1,T v2,T v3,T v4)
	{
		this->vec[0] = v1; this->vec[1] = v2; this->vec[2] = v3; this->vec[3] = v4;
	}

	void set(T v1,T v2,T v3,T v4)
	{
		this->vec[0] = v1; this->vec[1] = v2; this->vec[2] = v3; this->vec[3] = v4;
	}


	tpVec3<T> xyz() const
	{
		return tpVec3<T>(this->vec[0],this->vec[1],this->vec[2]);
	}

	tpVec2<T> xy() const
	{
		return tpVec2<T>(this->vec[0],this->vec[1]);
	}


};

typedef tpVec4<tpReal> tpVec4r;
typedef tpVec4<tpInt> tpVec4i;

typedef tpVec4<tpFloat> tpVec4f;
typedef tpVec4<tpDouble> tpVec4d;



// Quaternion
template <typename T>
class tpQuat : public tpVec<T,4> {
public:

	tpVec3<T> rotate( const tpVec3<T>& v )
	{
		tpVec3<T> result; // = v + 2 * v.cross(


	}
};


#endif
