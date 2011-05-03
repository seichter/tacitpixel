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
