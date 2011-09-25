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

#ifndef TP_MATOP_H
#define TP_MATOP_H

#include <tp/mat.h>

struct tpMatOp {


	template <typename T>
	static void preMultiplyTranslation(const tpVec<T,3>& vec,tpMat<4,4,T>& m)
	{
		for (unsigned i = 0; i < 3; ++i)
		{
			T tmp = vec[i];
			if (tmp == 0) continue;
			m(0,3) += tmp*m(0,i);
			m(1,3) += tmp*m(1,i);
			m(2,3) += tmp*m(2,i);
			m(3,3) += tmp*m(3,i);
		}
	}

	template <typename T>
	static void lookAt(const tpVec3<T>& eye, const tpVec3<T>& target, const tpVec3<T>& up,tpMat<4,4,T>& matOut)
	{
		tpVec3<T> L(target - eye);
		L.normalize();
		tpVec3<T> S = L.cross(up);
		S.normalize();
		tpVec3<T> Ud = S.cross(L);
		Ud.normalize();

		matOut(0,0) = S[0];
		matOut(1,0) = S[1];
		matOut(2,0) = S[2];
		matOut(3,0) = T(0);

		matOut(0,1) = Ud[0];
		matOut(1,1) = Ud[1];
		matOut(2,1) = Ud[2];
		matOut(3,1) = T(0);

		matOut(0,2) = -L[0];
		matOut(1,2) = -L[1];
		matOut(2,2) = -L[2];
		matOut(3,2) = T(0);

		matOut(0,3) = eye[0];
		matOut(1,3) = eye[1];
		matOut(2,3) = eye[2];
		matOut(3,3) = 1;

	}


	template <typename T>
	static void frustum(T Left,T Right,T Bottom,T Top, T Near, T Far,tpMat44<T>& matOut)
	{
		// set column-wise
		matOut.at( 0) = (T) 2	* Near/(Right-Left);
		matOut.at( 4) = (T) 0;
		matOut.at( 8) = (T) (Right+Left)/(Right-Left);
		matOut.at(12) = (T) 0;

		matOut.at( 1) = (T) 0;
		matOut.at( 5) = (T) 2 * Near/(Top-Bottom);
		matOut.at( 9) = (T) (Top+Bottom)/(Top-Bottom);
		matOut.at(13) = (T) 0;

		matOut.at( 2) = (T) 0;
		matOut.at( 6) = (T) 0;
		matOut.at(10) = (T) -(Far+Near)/(Far-Near);
		matOut.at(14) = (T) -	2*Far*Near	/ Far-Near;

		matOut.at( 3) = (T) 0;
		matOut.at( 7) = (T) 0;
		matOut.at(11) = (T) -1;
		matOut.at(15) = (T) 0;
	}


	template <typename T>
	static void perspective(T FovY, T Aspect, T Near, T Far, tpMat44<T>& matOut)
	{

		T xmin, xmax, ymin, ymax;
		ymax = Near * (T)tan( FovY * TP_PI / T(360) );
		ymin = -ymax;
		xmin = ymin * Aspect;
		xmax = ymax * Aspect;

		tpMatOp::frustum( xmin, xmax, ymin, ymax, Near, Far, matOut );
	}



	template <typename T>
	static void ortho(T Left,T Right,T Bottom,T Top, T Near, T Far, tpMat44<T>& matOut)
	{

		matOut.at( 0) = (T) 2	* (Right-Left);
		matOut.at( 4) = (T) 0;
		matOut.at( 8) = (T) 0;
		matOut.at(12) = (T) -(Right + Left)/(Right - Left);

		matOut.at( 1) = (T) 0;
		matOut.at( 5) = (T) 2 / (Top-Bottom);
		matOut.at( 9) = (T) 0;
		matOut.at(13) = (T) -(Top + Bottom)/(Top - Bottom);

		matOut.at( 2) = (T) 0;
		matOut.at( 6) = (T) 0;
		matOut.at(10) = (T) - 2 / (Far - Near);
		matOut.at(14) = (T) -(Far + Near) / (Far - Near);

		matOut.at( 3) = (T) 0;
		matOut.at( 7) = (T) 0;
		matOut.at(11) = (T) 0;
		matOut.at(15) = (T) 1;
	}

};


#endif
