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

#ifndef TP_MAT_H
#define TP_MAT_H

#include <tp/fixed.h>
#include <tp/utils.h>
#include <tp/assert.h>


template <tpUInt R, tpUInt C, typename T, bool rowMajor=true> class tpMatRef {
public:

    typedef T value_type;

    static const tpUInt rows = R;
    static const tpUInt cols = C;
    static const tpUInt cells = R*C;
    static const bool rowmajor = rowMajor;

	//! assignment constructor 
    tpMatRef(T* ptr = 0) : mStorage(ptr) {}

	inline tpMatRef& operator = (const tpMatRef& other) {
		if (this != &other)
			for (tpUInt r = 0; r < R;++r)
				for (tpUInt c = 0; c < C;++c)
					(*this)(r,c) = other(r,c);
		return *this;
	}

	//! return pointer to data
    inline const T* data() const { return mStorage; }

    inline T* data() { return mStorage; }

	inline 
	tpMatRef<R,C,T>& 
	fill(const T& val) {
        for (tpUInt i = 0; i < tpMatRef<R,C,T>::cells; ++i) this->at(i) = val; return *this;
    }

    inline T& at(tpUInt idx) { return mStorage[idx]; }
    inline const T& at(tpUInt idx) const { return mStorage[idx]; }

    inline T& operator()(tpUInt r,tpUInt c) { return (rowMajor) ? mStorage[r*C+c] : mStorage[c*R+r]; }
    inline const T& operator()(tpUInt r,tpUInt c) const { return (rowMajor) ? mStorage[r*C+c] : mStorage[c*R+r]; }

    inline void getTranspose(tpMatRef<C,R,T>& transp) const {
        for (register tpUInt r = 0; r < R; r++)
            for (register tpUInt c = 0; c < C; c++ )
                transp(c,r) = (*this)(r,c);
    }

    inline tpUInt getDiagonalSize() const { return tpMin(R,C); }

    inline bool isSquare() const { return C == R; }

	inline tpMatRef& setIdentity() {
        for (tpUInt r = 0; r < R; r++)
            for (tpUInt c = 0; c < C; c++)
                (*this)(r,c) = (r == c) ? T(1) : T(0);
        return *this;
    }

	inline tpMatRef& setCellIdValue() {
        for (register tpUInt i = 0; i < tpMatRef<R,C,T>::cells;++i)
			this->at(i) = static_cast<T>(i);
        return *this;
    }

	inline void transpose() {
        TP_STATIC_ASSERT(R==C)
        for (tpUInt r = 0;r < R; r++)
            for (tpUInt c = r; c < C; ++c) tpSwap((*this)(c,r),(*this)(r,c));
    }

	inline void div(const T& val) { for (tpUInt i = 0; i < tpMatRef<R,C,T>::cells;++i) this->at(i) /= static_cast<T>(val); }

	inline void sub(const T& val) { for (tpUInt i = 0; i < tpMatRef<R,C,T>::cells;++i) this->at(i) -= static_cast<T>(val); }

	inline void mul(const T& val) { for (tpUInt i = 0; i < tpMatRef<R,C,T>::cells;++i) this->at(i) *= static_cast<T>(val); }

	inline void add(const T& val) { for (tpUInt i = 0; i < tpMatRef<R,C,T>::cells;++i) this->at(i) += static_cast<T>(val); }


	inline tpMatRef& operator *= (T val) { this->mul(val); return *this; }

	inline tpMatRef& operator /= (T val) { this->div(val); return *this; }

	inline tpMatRef& operator += (T val) { this->add(val); return *this; }

	inline tpMatRef& operator -= (T val) { this->sub(val); return *this; }

	void setData(T* data) { mStorage = data; }

protected:

    T* mStorage;

};



//////////////////////////////////////////////////////////////////////////

