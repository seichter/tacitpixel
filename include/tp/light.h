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

#ifndef TPLIGHT_H
#define TPLIGHT_H

#include <tp/array.h>
#include <tp/object.h>
#include <tp/vec.h>


class TP_API tpLight : public tpObject {
public:

	TP_TYPE_DECLARE

	tpLight();

	tpUInt getID() const { return m_id; }

	void setAmbientColor(const tpVec4f& color) { m_ambient = color; }
	void setDiffuseColor(const tpVec4f& color) { m_diffuse = color; }
	void setSpecularColor(const tpVec4f& color) { m_specular = color; }
	
	const tpVec4f& getAmbientColor() const { return m_ambient; }
	const tpVec4f& getDiffuseColor() const { return m_diffuse; }
	const tpVec4f& getSpecularColor() const { return m_specular; }

	void setPosition(const tpVec3f& position) { m_position = position; }
	const tpVec3f& getPosition() const { return m_position; }

protected:

	virtual ~tpLight();
	
	/* colors */	
	tpVec4f m_ambient;
	tpVec4f m_diffuse;
	tpVec4f m_specular;
	
	/* position & orientation */
	tpVec3f m_position;
	tpVec3f m_direction;
	
	/* light model */
	tpInt m_exponent;
	tpInt m_cutoff;
	tpInt m_constant_attenuation;
	tpInt m_linear_attenuation;
	tpInt m_quadratic_attenuation;
	
	/* ID management */
	tpUInt m_id;
	static tpArray<tpUInt> ms_lightids;

};

#endif
