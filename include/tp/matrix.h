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

#ifndef TPMATRIX_H
#define TPMATRIX_H


#include <tp/math.h>
#include <tp/fixed.h>

/*!
 
 \class tpMatrix
 \brief a template for a colum-order matrix (OpenGL)
 
 The matrix in libTAP is a column-order matrix as known
 in OpenGL, due to the fact that this is the main rendering
 system to be used. Because it is using a single array, the
 conversion for DirectX should be painless as it is only
 flipped. 
 
 \code
 row ->
 1 0 0 0			c0	c4	c8	c12 
 0 1 0 0			c1	c5	c9	c13
 0 0 1 0			c2	c6	c10	c14
 0 0 0 1			c3	c7	c11	c15
 \endcode
 
 */

template <class T> class tpMatrix {
public:
	
	typedef T value_type;
	
	//! c'tor
	explicit tpMatrix();
	
	tpMatrix(const T* raw);
	
	//! copy c'tor
	tpMatrix(const tpMatrix<T>&);
	//! set by value
	tpMatrix(T c0,T c4,T c8,T c12,
			 T c1,T c5,T c9,T c13,
			 T c2,T c6,T c10,T c14,
			 T c3,T c7,T c11,T c15);
	//! destructor
	~tpMatrix();
	
	//! set identity matrix
	void setIdentity();
	
	//! set null matrix
	void setNull();
	
	
	//! set the values
	void set(tpMatrix& orig);
	
	//! set values for specific row
	void setRow(tpUInt row,T c1, T c2, T c3, T c4);
	//! set values for specific column
	void setColumn(tpUInt column,T c1, T c2, T c3, T c4);
	
	
	//! set the translation by values
	void setTranslation(T v1, T v2, T v3);	
	//! set the translation by a vector
	void setTranslation(const tpVec3<T>& vec);
	
	//! translate in direction v1,v2,v3 by value
	void translate(const T& v1, const T& v2, const T& v3);
	//! translate in direction of a value
	void translate(const tpVec3<T>& vec);
	
	//! returns the translation vector
	tpVec3<T> getTranslation() const;
	
	//! set scale absolute by value
	void setScale(const T& v1, const T& v2, const T& v3);
	//! set scale absolute by three dimensional vector
	void setScale(const tpVec3<T>& vec);
	
	//! scale matrix by values
	void scale(const T& v1, const T& v2, const T& v3);
	//! scale matrix by vector
	void scale(const tpVec3<T>& vec);
	
	//! set rotation around vector with angle in degree
	void setRotation(const tpVec3<T>& vec,const T& rotation);
	
	//! set rotation around vector with angle in degree
	void setRotation(const T& c1,const T& c2,const T& c3,const T& angle);
	
	//! rotate matrix around vector with angle in degree
	void rotate(const tpVec3<T>& vec,const T& rotation);
	
	//! rotate matrix around vector with angle in degree
	void rotate(const T& c1,const T& c2,const T& c3,const T& angle);
	
	//! rotate matrix around vector with angle in degree
	void setEuler(const T& yaw,const T& pitch,const T& roll);
	
	
	//! multiply matrix
	tpMatrix<T> operator*(const tpMatrix<T>& rs) const;
	
	//! assign multiplied matrix
	const tpMatrix<T>& operator*=(const tpMatrix<T>& rs);
	
	//! assign matrix
	tpMatrix<T>& operator = (const tpMatrix& rs);
	
	
	//! invert the matrix
	void invert();
	
	//! set the matrix to the inverted matrix of the parameter
	void setInverse(const tpMatrix& mat);
	
	//! store the inverse of a matrix
	void inverse(const tpMatrix& m);
	
	//! transpose the matrix
	tpMatrix<T>& transpose();
	
	//! set the perspective matrix
	tpMatrix<T>& setPerspective(T FovY, T Aspect, T Near, T Far);
	
	tpMatrix<T>& setOrtho(T Left,T Right,T Bottom,T Top, T Near, T Far);
	
	//! set the frustum
	tpMatrix<T>& setFrustum(T Left,T Right,T Bottom,T Top, T Near, T Far);
	
	//! return the matrix determinant
	T getDeterminant() const;
	
	tpMatrix<T>& lookAt(const tpVec3<T>& eye, 
						const tpVec3<T>& target, 
						const tpVec3<T>& up);
	
	//! return euler angles 
	tpVec3<T> getEulerAngles() const;
	
	//! get a pointer to the raw data
	const T* getData() const;
	
	//! get a pointer to the raw data
	T* getData();
	
	//! set from raw data (float array)
	void setFromRaw(const tpFloat* raw);
	//! set from raw data (double array)
	void setFromRaw(const tpDouble* raw);
	//! set from raw data (integer array)
	void setFromRaw(const tpInt* raw);
	
	void dump(const tpUInt& level = TP_LOG_NOTIFY);
	
public:
	T mat[16];
private:
	
	static T m_identity[16];
	
};


