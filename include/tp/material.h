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

#ifndef TP_MATERIAL_H
#define TP_MATERIAL_H

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
	tpMaterial(const tpMaterial& mat);

	//! cloning interface
	virtual tpObject* clone();

	//!
	tpMaterial& operator = (const tpMaterial&);

	void setShininess(const tpFloat& v) { m_shininess = v; }
	const tpFloat& getShininess() const {return m_shininess; }

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

extern TP_API tpMaterial* tpDefaultMaterial;

#endif