template <tpUInt R, tpUInt C, typename T> class tpMat
        : public tpMatRef<R,C,T,true>
{

public:

    tpMat() : tpMatRef<R,C,T>(&m[0]) {}

    tpMat(const tpMat& mtc) : tpMatRef<R,C,T>(&m[0]) { *this = mtc; }

	inline const tpMat operator - (const tpMat& r) const {
		return tpMat(*this)-=r;
	}

	inline const tpMat operator + (const tpMat& r) const {
		return tpMat(*this)+=r;
	}

	inline void
    getInverse(tpMat& resMat) const
    {
        for ( tpUInt r = 0; r < C; ++r)
        {
            for ( int j = 0; j < R; ++j)
            {
                short sgn = ( (r+j)%2) ? -1 : 1;
                tpMat<R-1,C-1,T> minor;
                this->getMinor(minor,r,j);
                resMat(r,j) = minor.getDeterminant() * sgn;
            }
        }
        resMat.transpose();
        resMat.mul(static_cast<T>((T)1/this->getDeterminant()));
    }

	inline
	tpMat& invert()
	{
		tpMat<R,C,T> resMat;
		this->getInverse(resMat);
		*this = resMat;
		return *this;
	}

    tpMatRef<R,C,T>& operator *= (const T& rhs) { return tpMatRef<R,C,T>::operator*=(rhs); }
    tpMatRef<R,C,T>& operator += (const T& rhs) { return tpMatRef<R,C,T>::operator+=(rhs); }
    tpMatRef<R,C,T>& operator /= (const T& rhs) { return tpMatRef<R,C,T>::operator/=(rhs); }
    tpMatRef<R,C,T>& operator -= (const T& rhs) { return tpMatRef<R,C,T>::operator-=(rhs); }


	void getMinor(tpMat<R-1,C-1,T>& res, tpUInt r0, tpUInt c0) const;

	T getDeterminant() const;

	//tpMat<R,C,T>& operator = (const tpMat<R,C,T>& rhs);

	tpMat<R,C,T>& operator *= (const tpMat<R,C,T>& rhs);


	tpMat<R,C,T>& copyFrom(const T* src) { for (int i = 0; i < tpMat::cells; ++i) { this->m[i] = src[i]; } return *this; }

    tpMat<R,C,T> operator * (const tpMat<R,C,T>& rhs) const {
        return tpMatMul(*this,rhs);
	}

    tpMat<C,R,T>
    reshape() const {
        tpMat<C,R,T> m;
        for (tpUInt r = 0; r < R; ++r)
            for (tpUInt c = 0; c < C; ++c)
                m(r,c) = (*this)(c,r);
        return m;
    }

    tpMat<R,1,T> getColumn(tpUInt col) const {
        tpMat<R,1,T> c; for (tpUInt r = 0; r < R; ++r) c(r,0) = (this)(r,col);
        return c;
    }

    tpMat<1,C,T> getRow(tpUInt row) const {
        tpMat<1,C,T> r; for (tpUInt c = 0; c < C; ++c) r(0,c) = (this)(row,c);
        return r;
    }

    inline
    static const tpMat<R,C,T>
    Identity()
    {
        tpMat<R,C,T> r = tpMat<R,C,T>::All(0);
        for (register tpUInt i = 0; i < r.getDiagonalSize(); ++i) r(i,i) = T(1);
        return r;
    }

    inline
    static tpMat<R,C,T>
    All(T value)
    {
        tpMat<R,C,T> r; r.fill(value);
        return r;
    }

protected:

	T m[R*C];
};


/////////////////////////////////////////////////////////////////////////////

template <tpUInt aR,tpUInt aCbR, tpUInt bC, typename T>
tpMat<aR,bC,T> static inline
tpMatMul(const tpMat<aR,aCbR,T>& A, const tpMat<aCbR,bC,T>& B)
{
    // aC == bR
    // set all null
    tpMat<aR,bC,T> res = tpMat<aR,bC,T>::All(0);

    // compute all resulting cells
    for (tpUInt r = 0; r < aR; ++r) {
        for (tpUInt c = 0; c < bC; ++c) {
            // building inner product
            for (tpUInt iI = 0; iI < aCbR;iI++) {
                res(r,c) += A(r,iI) * B(iI,c);
            }
        }
    }
    return res;
}