template <class T> inline tpMatrix<T> tpGetTranslationMatrix(T x, T y, T z)
{
	tpMatrix<T> mat;
	mat.setTranslation(x,y,z);
	return mat;
};

template <class T> inline tpMatrix<T> tpGetRotationMatrix(T x, T y, T z, T angle)
{
	tpMatrix<T> mat;
	mat.setRotation(x,y,z,angle);
	return mat;
};


// ----------------------------------------------------------------

template <class T> inline tpMatrix<T>::tpMatrix()
{
	setIdentity();
}

template <class T> inline tpMatrix<T>::tpMatrix(const T* raw)
{
	setFromRaw(raw);
}

template <class T> inline tpMatrix<T>::tpMatrix(const tpMatrix& matrix)
{
	for (register tpUShort i = 0; i < 16; i++) mat[i] = (T)matrix.mat[i];
}

template <class T> inline const T* tpMatrix<T>::getData() const
{
	return mat;
}

template <class T> inline T* tpMatrix<T>::getData()
{
	return mat;
}

template <class T> inline tpMatrix<T>::tpMatrix(T c0,T c4,T c8,T c12,
												T c1,T c5,T c9,T c13,
												T c2,T c6,T c10,T c14,
												T c3,T c7,T c11,T c15
												)
{
    mat[0] = c0;
    mat[1] = c1;
    mat[2] = c2;
    mat[3] = c3;
	
    mat[4] = c4;
    mat[5] = c5;
    mat[6] = c6;
    mat[7] = c7;
	
    mat[8] = c8;
    mat[9] = c9;
    mat[10] = c10;
    mat[11] = c11;
	
    mat[12] = c12;
    mat[13] = c13;
    mat[14] = c14;
    mat[15] = c15;
}


template <class T> inline 
void tpMatrix<T>::setRow(tpUInt row,T c1,T c2,T c3,T c4)
{
	mat[0  + row] = c1;
	mat[4  + row] = c2;
	mat[8  + row] = c3;
	mat[12 + row] = c4;
};


template <class T> inline 
void tpMatrix<T>::setColumn(tpUInt column,T c1,T c2,T c3,T c4)
{
	mat[0 + column * 4] = c1;
	mat[1 + column * 4] = c2;
	mat[2 + column * 4] = c3;
	mat[3 + column * 4] = c4;
};

template <class T> inline 
tpMatrix<T>::~tpMatrix()
{
}

template <class T> T tpMatrix<T>::m_identity[16] =
{
	T(1), T(0), T(0), T(0),
	T(0), T(1), T(0), T(0),
	T(0), T(0), T(1), T(0),
	T(0), T(0), T(0), T(1)
};

template <class T> inline void tpMatrix<T>::setIdentity()
{
	for (register tpUShort i = 0; i < 16;++i) mat[i] = T(m_identity[i]);
}

template <class T> inline void tpMatrix<T>::setNull()
{
	for (register tpUShort i = 0; i < 16;++i) mat[i] = (T)0;
};

template <class T> inline void tpMatrix<T>::set(tpMatrix& orig)
{
	for (register tpUShort i = 0; i < 16; ++i) mat[i] = (T)orig.mat[i];
}


template <class T> inline 
void tpMatrix<T>::setTranslation(T v1, T v2, T v3)
{
	setIdentity();
	mat[12] = v1;
	mat[13] = v2;
	mat[14] = v3;
};


template <class T> inline 
void tpMatrix<T>::setTranslation(const tpVec3<T>& vec)
{
	setIdentity();
	mat[12] = vec.vec[0];
	mat[13] = vec.vec[1];
	mat[14] = vec.vec[2];
};


template <class T> inline
tpVec3<T> tpMatrix<T>::getTranslation() const
{
	tpVec3<T> out;
	out.set(mat[12],mat[13],mat[14]);
	
	return out;
}


template <class T> 
void tpMatrix<T>::translate(const T& v1,const T& v2,const T& v3)
{
	mat[12] += v1;
	mat[13] += v2;
	mat[14] += v3;
};

