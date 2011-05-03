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

#include <tp/light.h>
#include <tp/log.h>


tpArray<tpUInt> tpLight::ms_lightids = tpArray<tpUInt>();

tpLight::tpLight()
	: tpObject(),
	m_ambient(tpVec4f(0.0f,0.0f,0.0f,1.0f)),
	m_diffuse(tpVec4f(0.0f,0.0f,0.0f,1.0f)),
	m_specular(tpVec4f(0.0f,0.0f,0.0f,1.0f)),

	m_position(tpVec3f(0.0f,0.0f,1.0f)),
	m_direction(tpVec3f(0.0f,0.0f,-1.0f))

{
	tpInt _id = ms_lightids.getSize();
	while (ms_lightids.find(_id) ) {
		++_id;
	}
	ms_lightids.add(_id);
	m_id = _id;
}

tpLight::~tpLight()
{
	ms_lightids.erase(m_id);
}

TP_TYPE_REGISTER(tpLight,tpObject,Light);