//template <tpUInt R, tpUInt C, typename T> 
//tpMat<R,C,T>
//operator + (const tpMat<R,C,T>& A, const tpMat<R,C,T>& B)
//{
//	tpMat<R,C,T> res; for (tpUInt i = 0; i < R*C; ++i) res.at(i) = A.at(i) + B.at(i);
//	return res;
//}
//
//template <tpUInt R, tpUInt C, typename T> 
//tpMat<R,C,T>
//operator - (const tpMat<R,C,T>& A, const tpMat<R,C,T>& B)
//{
//	tpMat<R,C,T> res; for (tpUInt i = 0; i < R*C; ++i) res.at(i) = A.at(i) - B.at(i);
//	return res;
//}


//template <tpUInt R, tpUInt C, typename T>
//tpMat<C,R,T> static inline
//tpMatTranspose(const tpMat<R,C>& A)
//{
//}

/////////////////////////////////////////////////////////////////////////////

//template <tpUInt R, tpUInt C,typename T>
//T& tpMat<R,C,T>::operator()(tpUInt r,tpUInt c) { return m[r * C + c]; }

//template <tpUInt R, tpUInt C,typename T>
//const T& tpMat<R,C,T>::operator()(tpUInt r,tpUInt c) const { return m[r * C + c]; }

//template <tpUInt R, tpUInt C,typename T>
//tpMat<R,C,T>& tpMat<R,C,T>::operator = (const tpMat<R,C,T>& rhs)
//{
//	if (&rhs != this)
//	{
//		for (register tpUInt i = 0; i < tpMat<R,C,T>::cells; i++) {
//			this->m[i] = rhs.m[i];
//
//		}
//	}
//	return *this;
//}

template <tpUInt R, tpUInt C,typename T>
tpMat<R,C,T>& tpMat<R,C,T>::operator *= (const tpMat& rhs)
{
    *this = tpMatMul(*this,rhs);
	return *this;
}


/////////////////////////////////////////////////////////////////////////////


template <tpUInt R, tpUInt C,typename T>
void tpMat<R,C,T>::getMinor(tpMat<R-1,C-1,T>& res,tpUInt r0, tpUInt c0) const
{
	tpUInt r = 0;
	for (tpUInt ri = 0; ri < R; ri++)
	{
		tpUInt c = 0;
		if (ri == r0) continue;
		for (tpUInt ci = 0; ci < C; ci++)
		{
			if (ci == c0) continue;
			res.data()[r*(C-1)+c] = this->m[ri*C + ci];//(*this)(ri,ci);
			c++;
		}
		r++;
	}
}

template <tpUInt R, tpUInt C,typename T> inline
T tpMat<R,C,T>::getDeterminant() const
{
	T res(0);

	tpMat<R-1,C-1,T> minor;

	// using Laplace Expansion at compile time
	for (tpUInt c = 0; c < C; c++)
	{
		this->getMinor(minor,0,c);
		res += ((c % 2 == 0) ? m[c] : -m[c]) *  minor.getDeterminant();
	}

	return res;
}

/////////////////////////////////////////////////////////////////////////////

// explicit specializations

template <typename T>
class tpMat44 : public tpMat<4,4,T>
{
public:

	tpMat44()
	{
	}

	tpMat44(const tpMat<4,4,T>& i)
	{
		*this = i;
	}