template <class T> inline 
void tpMatrix<T>::translate(const tpVec3<T>& vec)
{
	mat[12] += vec.vec[0];
	mat[13] += vec.vec[1];
	mat[14] += vec.vec[2];
};


template <class T> inline 
void tpMatrix<T>::setScale(const T& v1,const T& v2,const T& v3)
{
	mat[0] = v1;
	mat[5] = v2;
	mat[10] = v3;
};

template <class T> inline 
void tpMatrix<T>::setScale(const tpVec3<T>& vec)
{
	setIdentity();
	mat[0] = vec.vec[0];
	mat[5] = vec.vec[1];
	mat[10] = vec.vec[2];
};

template <class T> inline void tpMatrix<T>::scale(const T& v1, const T& v2, const T& v3)
{
	tpMatrix<T> _mat;
	_mat.setScale(v1,v2,v3);
	*this *= _mat;
};

template <class T> inline void tpMatrix<T>::scale(const tpVec3<T>& vec)
{
	tpMatrix<T> _mat;
	_mat.setScale(vec);
	*this *= _mat;
};

template <class T> inline void tpMatrix<T>::setRotation(const T& c1,
														const T& c2,
														const T& c3,
														const T& angle)
{
	setIdentity();
	tpVec3<T> _vec(c1,c2,c3);	
	setRotation(_vec,angle);
};


template <class T> inline void tpMatrix<T>::setRotation(const tpVec3<T>& vec,const T& rotation)
{
	
	setIdentity();
	
	if (vec.getLength() < .000001) return;
	
	T _radiant = tpDeg2Rad(rotation);
    
	T _fCos = (T) cos (_radiant);
	
	tpVec<T,3> _vCos = vec * (1 - _fCos);
	tpVec<T,3> _vSin = vec * (T)sin(_radiant);
	
	mat[0]= (T) ((vec.vec[0] * _vCos.vec[0]) + _fCos);
    mat[4]= (T) ((vec.vec[0] * _vCos.vec[1]) - _vSin.vec[2]);
    mat[8]= (T) ((vec.vec[0] * _vCos.vec[2]) + _vSin.vec[1]);
    
    mat[1]= (T) ((vec.vec[1] * _vCos.vec[0]) + _vSin.vec[2]);
    mat[5]= (T) ((vec.vec[1] * _vCos.vec[1]) + _fCos);
    mat[9]= (T) ((vec.vec[1] * _vCos.vec[2]) - _vSin.vec[0]);
	
    mat[2]= (T)  ((vec.vec[2] * _vCos.vec[0]) - _vSin.vec[1]);
    mat[6]= (T)  ((vec.vec[2] * _vCos.vec[1]) + _vSin.vec[0]);
    mat[10]= (T) ((vec.vec[2] * _vCos.vec[2]) + _fCos);
	
    mat[3] = mat[7] = mat[11] = 0;
	
    mat[15] = 1;
    
};


template <class T> inline void tpMatrix<T>::rotate(const tpVec3<T>& vec,const T& rotation)
{
	setRotation(vec.vec[0],vec.vec[1],vec.vec[2],rotation);
}

template <class T> inline void tpMatrix<T>::rotate(const T& c1,
												   const T& c2,
												   const T& c3,
												   const T& angle)
{
	tpMatrix<T> mat;
	mat.setRotation(c1,c2,c3,angle);
	*this *= mat;
}



template <class T> inline tpMatrix<T> tpMatrix<T>::operator*(const tpMatrix<T>& rs) const
{
	register int r;
	register int c;
	register int t = 4;
	
	tpMatrix<T> ret;
	
	for (register tpUByte k = 0; k < 16; k++ ) {
		r = k % t;
		c = (tpUInt)(k / t);
		ret.mat[k] = (T)0;
		for (int i = 0;i < t; i++)
			ret.mat[k] += mat[r+(i*t)] * rs.mat[(c*t)+i];
	}
	return ret;
};

template <class T> inline tpMatrix<T>& tpMatrix<T>::operator=(const tpMatrix& rs)
{
	for (register tpUInt i = 0; i < 16; i++) mat[i] = (T)rs.mat[i];
	return *this;
};


template <class T> const tpMatrix<T>& tpMatrix<T>::operator*=(const tpMatrix<T>& rs)
{
    *this = *this * rs;
	return *this;
};

