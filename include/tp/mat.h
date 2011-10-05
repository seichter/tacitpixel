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

#include <tp/vec.h>
#include <tp/fixed.h>

template <tpUInt R, tpUInt C, typename T> class tpMat {
public:

	typedef T value_type;

	static const tpUInt rows = R;
	static const tpUInt cols = C;
	static const tpUInt cells = R*C;

	tpMat() {}

	tpMat(const tpMat<R,C,T>& mtc) { *this = mtc; }

	tpMat(const T* val) { for (int i = 0; i < tpMat<R,C,T>::cells; ++i) m[i] = val[i]; }

	template <typename Tout>
	void copy(tpMat<R,C,Tout>& out) const { for (tpUInt i = 0; i < tpMat<R,C,T>::cells; ++i) out[i] = Tout(m[i]); }

	inline const T*
	data() const { return m; }

	inline T*
	data() { return m; }

	bool isSquare() const { return C == R; }

	tpMat<R,C,T>& all(const T& val = T(0)) { for (int i = 0; i < tpMat<R,C,T>::cells; ++i) m[i] = val; return *this; }

	tpMat<R,C,T>& identity()
	{
		if (isSquare())
		{
			all(0);
			for (int i = 0; i < R; ++i)
			{
				m[i*C+i] = T(1);
			}
		}
		return *this;
	}

	void setCellIdValue()
	{
		for (register int i = 0; i < tpMat<R,C,T>::cells;++i) {m[i] = i;}
	}

	inline tpMat<R,C,T>&
	transpose()
	{
		tpMat<C,R,T> r;
		this->getTranspose(r);
		*this = r;
		return *this;
	}

	inline void
	getTranspose(tpMat<C,R,T>& rot) const
	{
		for (register tpUInt r = 0; r < rows; r++)
			for (register tpUInt c = 0; c < cols; c++ )
			{
				rot(c,r) = (*this)(r,c);
			}
	}

	inline tpMat<R,C,T>
	multiply(const tpMat<R,C,T>& rhs) const
	{
		tpMat<R,C,T> res;
		for (tpUInt r = 0; r < R; ++r)
		{
			for (tpUInt c = 0; c < C; ++c)
			{
				T& v = res(r,c) = T(0);
				for (tpUInt k = 0; k < C; ++k)
				{
					v += (*this)(r,k) * rhs(k,c);
				}
			}
		}
		return res;
	}

	inline void
	getInverse(tpMat<R,C,T>& resMat) const
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
		resMat *= T(1)/getDeterminant();
	}

	inline
	tpMat<R,C,T>& invert()
	{
		tpMat<R,C,T> resMat;
		this->getInverse(resMat);
		*this = resMat;
		return *this;
	}

	void getMinor(tpMat<R-1,C-1,T>& res, tpUInt r0, tpUInt c0) const;

	T getDeterminant() const;

	T& at(tpUInt idx) { return m[idx]; }
	const T& at(tpUInt idx) const { return m[idx]; }

	T& operator()(tpUInt r,tpUInt c);
	const T& operator()(tpUInt r,tpUInt c) const;

	tpMat<R,C,T>& operator = (const tpMat<R,C,T>& rhs);

	tpMat<R,C,T>& operator *= (const tpMat<R,C,T>& rhs);

	tpMat<R,C,T>& operator *= (const T& rhs);

	tpMat<R,C,T>& copyFrom(const T* ptr) { for (int i = 0; i < tpMat::cells; ++i) { this->at(i) = ptr[i]; } return *this; }

protected:

	T m[R*C];
};


/////////////////////////////////////////////////////////////////////////////

template <tpUInt R, tpUInt C,typename T>
T& tpMat<R,C,T>::operator()(tpUInt r,tpUInt c) { return m[r * C + c]; }

template <tpUInt R, tpUInt C,typename T>
const T& tpMat<R,C,T>::operator()(tpUInt r,tpUInt c) const { return m[r * C + c]; }

template <tpUInt R, tpUInt C,typename T>
tpMat<R,C,T>& tpMat<R,C,T>::operator = (const tpMat<R,C,T>& rhs)
{
	if (&rhs != this)
	{
		for (register tpUInt i = 0; i < tpMat<R,C,T>::cells; i++) {
			this->m[i] = rhs.m[i];
		}
	}
	return *this;
}

template <tpUInt R, tpUInt C,typename T>
tpMat<R,C,T>& tpMat<R,C,T>::operator *= (const tpMat<R,C,T>& rhs)
{
	*this = this->multiply(rhs);
	return *this;
}

