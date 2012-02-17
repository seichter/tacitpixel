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

#ifndef TP_QUAT_H
#define TP_QUAT_H

#include <tp/vec.h>
#include <tp/mat.h>


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


     void set(const tpMat<4,4,T>& m)
	 {
		 this->w() = ::sqrt(T(1) + m(0,0) + m(1,1) + m(2,2)) / T(2.0);
		 
		 T w4 = T(4.0) * this->w();
		 this->x() = (m(2,1) - m(1,2)) / w4 ;
		 this->y() = (m(0,2) - m(2,0)) / w4 ;
		 this->z() = (m(1,0) - m(0,1)) / w4 ;
	 }

     void get(tpMat<4,4,T>& m) const
     {
     }

};

#endif