template <class T> inline tpVec3<T> tpMatrix<T>::getEulerAngles() const
{
	tpVec3<T> vec;
	
	if (mat[1] != 0)
	{
		vec.vec[0] = (T)asin(mat[6]);
		vec.vec[1]  = (T)atan2(-mat[2], mat[10]);
		vec.vec[2] = (T)atan2(-mat[1], mat[5]);
	} else 
	{
		vec[0] = (T)asin(mat[6]);
		vec[1]  = 0;
		vec[2] = (T)atan2(-mat[1],mat[0]);
	}
	
	return vec;
}



template <class T> inline T tpMatrix<T>::getDeterminant() const
{
	return
	(mat[0]*mat[5]  - mat[4]  *mat[1])*(mat[10]*mat[15] - mat[14]*mat[11])
	-(mat[0]*mat[9]  - mat[8]  *mat[1])*(mat[6]*mat[15] - mat[14]*mat[7])
	+(mat[0]*mat[13] - mat[12] *mat[1])*(mat[6]*mat[11] - mat[10]*mat[7])
	+(mat[4]*mat[9]  - mat[8]  *mat[5])*(mat[2]*mat[15] - mat[14]*mat[3])
	-(mat[4]*mat[13] - mat[12] *mat[5])*(mat[2]*mat[11] - mat[10]*mat[3])
	+(mat[8]*mat[13] - mat[12] *mat[9])*(mat[2]*mat[7] - mat[6]*mat[3]);
}

template <class T> 
inline tpMatrix<T>& tpMatrix<T>::lookAt(const tpVec3<T>& eye, 
										const tpVec3<T>& target, 
										const tpVec3<T>& up)
{
	tpVec3<T> L = target;
	
	L -= eye;
	
	L.normalize();
	
	tpVec3<T> S = L.cross(up);
	S.normalize();
	
	tpVec3<T> Ud = S.cross(L);
	
	/*
	 mat[0] = S[0];
	 mat[1] = S[1];
	 mat[2] = S[2];
	 mat[3] = (T)0;
	 
	 mat[4] = Ud[0];
	 mat[5] = Ud[1];
	 mat[6] = Ud[2];
	 mat[7] = (T) 0;
	 
	 mat[8] = -L[0];
	 mat[9] = -L[1];
	 mat[10] = -L[2];
	 mat[11] = (T) 0;
	 */
	
	mat[0] = S[0];
	mat[4] = S[1];
	mat[8] = S[2];
	mat[3] = (T)0;
	
	mat[1] = Ud[0];
	mat[5] = Ud[1];
	mat[9] = Ud[2];
	mat[7] = (T) 0;
	
	mat[2] = -L[0];
	mat[6] = -L[1];
	mat[10] = -L[2];
	mat[14] = (T) 0;
	
	mat[12] = -eye.vec[0];
	mat[13] = -eye.vec[1];
	mat[14] = -eye.vec[2];
	
	mat[15] = 1;
	
	return *this;
};

