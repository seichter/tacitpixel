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

#ifndef TPCAMERA_H
#define TPCAMERA_H 1


#include <tp/referenced.h>
#include <tp/thread.h>
#include <tp/mat.h>
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
		m_projection.setOrtho(l,r,b,t,n,f); 
	}

	void setProjectionFrustum(const tpReal& l, const tpReal& r, const tpReal& b, const tpReal& t, const tpReal& n, const tpReal& f ) { 
		m_projection.setFrustum(l,r,b,t,n,f);
	}

	void setProjectionPerspective(const tpReal& fov, const tpReal& aspect, const tpReal& n, const tpReal& f ) { 
		m_projection.setPerspective(fov,aspect,n,f);
	}

	const tpMat44r& getView() const { return m_view; }
	void setView(const tpMat44r& val) { m_view = val; }

	tpMat44r getInverseView() const { return m_view.getInverse(); }

	void setViewLookAt(const tpVec3r& eye, const tpVec3r& target, const tpVec3r& up);

protected:

	tpUShort m_clearflags;
	tpVec4f m_clearcolor;
	tpVec4i m_viewport;

	tpMat44r	m_view;
	tpMat44r	m_projection;
};



#endif
