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
#include "tp/renderable.h"

tpRenderable::tpRenderable(const tpString& name)
	: tpNode(name),
	m_material(tpDefaultMaterial)
{
	//m_fields.add(new tpSingleField<tpMaterial>(m_material.get(),"material"));
	//m_fields.add(new tpSingleField<tpTexture>(m_texture,"texture"));

	//m_material.setValue(NULL);
	//m_texture.setValue(NULL);
	//m_culling.setValue(FALSE);
}


tpRenderable::tpRenderable(const tpRenderable& geo)
	: tpNode(geo),
	m_material(geo.m_material),
	m_texture(geo.m_texture)
{
}

tpObject* tpRenderable::clone()
{
	return new tpRenderable(*this);
};


tpMaterial* tpRenderable::getMaterial()
{
	return m_material.get();
}


void tpRenderable::setMaterial(tpMaterial* material)
{
	m_material = material;
}


void tpRenderable::setTexture(tpTexture* texture)
{
	m_texture = texture;
}

tpTexture* tpRenderable::getTexture()
{
	return m_texture.get();
}

void tpRenderable::setCulling(bool flag)
{
	m_culling = flag;
}

bool tpRenderable::getCulling() const
{
	return m_culling;
}


TP_TYPE_REGISTER(tpRenderable,tpNode,Geometry);
