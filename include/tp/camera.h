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


#include <tp/referenced.h>
#include <tp/mat.h>
#include <tp/matop.h>
#include <tp/vec.h>

/*! \brief camera in a scene

	To render a scene you can either use your own
	matrix before rendering the scene. Or you
	pass a pointer to a camera (tpCamera) to the
	respective renderengine (tpRenderEngine)
 */
class TP_API tpCamera : public tpReferenced {
public:

	TP_TYPE_DECLARE;

	enum {
		kClearColor = (1 << 0),
		kClearDepth = (1 << 1),
		kClearAccum = (1 << 2),
		kClearStencil = (1 << 3)
	};

	tpCamera();

	const tpVec4i& getViewport() const { return m_viewport; }
	void setViewport(const tpVec4i& val) { m_viewport = val; }

	const tpVec4f& getClearColor() const { return m_clearcolor; }
	void setClearColor(const tpVec4f& val) { m_clearcolor = val; }

	const tpUShort& getClearFlags() const { return m_clearflags; }
	void setClearFlags(const tpUShort& val) { m_clearflags = val; }
	bool hasClearFlag(const tpUShort& val) const { return (0 != (m_clearflags & val)); }

	const tpMat44r& getProjection() const { return m_projection; }
	void setProjection(const tpMat44r& val) { m_projection = val; }


	void setProjectionOrtho(const tpReal& l, const tpReal& r, const tpReal& b, const tpReal& t, const tpReal& n, const tpReal& f ) {
		tpMatOp::ortho(l,r,b,t,n,f,m_projection);
	}

	void setProjectionFrustum(const tpReal& l, const tpReal& r, const tpReal& b, const tpReal& t, const tpReal& n, const tpReal& f );

	void setProjectionPerspective(const tpReal& fov, const tpReal& aspect, const tpReal& n, const tpReal& f );

	const tpMat44r& getView() const { return m_view; }
	void setView(const tpMat44r& val) { m_view = val; }

	//tpMat44r getInverseView() const { return m_view.getInverse(); }

	void setViewLookAt(const tpVec3r& eye, const tpVec3r& target, const tpVec3r& up);

protected:

	tpUShort m_clearflags;
	tpVec4f m_clearcolor;
	tpVec4i m_viewport;

	tpMat44r	m_view;
	tpMat44r	m_projection;
};



#endif
