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

#include "tp/material.h"
#include "tp/log.h"

tpMaterial::tpMaterial(const tpString& name)
	: tpObject(name)
	, m_ambientcolor(tpVec4r(0.8,0.8,0.8,1.0))
	, m_diffusecolor(tpVec4r(1.0,1.0,1.0,1.0))
	, m_emissivecolor(tpVec4r(0.0,0.0,0.0,1.0))
	, m_specularcolor(tpVec4r(1.0,1.0,1.0,1.0))
	, m_shininess(0.f)
{
}


tpMaterial::tpMaterial(const tpMaterial& node)
	: tpObject(node)
{
	*this = node;
}


tpObject* tpMaterial::clone(const tpUByte& m) const
{
	return new tpMaterial(*this);
}

tpMaterial& tpMaterial::operator = (const tpMaterial& mat)
{
	this->m_ambientcolor = mat.m_ambientcolor;
	this->m_diffusecolor = mat.m_diffusecolor;
	this->m_emissivecolor = mat.m_emissivecolor;
	this->m_specularcolor = mat.m_specularcolor;

	this->m_transparency = mat.m_transparency;
	this->m_shininess = mat.m_shininess;

	return *this;
}

tpMaterial *tpMaterial::createDefault(tpReal r, tpReal g, tpReal b, tpReal intensity)
{
	tpMaterial* m = new tpMaterial();

	m->setDiffuseColor(tpVec4f(r,g,b,1));
	m->setSpecularColor(tpVec4f(intensity,intensity,intensity,1));

	m->setAmbientColor(tpVec4f(0,0,0,1));
	m->setEmissiveColor(tpVec4f(0,0,0,1));
	m->setShininess(1);

	return m;
}


TP_TYPE_REGISTER(tpMaterial,tpObject,Material);

//static tpMaterial gs_defaultmaterial;
//tpMaterial* tpDefaultMaterial = &gs_defaultmaterial;