/* based on a paper from Intel */
template <class T> inline tpVoid tpMatrix<T>::setInverse(const tpMatrix<T>& mtr) {
	
	
	T tmp[12]; /* temp array for pairs             */
	T src[16]; /* array of transpose source matrix */
	T det;     /* determinant*/
	
	/* transpose matrix */
	for (int i = 0; i < 4; i++) {
		src[i]         = mtr.mat[i*4];
		src[i + 4]     = mtr.mat[i*4 + 1];
		src[i + 8]     = mtr.mat[i*4 + 2];
		src[i + 12]    = mtr.mat[i*4 + 3];
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
	this->mat[0]         =   tmp[0]*src[5]           + tmp[3]*src[6] + tmp[4]*src[7];
	this->mat[0]       -=    tmp[1]*src[5]           + tmp[2]*src[6] + tmp[5]*src[7];
	this->mat[1]         =   tmp[1]*src[4]           + tmp[6]*src[6] + tmp[9]*src[7];
	this->mat[1]       -=    tmp[0]*src[4]           + tmp[7]*src[6] + tmp[8]*src[7];
	this->mat[2]         =   tmp[2]*src[4]           + tmp[7]*src[5] + tmp[10]*src[7];
	this->mat[2]       -=    tmp[3]*src[4]           + tmp[6]*src[5] + tmp[11]*src[7];
	this->mat[3]         =   tmp[5]*src[4]           + tmp[8]*src[5] + tmp[11]*src[6];
	this->mat[3]       -=    tmp[4]*src[4]           + tmp[9]*src[5] + tmp[10]*src[6];
	this->mat[4]          =  tmp[1]*src[1]           + tmp[2]*src[2] + tmp[5]*src[3];
	this->mat[4]       -=    tmp[0]*src[1]           + tmp[3]*src[2] + tmp[4]*src[3];
	this->mat[5]          =  tmp[0]*src[0]           + tmp[7]*src[2] + tmp[8]*src[3];
	this->mat[5]       -=    tmp[1]*src[0]           + tmp[6]*src[2] + tmp[9]*src[3];
	this->mat[6]          =  tmp[3]*src[0]           + tmp[6]*src[1] + tmp[11]*src[3];
	this->mat[6]       -=    tmp[2]*src[0]           + tmp[7]*src[1] + tmp[10]*src[3];
	this->mat[7]          =  tmp[4]*src[0]           + tmp[9]*src[1] + tmp[10]*src[2];
	this->mat[7]       -=    tmp[5]*src[0]           + tmp[8]*src[1] + tmp[11]*src[2];
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
	this->mat[8] =         tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	this->mat[8] -=        tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	this->mat[9] =         tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	this->mat[9] -=        tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	this->mat[10] =        tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	this->mat[10]-=        tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	this->mat[11] =        tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	this->mat[11]-=        tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	this->mat[12] =        tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	this->mat[12]-=        tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	this->mat[13] =        tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	this->mat[13]-=        tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	this->mat[14] =        tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	this->mat[14]-=        tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	this->mat[15] =        tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	this->mat[15]-=        tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];
	
	/* calculate determinant */
	det=src[0]*this->mat[0]+src[1]*this->mat[1]+src[2]*this->mat[2]+src[3]*this->mat[3];
	
	/* calculate matrix inverse */
	det = 1/det;
	for (int j = 0; j < 16; j++)
		this->mat[j] *= det;
}


template <class T> inline tpVoid tpMatrix<T>::invert()
{
	tpMatrix<T> tmp = *this;
	this->setInverse(tmp);
}


#if 0
template <class T> inline tpVoid tpMatrix<T>::invert()
{
	
	T det = this->getDeterminant();
	
	if ((T)0 == det) return;
	
	
	det = (T)1 / det;
	
	tpMatrix<T> res;
	
	res.mat[0] = det * (mat[5]*(mat[10]*mat[15] - mat[14]*mat[11]) + mat[9]*(mat[14]*mat[7] - mat[6]*mat[15]) + mat[13]*(mat[6]*mat[11] - mat[10]*mat[7]));
	res.mat[4] = det * (mat[6]*(mat[8]*mat[15] - mat[12]*mat[11]) + mat[10]*(mat[12]*mat[7] - mat[4]*mat[15]) + mat[14]*(mat[4]*mat[11] - mat[8]*mat[7]));
	res.mat[8] = det * (mat[7]*(mat[8]*mat[13] - mat[12]*mat[9]) + mat[11]*(mat[12]*mat[5] - mat[4]*mat[13]) + mat[15]*(mat[4]*mat[9] - mat[8]*mat[5]));
	res.mat[12] = det * (mat[4]*(mat[13]*mat[10] - mat[9]*mat[14]) + mat[8]*(mat[5]*mat[14] - mat[13]*mat[6]) + mat[12]*(mat[9]*mat[6] - mat[5]*mat[10]));
	res.mat[1] = det * (mat[9]*(mat[2]*mat[15] - mat[14]*mat[3]) + mat[13]*(mat[10]*mat[3] - mat[2]*mat[11]) + mat[1]*(mat[14]*mat[11] - mat[10]*mat[15]));
	res.mat[5] = det * (mat[10]*(mat[0]*mat[15] - mat[12]*mat[3]) + mat[14]*(mat[8]*mat[3] - mat[0]*mat[11]) + mat[2]*(mat[12]*mat[11] - mat[8]*mat[15]));
	res.mat[9] = det * (mat[11]*(mat[0]*mat[13] - mat[12]*mat[1]) + mat[15]*(mat[8]*mat[1] - mat[0]*mat[9]) + mat[3]*(mat[12]*mat[9] - mat[8]*mat[13]));
	res.mat[13] = det * (mat[8]*(mat[13]*mat[2] - mat[1]*mat[14]) + mat[12]*(mat[1]*mat[10] - mat[9]*mat[2]) + mat[0]*(mat[9]*mat[14] - mat[13]*mat[10]));
	res.mat[2] = det * (mat[13]*(mat[2]*mat[7] - mat[6]*mat[3]) + mat[1]*(mat[6]*mat[15] - mat[14]*mat[7]) + mat[5]*(mat[14]*mat[3] - mat[2]*mat[15]));
	res.mat[6] = det * (mat[14]*(mat[0]*mat[7] - mat[4]*mat[3]) + mat[2]*(mat[4]*mat[15] - mat[12]*mat[7]) + mat[6]*(mat[12]*mat[3] - mat[0]*mat[15]));
	res.mat[10] = det * (mat[15]*(mat[0]*mat[5] - mat[4]*mat[1]) + mat[3]*(mat[4]*mat[13] - mat[12]*mat[5]) + mat[7]*(mat[12]*mat[1] - mat[0]*mat[13]));
	res.mat[14] = det * (mat[12]*(mat[5]*mat[2] - mat[1]*mat[6]) + mat[0]*(mat[13]*mat[6] - mat[5]*mat[14]) + mat[4]*(mat[1]*mat[14] - mat[13]*mat[2]));
	res.mat[3] = det * (mat[1]*(mat[10]*mat[7] - mat[6]*mat[11]) + mat[5]*(mat[2]*mat[11] - mat[10]*mat[3]) + mat[9]*(mat[6]*mat[3] - mat[2]*mat[7]));
	res.mat[7] = det * (mat[2]*(mat[8]*mat[7] - mat[4]*mat[11]) + mat[6]*(mat[0]*mat[11] - mat[8]*mat[3]) + mat[10]*(mat[4]*mat[3] - mat[0]*mat[7]));
	res.mat[11] = det * (mat[3]*(mat[8]*mat[5] - mat[4]*mat[9]) + mat[7]*(mat[0]*mat[9] - mat[8]*mat[1]) + mat[11]*(mat[4]*mat[1] - mat[0]*mat[5]));
	res.mat[15] = det * (mat[0]*(mat[5]*mat[10] - mat[9]*mat[6]) + mat[4]*(mat[9]*mat[2] - mat[1]*mat[10]) + mat[8]*(mat[1]*mat[6] - mat[5]*mat[2]));
	
	*this = res;
};

