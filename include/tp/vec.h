#ifndef TPVEC_H
#define TPVEC_H


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

	//! c'tor initializes all to 0
	tpVec();

	//! copy constructor
	tpVec(const tpVec& v);

	//! d'tor
	~tpVec();

	//! get the dimension of a vector
	tpUInt dimension() const;

	//! get the raw vector data
	const T* getData() const;

	//! substract a vector
	tpVec operator -(const tpVec& rs) const;
	//! add a vector
	tpVec operator +(const tpVec& rs) const;

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
	
	//! assignment operator
	tpVec& operator = (const tpVec& rs);

	//! assignment operator using raw data 
	tpVec& operator = (const T* rs);

	//! comparison
	bool operator == (const tpVec<T,N>& rs) const;
	
	//! inverted comparison
	bool operator != (const tpVec<T,N>& rs) const;

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
	T& operator [] (const tpUInt& i);
	
	//! swap components in the vector
	tpVec<T,N>& swapComponent(tpUInt c1,tpUInt c2);

	//template <tpUInt cut> inline 
	//void slice(tpVec<T,cut>& front, tpVec<T,N-cut>& back) const
	//{
	//	cu
	//}

protected:
	T vec[N];
};

// Vec -----------------------------------------------------------------------------------------

template <class T,tpUInt N> inline tpVec<T,N>::tpVec()
{
	for (tpUInt i = 0; i < N; i++) vec[i] = 0;
};

template <class T,tpUInt N> inline tpVec<T,N>::tpVec(const tpVec& v)
{
	for (tpUInt i = 0; i < N; i++) this->vec[i] = v.vec[i];
};


template <class T,tpUInt N> inline tpVec<T,N>::~tpVec() 
{
};

template <class T,tpUInt N> inline tpUInt tpVec<T,N>::dimension() const
{
	return N;
};

template <class T,tpUInt N> inline const T* tpVec<T,N>::getData() const 
{ 
	return (T*)&vec; 
};


template <class T,tpUInt N> inline tpVec<T,N> tpVec<T,N>::operator - (const tpVec<T,N>& rs) const
{
	tpVec<T,N> out;
	for (tpUInt i = 0; i < N; i++) out.vec[i] = vec[i] - rs.vec[i];
	return out;
};

template <class T,tpUInt N> inline tpVec<T,N> tpVec<T,N>::operator +(const tpVec& rs) const
{
	tpVec<T,N> out;
	for (tpUInt i = 0; i < N; i++) out.vec[i] = vec[i] + rs.vec[i];
	return out;
};

template <class T,tpUInt N> inline tpVec<T,N>& tpVec<T,N>::operator -= (const tpVec<T,N>& v)
{
	for (tpUInt i = 0; i < N; i++) vec[i] -= v.vec[i];
	return *this;
};

template <class T,tpUInt N> inline tpVec<T,N>& tpVec<T,N>::operator += (const tpVec& v)
{
	for (tpUInt i = 0; i < N; i++) vec[i] += v.vec[i];
	return *this;
};


template <class T,tpUInt N> void tpVec<T,N>::set(const tpVec<T,N>& v)
{
	for (tpUInt i = 0; i < N; i++) vec[i] = v.vec[i];
};


template <class T,tpUInt N> void tpVec<T,N>::set(const T* buf)
{
	for (tpUInt i =0; i < N; ++i) vec[i] = *buf[i];
};

template <class T,tpUInt N> T tpVec<T,N>::dot(const tpVec<T,N>& v) const
{
	T res = 0;
	for (tpUInt i = 0; i < N; i++) res = res + (vec[i] * v.vec[i]);
	return res;
}

template <class T,tpUInt N> T tpVec<T,N>::getAngle(tpVec<T,N> v) const
{
	tpVec<T,N> nself = *this;
	tpVec<T,N> nothr = v;
	nself.normalize();
	nothr.normalize();
	return (T)acos( nothr.dot(nself) );
}

template <class T,tpUInt N> inline tpVec<T,N>& tpVec<T,N>::operator = (const tpVec& rs)
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

template <class T,tpUInt N> inline void tpVec<T,N>::normalize()
{
	T _length = getLength();
	if (_length > 0)
	{
		for (tpUInt i = 0; i < N; i++) vec[i] /= _length;//vec[i] = T(vec[i] / _length);
	}
}

template <class T,tpUInt N> inline T tpVec<T,N>::getLength() const
{
	return tpSqrt(getSquareLength());
};

