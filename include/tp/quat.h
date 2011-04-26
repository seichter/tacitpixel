/*
 * This file is part of tacit pixel
 *
 * (c) Copyrights 1999-2011 Hartmut Seichter
 *
 */
#ifndef TP_QUAT_H
#define TP_QUAT_H

#include <tp/vec.h>
#include <tp/matrix.h>


template <typename T> class tpQuat : tpVec4<T>
{
public:

	const T& w() const { return this->vec[0]; }
	const T& x() const { return this->vec[1]; }
	const T& y() const { return this->vec[2]; }
	const T& z() const { return this->vec[3]; }

	 T& w()  { return this->vec[0]; }
	 T& x()  { return this->vec[1]; }
	 T& y()  { return this->vec[2]; }
	 T& z()  { return this->vec[3]; }


	 void set(const tpMat<T>& m)
	 {
		 this->w() = ::sqrt(T(1) + m(0,0) + m(1,1) + m(2,2)) / T(2.0);
		 
		 T w4 = T(4.0) * this->w();
		 this->x() = (m(2,1) - m(1,2)) / w4 ;
		 this->y() = (m(0,2) - m(2,0)) / w4 ;
		 this->z() = (m(1,0) - m(0,1)) / w4 ;
	 }




};

#endif