#endif

template <class T> inline tpVoid
tpMatrix<T>::inverse(const tpMatrix& m)
{
	*this = m;
	this->invert();
}




template <class T> inline tpMatrix<T>& tpMatrix<T>::transpose()
{
	tpSwap(mat[1], mat[4]);
	tpSwap(mat[2], mat[8]);
	tpSwap(mat[3], mat[12]);
	tpSwap(mat[6], mat[9]);
	tpSwap(mat[7], mat[13]);
	tpSwap(mat[11],mat[14]);
	
	return *this;
};



template <class T> inline tpMatrix<T>& 
tpMatrix<T>::setFrustum(T Left,T Right,T Bottom,T Top, T Near, T Far)
{
	
	mat[ 0] = (T) 2	* Near/(Right-Left);
	mat[ 4] = (T) 0;
	mat[ 8] = (T) (Right+Left)/(Right-Left);
	mat[12] = (T) 0;
	
	mat[ 1] = (T) 0;
	mat[ 5] = (T) 2 * Near/(Top-Bottom);	
	mat[ 9] = (T) (Top+Bottom)/(Top-Bottom);
	mat[13] = (T) 0;
	
	mat[ 2] = (T) 0;
	mat[ 6] = (T) 0;
	mat[10] = (T) -(Far+Near)/(Far-Near);
	mat[14] = (T) -(2*Far*Near)/(Far-Near);
	
	mat[ 3] = (T) 0;
	mat[ 7] = (T) 0;	
	mat[11] = (T) -1;
	mat[15] = (T) 0;
	
	return *this;
};

template <class T> inline tpMatrix<T>& 
tpMatrix<T>::setOrtho(T Left,T Right,T Bottom,T Top, T Near, T Far)
{
	
	mat[ 0] = (T) 2	* (Right-Left);
	mat[ 4] = (T) 0;
	mat[ 8] = (T) 0;
	mat[12] = (T) -(Right + Left)/(Right - Left);
	
	mat[ 1] = (T) 0;
	mat[ 5] = (T) 2 / (Top-Bottom);	
	mat[ 9] = (T) 0;
	mat[13] = (T) -(Top + Bottom)/(Top - Bottom);
	
	mat[ 2] = (T) 0;
	mat[ 6] = (T) 0;
	mat[10] = (T) - 2 / (Far - Near);
	mat[14] = (T) -(Far + Near) / (Far - Near);
	
	mat[ 3] = (T) 0;
	mat[ 7] = (T) 0;	
	mat[11] = (T) 0;
	mat[15] = (T) 1;
	
	return *this;
};



