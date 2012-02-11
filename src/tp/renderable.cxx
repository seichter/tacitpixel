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

#include "tp/renderable.h"

tpRenderable::tpRenderable(const tpString& name)
	: tpNode(name)
    , mCulling(false)
    , mLighting(true)
//	, m_material(tpDefaultMaterial)
{
	//m_fields.add(new tpSingleField<tpMaterial>(m_material.get(),"material"));
	//m_fields.add(new tpSingleField<tpTexture>(m_texture,"texture"));

	//m_material.setValue(NULL);
	//m_texture.setValue(NULL);
	//m_culling.setValue(FALSE);
}


tpRenderable::tpRenderable(const tpRenderable& geo)
	: tpNode(geo),
	mMaterial(geo.mMaterial),
	mTexture(geo.mTexture)
{
}

tpObject* 
tpRenderable::clone()
{
	return new tpRenderable(*this);
}

TP_TYPE_REGISTER(tpRenderable,tpNode,Geometry);
