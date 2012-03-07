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


/**
  * @brief just a stub for typical OpenGL matrix operations
  */
struct TP_API tpMat44Op {

	/**
	  * @brief OpenGL based lookAt implementation
	  * @param eye position of the eye (or camera)
	  * @param target where the camera looks at
	  * @param up vector describing the upright direction
	  * @param matOut output of the calculation
	  */
	template <typename T> inline
	static void lookAt(const tpVec3<T>& eye, const tpVec3<T>& target, const tpVec3<T>& up,tpMat<4,4,T>& matOut)
	{
		tpVec3<T> L(target); L-= eye;
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

	/**
	  * @brief OpenGL based frustum implementation
	  * @param Left left border of the frustum (in pixel)
	  * @param Right right border of the frustum (in pixel)
	  * @param Bottom bottom border of the frustum (in pixel)
	  * @param Top topborder of the frustum (in pixel)
	  * @param Near near clipping plane
	  * @param Far far clipping plane
	  * @param matOut output of the calculation
	  */
	template <typename T>
	static void frustum(T Left,T Right,T Bottom,T Top,T zNear,T zFar,tpMat<4,4,T>& matOut)
	{
        matOut.fill(0);

		matOut(0,0) = 2 * zNear/(Right-Left);
		matOut(1,1) = 2 * zNear/(Top-Bottom);

		matOut(0,2) =     (Right+Left)/(Right-Left);	//A
		matOut(1,2) =     (Top+Bottom)/(Top-Bottom);	//B
		matOut(2,2) = -   (zFar+zNear)/(zFar-zNear);		//C
		matOut(3,2) = -(2 * zFar*zNear)/(zFar-zNear);     //D

		matOut(2,3) = -1;
	}


	/**
	  * @brief OpenGL based perspective projection matrix implementation
	  * @param FovY of the camera (in degree)
	  * @param Aspect aspect ratio (normalized) width by height
	  * @param Near near clipping plane
	  * @param Far far clipping plane
	  * @param matOut output of the calculation
	  */
	template <typename T>
	static void perspective(T fovY, T aspectRatio, T zNear, T zFar, tpMat<4,4,T>& matOut)
	{
		T height = zNear * tan(fovY/T(360)*TP_PI);          // half height of near plane
		T width = height * aspectRatio;      // half width of near plane

		tpMat44Op::frustum( -width, width, -height, height, zNear, zFar, matOut );
	}

	/**
	  * @brief OpenGL based 2D orthographic matrix implementation
	  * @param Left left border of the frustum (in pixel)
	  * @param Right right border of the frustum (in pixel)
	  * @param Bottom bottom border of the frustum (in pixel)
	  * @param Top topborder of the frustum (in pixel)
	  * @param Near near clipping plane
	  * @param Far far clipping plane
	  * @param matOut output of the calculation
	  */
	template <typename T>
	static void ortho(T Left, T Right, T Bottom, T Top, T Near, T Far, tpMat<4,4,T>& matOut)
	{
		matOut.fill(0);
		matOut(0,0) =  2 / (Right-Left);
		matOut(1,1) =  2 / (Top-Bottom);
		matOut(2,2) = -2 / (Far-Near);

		matOut(0,3) = (Right+Left)/(Right-Left);
		matOut(1,3) = (Top+Bottom)/(Top-Bottom);
		matOut(2,3) = (Far+Near)/(Far-Near);

		matOut(3,3) = 1;
	}

	template <typename T>
	static tpMat44<T> translation(T x, T y, T z)
	{
		tpMat44<T> out; out.setTranslation(x,y,z); return out;
	}

};


#endif