	tpMat44&
	operator = (const tpMat<4,4,T>& rhs)
	{
		if (this != &rhs){
			(*this)(0,0) = rhs(0,0);(*this)(0,1) = rhs(0,1);(*this)(0,2) = rhs(0,2);(*this)(0,3) = rhs(0,3);
			(*this)(1,0) = rhs(1,0);(*this)(1,1) = rhs(1,1);(*this)(1,2) = rhs(1,2);(*this)(1,3) = rhs(1,3);
			(*this)(2,0) = rhs(2,0);(*this)(2,1) = rhs(2,1);(*this)(2,2) = rhs(2,2);(*this)(2,3) = rhs(2,3);
			(*this)(3,0) = rhs(3,0);(*this)(3,1) = rhs(3,1);(*this)(3,2) = rhs(3,2);(*this)(3,3) = rhs(3,3);
		}
		return *this;
	}

	tpMat<4,4,T>&
	translate(const T& v1,const T& v2,const T& v3)
	{
		this->m[12] += v1;
		this->m[13] += v2;
		this->m[14] += v3;

		return *this;
	}

	tpMat<4,4,T>&
	setTranslation(const T& v1,const T& v2,const T& v3)
	{
        this->setIdentity();
		this->m[12] = v1;
		this->m[13] = v2;
		this->m[14] = v3;
		return *this;
	}

	//tpMatRef<3,1,T>
	//getTranslation() const
	//{
	//	return tpVec3<T>(this->m[12],this->m[13],this->m[14]);
	//}

	tpMat<4,4,T>&
	setScale(const T& v1,const T& v2,const T& v3)
	{
		this->identity();
		(*this)(0,0) = v1;
		(*this)(1,1) = v2;
		(*this)(2,2) = v3;
		return *this;
	}

	tpMat<4,4,T>&
	scale(const T& v1,const T& v2,const T& v3)
	{
		(*this)(0,0) *= v1;
		(*this)(1,1) *= v2;
		(*this)(2,2) *= v3;
		return *this;
	}

	//tpMat<4,4,T>&
	//setRotation(const tpVec3<T>& vec, const T& rotation)
	//{
	//	this->identity();

	//	if (vec.getLength() < T(.000001f)) return *this;

	//	T _radiant = tpDeg2Rad(rotation);

	//	T _fCos = (T) cos (_radiant);

	//	tpVec<T,3> _vCos = vec * (1 - _fCos);
	//	tpVec<T,3> _vSin = vec * (T)sin(_radiant);

	//	this->m[0]= (T) ((vec[0] * _vCos[0]) + _fCos);
	//	this->m[4]= (T) ((vec[0] * _vCos[1]) - _vSin[2]);
	//	this->m[8]= (T) ((vec[0] * _vCos[2]) + _vSin[1]);

	//	this->m[1]= (T) ((vec[1] * _vCos[0]) + _vSin[2]);
	//	this->m[5]= (T) ((vec[1] * _vCos[1]) + _fCos);
	//	this->m[9]= (T) ((vec[1] * _vCos[2]) - _vSin[0]);

	//	this->m[2]= (T)  ((vec[2] * _vCos[0]) - _vSin[1]);
	//	this->m[6]= (T)  ((vec[2] * _vCos[1]) + _vSin[0]);
	//	this->m[10]= (T) ((vec[2] * _vCos[2]) + _fCos);

	//	this->m[3] = this->m[7] = this->m[11] = T(0);

	//	this->m[15] = T(1);

	//	return *this;
	//}

	//tpMat<4,4,T>&
	//rotate(const tpVec3<T>& vec, const T& rotation)
	//{
	//	tpMat<4,4,T> rot; rot.setRotation(vec,rotation); *this *= rot;
	//	return *this;
	//}
};

//////////////////////////////////////////////////////////////////////////

//
// Specializations
//

template <> inline
float tpMat<1,1,float>::getDeterminant() const
{
	return this->m[0];
}

template <> inline
tpFixed32 tpMat<1,1,tpFixed32>::getDeterminant() const
{
	return this->m[0];
}


//////////////////////////////////////////////////////////////////////////

// predefined matricies

typedef tpMat44<tpReal> tpMat44r;
typedef tpMat44<tpDouble> tpMat44d;
typedef tpMat44<tpFloat> tpMat44f;
typedef tpMat44<tpFixed32> tpMat44x;


#endif
