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
#include "tp/material.h"
#include "tp/log.h"

tpMaterial::tpMaterial(const tpString& name)
	: tpObject(name)
{
	//m_ambientcolor.setValue(tpVec4f(0.2f,0.2f,0.2f,1.0f));
	//m_diffusecolor.setValue(tpVec4f(0.8f,0.8f,0.8f,1.0f));
	//m_specularcolor.setValue(tpVec4f(0.0f,0.0f,0.0f,1.0f));
	//m_emissivecolor.setValue(tpVec4f(0.0f,0.0f,0.0f,1.0f));
	//
	//m_shininess.setValue(0.0f);
	//m_transparency.setValue(0.0f);
}


tpMaterial::tpMaterial(const tpMaterial& node)
	: tpObject(node)
{
	*this = node;
}


tpObject* tpMaterial::clone()
{
	return new tpMaterial(*this);
};

tpMaterial& tpMaterial::operator = (const tpMaterial& mat)
{
	this->m_ambientcolor = mat.m_ambientcolor;
	this->m_diffusecolor = mat.m_diffusecolor;
	this->m_emissivecolor = mat.m_emissivecolor;	
	this->m_specularcolor = mat.m_specularcolor;

	this->m_transparency = mat.m_transparency;
	this->m_shininess = mat.m_shininess;

	return *this;
};




TP_TYPE_REGISTER(tpMaterial,tpObject,Material);

static tpMaterial gs_defaultmaterial;
tpMaterial* tpDefaultMaterial = &gs_defaultmaterial;