template <class T> inline 
tpMatrix<T>& tpMatrix<T>::setPerspective(T FovY, T Aspect, T Near, T Far) {
	
	T xmin, xmax, ymin, ymax;
	ymax = Near * (T)tan( FovY * TP_PI / T(360) );
	ymin = -ymax;
	xmin = ymin * Aspect;
	xmax = ymax * Aspect;
	
	return setFrustum( xmin, xmax, ymin, ymax, Near, Far );
}

template <class T> inline void tpMatrix<T>::dump(const tpUInt& level /*= TP_LOG_NOTIFY*/)
{
	tpLog::get().printf(level,"%3.2f\t%3.2f\t%3.2f\t%3.2f",
						mat[0],mat[1],mat[2],mat[3]);
	
	tpLog::get().printf(level,"%3.2f\t%3.2f\t%3.2f\t%3.2f",
						mat[4],mat[5],mat[6],mat[7]);
	
	tpLog::get().printf(level,"%3.2f\t%3.2f\t%3.2f\t%3.2f",
						mat[8],mat[9],mat[10],mat[11]);
	
	tpLog::get().printf(level,"%3.2f\t%3.2f\t%3.2f\t%3.2f\n",
						mat[12],mat[13],mat[14],mat[15]);
	
}

template <class T> inline void tpMatrix<T>::setEuler(const T& yaw, 
													 const T& pitch,
													 const T& roll)
{
	
	T _rad_yaw = tpDeg2Rad(yaw);
	T _rad_pitch = tpDeg2Rad(pitch);
	T _rad_roll = tpDeg2Rad(roll);
	
	
	
	T cosYaw = T(cos(_rad_yaw));
	T sinYaw = T(sin(_rad_yaw));
	
	T cosPitch = T(cos(_rad_pitch));
	T sinPitch = T(sin(_rad_pitch));
	
	T cosRoll = T(cos(_rad_roll));
	T sinRoll = T(sin(_rad_roll));
	
	mat[0] = cosYaw * cosPitch;
	mat[4] = cosYaw * sinPitch * sinRoll - sinYaw * cosRoll;
	mat[8] = cosYaw * sinPitch * cosRoll + sinYaw * sinRoll;
	mat[12] = T(0);
	
	mat[1] = sinYaw * cosPitch;
	mat[5] = cosYaw * cosRoll + sinYaw * sinPitch * sinRoll;
	mat[9] = sinYaw * sinPitch * cosRoll - cosYaw * sinRoll;
	mat[13] = T(0);
	
	mat[2] = -sinPitch;
	mat[6] = cosPitch * sinRoll;
	mat[10] = cosPitch * cosRoll;
	mat[14] = T(0);
	
	mat[3] = T(0);
	mat[7] = T(0);
	mat[11] = T(0);
	mat[15] = T(1);
	
};

template <class T> void tpMatrix<T>::setFromRaw(const tpFloat* raw)
{
	for (tpUInt i = 0;i < 16;i++)
	{
		mat[i] = (T)*raw++;
	}
	
};

template <class T> void tpMatrix<T>::setFromRaw(const tpDouble* raw)
{
	for (register tpUChar i = 0;i < 16;i++)
	{
		mat[i] = (T)*raw;
		raw++;
	};
	
};


template <class T> void tpMatrix<T>::setFromRaw(const tpInt* raw)
{
	for (register tpUChar i = 0;i < 16;i++)
	{
		mat[i] = (T)*raw;
		raw++;
	};
	
};

