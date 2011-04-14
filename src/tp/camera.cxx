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
#include "tp/camera.h"


tpCamera::tpCamera(tpUByte type /* = TP_CAMERA_PERSPECTIVE */)
	: tpReferenced(),
	m_type(type),
	m_nearplane(1.0),
	m_farplane(100.0),
	m_fov(45.0),
	m_clearcolor(tpVec4f(.3f,.4f,.7f,1.f))
{
	m_transform.setIdentity();
	m_projection.setIdentity();

	apply();

}


void tpCamera::apply()
{
	if (m_type == kPerspective) 
	{
		m_projection.setPerspective(m_fov,m_aspectratio,m_nearplane,m_farplane);
	} else {
		m_projection.setOrtho(-1,1,-1,1,m_nearplane,m_farplane);
	}

	applyInverse();
}



tpCamera::~tpCamera()
{
};


void tpCamera::lookAt(const tpVec3r& pos,
		const tpVec3r& target,
		const tpVec3r& up /*= tpVec3d(0,1.0,0)*/)
{
	m_transform.lookAt(pos,target,up);
	applyInverse();
};



void tpCamera::setPosition(const tpVec3r& pos)
{
	setPosition(pos[0],pos[1],pos[2]);
};


void tpCamera::setPosition(tpReal x,
						   tpReal y,
						   tpReal z)
{
	m_transform.setTranslation(x,y,z);
	applyInverse();
};


void tpCamera::translate(tpReal x, 
						 tpReal y, 
						 tpReal z)
{
	m_transform.translate(x,y,z);
	applyInverse();
}


tpVec3r tpCamera::getPosition() const
{
	return m_transform.getTranslation();
}

void tpCamera::applyInverse() 
{
	m_transform = m_transform_inverse.getInverse();
}

void tpCamera::setTransformationMatrix(const tpMat44r& trans, bool isInverted ) 
{
	if (isInverted) 
	{
		m_transform = trans.getInverse();

	} else {
		m_transform = trans;
	}
}


void tpCamera::setRotation(tpReal x, tpReal y, tpReal z, tpReal angle)
{
	m_transform.setRotation(tpVec3r(x,y,z),angle);
	applyInverse();
}


void tpCamera::rotate(tpReal x, tpReal y, tpReal z,tpReal angle)
{
	m_transform.rotate(tpVec3r(x,y,z),angle);
	applyInverse();
}

void tpCamera::setTransform(const tpMat44r& transform)
{
	m_transform = transform;
	applyInverse();
}


void tpCamera::setFarClipping(const tpReal& plane)
{
	m_farplane = plane;
	apply();
}

void tpCamera::setNearClipping(const tpReal& plane)
{
	m_nearplane = plane;
	apply();
}

void 
tpCamera::setFOV(const tpReal& fov)
{
	m_fov = fov;
	apply();
}

const tpReal& 
tpCamera::getFarClipping() const
{
	return m_farplane;
}


const tpReal& 
tpCamera::getNearClipping() const
{
	return m_nearplane;
}

const tpReal& 
tpCamera::getFOV() const
{
	return m_fov;
};


const tpMat44r& 
tpCamera::getProjection() const
{
	return m_projection;
};

const tpMat44r& tpCamera::getTransform() const
{
	return m_transform;
}

const tpMat44r& tpCamera::getInverseTransform() const
{
	return m_transform_inverse;
}

TP_TYPE_REGISTER(tpCamera,tpReferenced,Camera);