template <tpUInt R, tpUInt C,typename T>
tpMat<R,C,T>& tpMat<R,C,T>::operator *= (const T& rhs)
{
	for (tpUInt i = 0; i < tpMat<R,C,T>::cells; i++)
	{
		m[i] *= rhs;
	}
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

// partial specializations

//
// Mat 4x4
//
template <typename T>
class tpMat44 : public tpMat<4,4,T> {
public:

	tpMat44<T>&
	translate(const T& v1,const T& v2,const T& v3)
	{
		this->m[12] += v1;
		this->m[13] += v2;
		this->m[14] += v3;

		return *this;
	}

	tpMat44<T>&
	setTranslation(const T& v1,const T& v2,const T& v3)
	{
		this->identity();
		this->m[12] = v1;
		this->m[13] = v2;
		this->m[14] = v3;
		return *this;
	}

	tpVec3<T>
	getTranslation() const
	{
		return tpVec3<T>(this->m[12],this->m[13],this->m[14]);
	}

	tpMat44<T>&
	setScale(const T& v1,const T& v2,const T& v3)
	{
		this->identity();
		this->m[ 0] = v1;
		this->m[ 5] = v2;
		this->m[10] = v3;
		return *this;
	}

	tpMat44<T>&
	scale(const T& v1,const T& v2,const T& v3)
	{
		this->m[ 0] *= v1;
		this->m[ 5] *= v2;
		this->m[10] *= v3;
		return *this;
	}

	tpMat44<T>&
	setRotation(const tpVec3<T>& vec, const T& rotation)
	{
		this->identity();

		if (vec.getLength() < T(.000001f)) return *this;

		T _radiant = tpDeg2Rad(rotation);

		T _fCos = (T) cos (_radiant);

		tpVec<T,3> _vCos = vec * (1 - _fCos);
		tpVec<T,3> _vSin = vec * (T)sin(_radiant);

		this->m[0]= (T) ((vec[0] * _vCos[0]) + _fCos);
		this->m[4]= (T) ((vec[0] * _vCos[1]) - _vSin[2]);
		this->m[8]= (T) ((vec[0] * _vCos[2]) + _vSin[1]);

		this->m[1]= (T) ((vec[1] * _vCos[0]) + _vSin[2]);
		this->m[5]= (T) ((vec[1] * _vCos[1]) + _fCos);
		this->m[9]= (T) ((vec[1] * _vCos[2]) - _vSin[0]);

		this->m[2]= (T)  ((vec[2] * _vCos[0]) - _vSin[1]);
		this->m[6]= (T)  ((vec[2] * _vCos[1]) + _vSin[0]);
		this->m[10]= (T) ((vec[2] * _vCos[2]) + _fCos);

		this->m[3] = this->m[7] = this->m[11] = T(0);

		this->m[15] = T(1);

		return *this;
	}

	tpMat44<T>&
	rotate(const tpVec3<T>& vec, const T& rotation)
	{
		tpMat44<T> rot; rot.setRotation(vec,rotation); *this *= rot;
		return *this;
	}

	T getDeterminant() const
	{
		return
			(this->m[0]*this->m[5]  - this->m[4]  *this->m[1])*(this->m[10]*this->m[15] - this->m[14]*this->m[11])
			-(this->m[0]*this->m[9]  - this->m[8]  *this->m[1])*(this->m[ 6]*this->m[15] - this->m[14]*this->m[7])
			+(this->m[0]*this->m[13] - this->m[12] *this->m[1])*(this->m[ 6]*this->m[11] - this->m[10]*this->m[7])
			+(this->m[4]*this->m[9]  - this->m[8]  *this->m[5])*(this->m[ 2]*this->m[15] - this->m[14]*this->m[3])
			-(this->m[4]*this->m[13] - this->m[12] *this->m[5])*(this->m[ 2]*this->m[11] - this->m[10]*this->m[3])
			+(this->m[8]*this->m[13] - this->m[12] *this->m[9])*(this->m[ 2]*this->m[7] - this->m[6]*this->m[3]);
	}

#if 0
	tpMat44&
	invert()
	{
		T tmp[12]; /* temp array for pairs             */
		T src[16]; /* array of transpose source matrix */
		T det;     /* determinant*/

		/* transpose matrix */
		for (int i = 0; i < 4; i++) {
			src[i]         = this->m[i*4];
			src[i + 4]     = this->m[i*4 + 1];
			src[i + 8]     = this->m[i*4 + 2];
			src[i + 12]    = this->m[i*4 + 3];
		}

		/* calculate pairs for first 8 elements (cofactors) */
		tmp[0]       =   src[10]         * src[15];
		tmp[1]       =   src[11]         * src[14];
		tmp[2]       =   src[9]          * src[15];
		tmp[3]       =   src[11]         * src[13];
		tmp[4]       =   src[9]          * src[14];
		tmp[5]       =   src[10]         * src[13];
		tmp[6]       =   src[8]          * src[15];
		tmp[7]       =   src[11]         * src[12];
		tmp[8]       =   src[8]          * src[14];
		tmp[9]       =   src[10]         * src[12];
		tmp[10]      =   src[8]          * src[13];
		tmp[11]      =   src[9]          * src[12];

		/* calculate first 8 elements (cofactors) */
		this->m[0]         =   tmp[0]*src[5]           + tmp[3]*src[6] + tmp[4]*src[7];
		this->m[0]       -=    tmp[1]*src[5]           + tmp[2]*src[6] + tmp[5]*src[7];
		this->m[1]         =   tmp[1]*src[4]           + tmp[6]*src[6] + tmp[9]*src[7];
		this->m[1]       -=    tmp[0]*src[4]           + tmp[7]*src[6] + tmp[8]*src[7];
		this->m[2]         =   tmp[2]*src[4]           + tmp[7]*src[5] + tmp[10]*src[7];
		this->m[2]       -=    tmp[3]*src[4]           + tmp[6]*src[5] + tmp[11]*src[7];
		this->m[3]         =   tmp[5]*src[4]           + tmp[8]*src[5] + tmp[11]*src[6];
		this->m[3]       -=    tmp[4]*src[4]           + tmp[9]*src[5] + tmp[10]*src[6];
		this->m[4]          =  tmp[1]*src[1]           + tmp[2]*src[2] + tmp[5]*src[3];
		this->m[4]       -=    tmp[0]*src[1]           + tmp[3]*src[2] + tmp[4]*src[3];
		this->m[5]          =  tmp[0]*src[0]           + tmp[7]*src[2] + tmp[8]*src[3];
		this->m[5]       -=    tmp[1]*src[0]           + tmp[6]*src[2] + tmp[9]*src[3];
		this->m[6]          =  tmp[3]*src[0]           + tmp[6]*src[1] + tmp[11]*src[3];
		this->m[6]       -=    tmp[2]*src[0]           + tmp[7]*src[1] + tmp[10]*src[3];
		this->m[7]          =  tmp[4]*src[0]           + tmp[9]*src[1] + tmp[10]*src[2];
		this->m[7]       -=    tmp[5]*src[0]           + tmp[8]*src[1] + tmp[11]*src[2];
		/* calculate pairs for second 8 elements (cofactors) */
		tmp[0]         =   src[2]*src[7];
		tmp[1]         =   src[3]*src[6];
		tmp[2]         =   src[1]*src[7];
		tmp[3]         =   src[3]*src[5];
		tmp[4]	       =   src[1]*src[6];
		tmp[5]         =   src[2]*src[5];
		tmp[6]  	= src[0]*src[7];
		tmp[7]  	= src[3]*src[4];
		tmp[8]		= src[0]*src[6];
		tmp[9]		= src[2]*src[4];
		tmp[10]		= src[0]*src[5];
		tmp[11]		= src[1]*src[4];

		/* calculate second 8 elements (cofactors) */
		this->m[8] =         tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
		this->m[8] -=        tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
		this->m[9] =         tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
		this->m[9] -=        tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
		this->m[10] =        tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
		this->m[10]-=        tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
		this->m[11] =        tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
		this->m[11]-=        tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
		this->m[12] =        tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
		this->m[12]-=        tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
		this->m[13] =        tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
		this->m[13]-=        tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
		this->m[14] =        tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
		this->m[14]-=        tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
		this->m[15] =        tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
		this->m[15]-=        tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];

		/* calculate determinant */
		det=src[0]*this->m[0]+src[1]*this->m[1]+src[2]*this->m[2]+src[3]*this->m[3];

		/* calculate matrix inverse */
		det = T(1)/det;
		for (int j = 0; j < 16; j++)
			this->m[j] *= det;

		return *this;
	}

#endif

};



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

class tpMat44r : public tpMat44<tpReal> {};
class tpMat44d : public tpMat44<tpDouble> {};
class tpMat44f : public tpMat44<tpFloat> {};
class tpMat44x : public tpMat44<tpFixed32> {};


class tpVector3d : public tpMat<1,3,tpDouble> {};



#endif