template <tpUInt R, tpUInt C, typename T> class tpMat {
public:
	
	typedef T value_type;
	
	tpMat() {}
	
	tpMat(const tpMat<R,C,T>& mtc) { for (int i = 0; i < R*C; ++i) m[i] = mtc.data()[i]; }
	
	tpMat(const T* val) { for (int i = 0; i < R*C; ++i) m[i] = val[i]; }
	
	template <typename Tout>
	void copy(tpMat<R,C,T>& out) const { for (tpUInt i = 0; i < R*C; ++i) out[i] = m[i]; }
	
	const T* data() const { return m; }	
	
	T* data() { return m; }	
	
	tpUInt getRowCount() const { return R; }
	
	tpUInt getColumnCount() const { return C; }
	
	bool isSquare() const { return C == R; }
	
	void setValue(T val) { for (int i = 0; i < R*C; ++i) m[i] = val; }
	
	void setIdentity()
	{
		if (isSquare())
		{
			setValue(0);
			for (int i = 0; i < R; ++i)
			{
				m[i*C+i] = 1;
			}
		}
	}
	
	void setCellIdValue()
	{
		for (int i = 0; i < R*C;i++) {m[i] = i;}
	}
	
	void transpose()
	{
		tpMat<R,C,T> r;
		getTranspose(r);
		*this = r;
	}
	
	void getTranspose(tpMat<C,R,T>& rot) const
	{
		for (tpUInt r = 0; r < R; r++) 
		{
			for (tpUInt c = 0; c < C; c++ )
			{
				rot(c,r) = (*this)(r,c);
			}
		}
	}
	
	tpMat<R,C,T> multiply(const tpMat<R,C,T>& rhs) const
	{
		tpMat<R,C,T> res;
		for (tpUInt r = 0; r < R; ++r)
		{
			for (tpUInt c = 0; c < C; ++c)
			{
				T& v = res(r,c) = 0;
				for (tpUInt k = 0; k < C; ++k)
				{
					v += (*this)(r,k) * rhs(k,c);
				}
			}
		}
		return res;
	}
	
	void invert() 
	{
		tpMat<R,C,T> b;
		for ( tpUInt r = 0; r < C; ++r)
		{
			for ( int j = 0; j < R; ++j) 
			{
				short sgn = ( (r+j)%2) ? -1 : 1;
				tpMat<R-1,C-1,T> minor;
				this->getMinor(minor,r,j);
				b(r,j) = sgn * minor.getDeterminant();
			}
		}
		b.transpose();
		b /= getDeterminant();
		*this = b;
	}
	
	void getMinor(tpMat<R-1,C-1,T>& res, tpUInt r0, tpUInt c0) const;
	
	T getDeterminant() const;
	
	T& operator()(tpUInt r,tpUInt c);
	
	const T& operator()(tpUInt r,tpUInt c) const;
	
	void _print();
	
	tpMat<R,C,T>& operator = (const tpMat<R,C,T>& rhs) { for (tpUInt i = 0; i < R*C; i++) {m[i] = rhs.m[i];} return *this; }
	
	tpMat<R,C,T>& operator / (const T& rhs) { for (tpUInt i = 0; i < R*C; i++) {m[i] /= rhs;} return *this; }
	
 	void operator /= (const T& rhs) { for (tpUInt i = 0; i < R*C; i++) {m[i] /= rhs;} }
	
	T m[R*C];
	
};

//template<> inline
//tpStringBase<tpChar>& tpStringBase<tpChar>::format(const tpChar* format,...)
//{
//}


template <tpUInt R, tpUInt C,typename T>
void tpMat<R,C,T>::_print()
{
	printf("tpMat %dx%d\n",R,C);
	for (int r=0;r < R;r++)
	{
		for (int c=0;c < C;c++)
		{
			printf("%3.5f ",(*this)(r,c));
		}
		
		printf("\n");
	}
	//	printf("determinant: %3.f\n",getDeterminant());
	printf("\n");
}




template <tpUInt R, tpUInt C,typename T>
T& tpMat<R,C,T>::operator()(tpUInt r,tpUInt c) { return m[r * C + c]; }

template <tpUInt R, tpUInt C,typename T>
const T& tpMat<R,C,T>::operator()(tpUInt r,tpUInt c) const { return m[r * C + c]; }

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
			res.m[r*(C-1)+c] = this->m[ri*C + ci];//(*this)(ri,ci);
			c++;
		}
		r++;
	}
}

template <tpUInt R, tpUInt C,typename T>
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



// partial specializations

//
// Mat 4x4
//
template <typename T> class tpMat44 : public tpMat<4,4,T>
{
public:
	
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
	
	void invert()
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
		det = 1/det;
		for (int j = 0; j < 16; j++)
			this->m[j] *= det;
	}
};

template <> class tpMat11x : public tpMat<1,1,tpFixed32>
{
public:

	tpFixed32 getDeterminant() const
	{
		return this->m[0];
	}
};

template <> class tpMat11f : public tpMatX<1,1,float>
{
public:

	float getDeterminant() const
	{
		return this->m[0];
	}
};




#endif
