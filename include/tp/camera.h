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
#ifndef TP_CAMERA_H
#define TP_CAMERA_H 1


#include <tp/object.h>
#include <tp/mat.h>
#include <tp/matop.h>
#include <tp/vec.h>

/*! \brief camera in a scene

	To render a scene you can either use your own
	matrix before rendering the scene. Or you
	pass a pointer to a camera (tpCamera) to the
	respective renderengine (tpRenderEngine)
 */
class TP_API tpCamera : public tpObject {
public:

	TP_TYPE_DECLARE

	enum {
		kClearColor		= (1 << 0),
		kClearDepth		= (1 << 1),
		kClearAccum		= (1 << 2),
		kClearStencil	= (1 << 3)
	};

	tpCamera();
	tpCamera(const tpCamera &cam);
	tpCamera &operator =(const tpCamera &cam);

	const tpVec4i& getViewport() const { return mViewport; }
	void setViewport(const tpVec4i& val) { mViewport = val; }

	const tpVec4f& getClearColor() const { return mClearColor; }
	void setClearColor(const tpVec4f& val) { mClearColor = val; }

	const tpUShort& getClearFlags() const { return mClearFlags; }
	void setClearFlags(const tpUShort& val) { mClearFlags = val; }
	bool hasClearFlag(const tpUShort& val) const { return (0 != (mClearFlags & val)); }

	const tpMat44r& getProjection() const { return mProjection; }
	void setProjection(const tpMat44r& val) { mProjection = val; }


	void setProjectionOrtho(const tpReal& l, const tpReal& r, const tpReal& b, const tpReal& t, const tpReal& n, const tpReal& f );

	void setProjectionFrustum(const tpReal& l, const tpReal& r, const tpReal& b, const tpReal& t, const tpReal& n, const tpReal& f );

	void setProjectionPerspective(const tpReal& fov, const tpReal& aspect, const tpReal& n, const tpReal& f );

	const tpMat44r& getView() const { return mView; }
	void setView(const tpMat44r& val) { mView = val; update(); }

	const tpMat44r& getViewInverse() const { return mViewInverse; }

	void setViewLookAt(const tpVec3r& eye, const tpVec3r& target, const tpVec3r& up);


protected:

	virtual ~tpCamera();

	void update();

	tpUShort mClearFlags;
	tpVec4f mClearColor;
	tpVec4i mViewport;

	tpMat44r	mView;
	tpMat44r	mViewInverse;
	tpMat44r	mProjection;
};



#endif