template <class T,tpUInt N> inline T tpVec<T,N>::getSquareLength() const
{
	T sum = 0;
	for (tpUInt i = 0; i < N; i++) sum = sum + (vec[i] * vec[i]);
	return sum;
};

template <class T,tpUInt N> inline tpVec<T,N>& tpVec<T,N>::scaleUniform(T scale)
{
	for (tpUInt i = 0; i < N; i++) vec[i] = vec[i] * scale;
	return *this;
};

template <class T,tpUInt N> inline tpVec<T,N> tpVec<T,N>::operator* (T scale) const
{
	tpVec<T,N> v = *this;
	v.scaleUniform(scale);
	return v;
};

template <class T,tpUInt N> inline T& tpVec<T,N>::operator [] (const tpUInt& i)
{
	return vec[i];
};


template <class T,tpUInt N> inline tpVec<T,N>& tpVec<T,N>::swapComponent(tpUInt c1,tpUInt c2) 
{
	T _swp = vec[c1];
	vec[c1] = vec[c2];
	vec[c2] = _swp;
	
	return *this;
};





// Vec3 -----------------------------------------------------------------------

template <class T> class tpVec3 : public tpVec<T,3>
{
public:
	tpVec3();
	tpVec3(T c1, T c2, T c3);

	void set(T c1, T c2, T c3);


	tpVec3 operator - (const tpVec3<T>& rs);

	tpVec3 operator + (const tpVec3<T>& rs);


	tpVec3 cross(const tpVec3&) const;
};

// Vec3 Implementation

template <class T> inline tpVec3<T>::tpVec3()
{
	this->vec[0] = this->vec[1] = this->vec[2] = (T)0;
};


template <class T> inline tpVec3<T>::tpVec3(T c1, T c2, T c3)
{
	this->vec[0] = c1; this->vec[1] = c2; this->vec[2] = c3;
};



template <class T> inline void tpVec3<T>::set(T c1, T c2, T c3)
{
	this->vec[0] = c1; this->vec[1] = c2; this->vec[2] = c3;
};


template <class T> inline tpVec3<T> tpVec3<T>::operator - (const tpVec3<T>& rs)
{
	tpVec3<T> out;

	out.vec[0] = this->vec[0] - rs.vec[0];
	out.vec[1] = this->vec[1] - rs.vec[1];
	out.vec[2] = this->vec[2] - rs.vec[2];

	return out;
};

template <class T> inline tpVec3<T> tpVec3<T>::operator + (const tpVec3<T>& rs)
{
	tpVec3<T> out;

	out.vec[0] = this->vec[0] + rs.vec[0];
	out.vec[1] = this->vec[1] + rs.vec[1];
	out.vec[2] = this->vec[2] + rs.vec[2];

	return out;
};

template <class T> inline tpVec3<T> tpVec3<T>::cross(const tpVec3<T>& vec2) const
{
	T _v1 = this->vec[1] * vec2.vec[2] - vec2.vec[1] * this->vec[2];
	T _v2 = this->vec[2] * vec2.vec[0] - vec2.vec[2] * this->vec[0];
	T _v3 = this->vec[0] * vec2.vec[1] - vec2.vec[0] * this->vec[1];

	return tpVec3<T>(_v1,_v2,_v3);
};



// Vec2x -----------------------------------------------------------------------

/*!	\brief a two dimensional vector
*/
template <class T> class tpVec2 : public tpVec<T,2> 
{
public:

	tpVec2()
	{
		this->vec[0] = this->vec[1] = (T)0;
	};

	tpVec2(T v1,T v2)
	{
		this->vec[0] = v1; this->vec[1] = v2;
	};

	void set(T v1,T v2)
	{
		this->vec[0] = v1; this->vec[1] = v2;
	};
};

typedef tpVec2<tpDouble> tpVec2d;
typedef tpVec2<tpFloat> tpVec2f;
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
		this->vec[0] = this->vec[1] = this->vec[2] = this->vec[3] = (T)0;
	}

	tpVec4(const tpVec3<T>& rv,T pad = T(0)) 
	{
		this->vec[0] = rv.vec[0]; this->vec[1] = rv.vec[1]; this->vec[2] = rv.vec[2]; this->vec[3] = pad;
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
typedef tpVec4<tpDouble> tpVec4d;
typedef tpVec4<tpFloat> tpVec4f;
typedef tpVec4<tpInt> tpVec4i;


// Quaternion
template <typename T> class tpQuat : public tpVec<T,4>
{
public:

	tpVec3<T> rotate( const tpVec3<T>& v )
	{
		tpVec3<T> result; // = v + 2 * v.cross(

		
	}

};

#endif
