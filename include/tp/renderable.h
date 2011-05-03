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

#ifndef TPGEOMETRY_H
#define TPGEOMETRY_H

#include <tp/node.h>
#include <tp/material.h>
#include <tp/texture.h>

class TP_API tpRenderable : public tpNode
{
public:

	TP_TYPE_DECLARE;

	//! c'tor
	tpRenderable(const tpString& name = "");
	//! copy c'tor
	tpRenderable(const tpRenderable&);

	//! cloning interface
	virtual tpObject* clone();

	//! set a material for the regarding geometry
	void setMaterial(tpMaterial* material) { m_material = material; }
	//! get a pointer to the material
	const tpMaterial* getMaterial() const { return m_material.get(); }


	//! set a texture for the regarding geometry
	void setTexture(tpTexture* texture) { m_texture = texture; }
	//! get a pointer to the texture
	const tpTexture* getTexture() const { return m_texture.get(); }

	//! check if this material has a texture
	bool hasTexture() { return m_texture.isValid(); }

	//! set if this material is renderable is for culling only
	void setCulling(bool flag) { m_culling = flag; }
	bool getCulling() const { return m_culling; }


protected:

	tpRefPtr<tpMaterial>	m_material;
	tpRefPtr<tpTexture>		m_texture;

	bool		m_culling;

};


#endif
