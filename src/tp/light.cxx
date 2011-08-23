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

