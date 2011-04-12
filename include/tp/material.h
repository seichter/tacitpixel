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

#ifndef TPMATERIAL_H
#define TPMATERIAL_H

#include <tp/object.h>
#include <tp/vec.h>


/*!
	\class tpMaterial
	\brief simple material assignment for tpGeometry

*/

class TP_API tpMaterial : public tpObject {
public:


	TP_TYPE_DECLARE

	//! standard c'tor
	tpMaterial(const tpString& name = "");

	//! copy c'tor
	tpMaterial(const tpMaterial& node);

	//! cloning interface
	virtual tpObject* clone();
	
	//! 
	tpMaterial& operator = (const tpMaterial&);

	void setShininess(const tpFloat&);
	tpFloat getShininess() const;

	void setTransparency(const tpFloat& v) { m_transparency = v; } 
	const tpFloat& getTransparency() const { return m_transparency; }

	void setDiffuseColor(const tpVec4f& v) { m_diffusecolor = v; }
	const tpVec4f& getDiffuseColor() const { return m_diffusecolor; }

	void setEmissiveColor(const tpVec4f& v) { m_emissivecolor = v; }
	const tpVec4f& getEmissiveColor() const { return m_emissivecolor; }

	void setSpecularColor(const tpVec4f& v) { m_specularcolor = v; }
	const tpVec4f& getSpecularColor() const { return m_specularcolor; }

	void setAmbientColor(const tpVec4f& v) { m_ambientcolor = v; }
	const tpVec4f& getAmbientColor() const { return m_ambientcolor; }

protected:
	
	tpVec4r m_ambientcolor;
	tpVec4r m_diffusecolor;
	tpVec4r m_emissivecolor;
	tpVec4r m_specularcolor;

	tpFloat m_shininess;
	tpFloat m_transparency;
};

extern tpMaterial* tpDefaultMaterial;

#endif
