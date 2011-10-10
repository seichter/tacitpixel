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

#include "tp/renderer.h"
#include "tp/module.h"
#include "tp/log.h"


TP_TYPE_REGISTER(tpRenderer,tpReferenced,Renderer);

tpRenderer::tpRenderer()
	: tpReferenced()
	, mActiveCamera(0)
{
}

tpRenderer::tpRenderer(const tpRenderer& other)
	: tpReferenced()
{
}

tpRenderer::~tpRenderer()
{
}

tpRenderer & tpRenderer::operator =(const tpRenderer &rhs)
{
	if (this != &rhs) {
		mCameras = rhs.mCameras;
		mActiveCamera = rhs.mActiveCamera;
	}

	return *this;
}


tpCamera * tpRenderer::getActiveCamera()
{
	if (mCameras.isEmpty())
	{
		tpCamera* camera = new tpCamera();

		camera->setProjectionPerspective(60.0f,1.3f,0.1f,1000.0f);
		camera->setViewLookAt(tpVec3r(0,0,1),tpVec3r(0,0,0),tpVec3r(0,1,0));

		addCamera(camera,true);
	}

	return mCameras[mActiveCamera].get();
}

void
tpRenderer::setActiveCamera(tpUInt camera)
{
	if (camera < mCameras.getSize())
	{
		mActiveCamera = camera;
	}
}


void
tpRenderer::addCamera(tpCamera *camera,
						   bool makeActive /*= true*/)
{
	if (makeActive) mActiveCamera = mCameras.getSize();
	mCameras.add(camera);
}

void
tpRenderer::removeCamera(tpCamera *camera)
{
	mCameras.erase(mCameras.find(camera));
}

tpRenderer* tpRenderer::create( const tpRendererTraits& traits )
{

	tpRenderer* renderer = 0;

	//tpLogNotify("%s has %d module(s)",__FUNCTION__, tpModuleManager::instance()->getModules().getSize());


	tpModuleList modules = tpModuleManager::get()->getModules();

	for (tpUInt i = 0; i < modules.getSize(); i++)
	{
		tpRefPtr<tpReferenced> item = modules[i];

		if (item->getType()->isOfType(tpRenderer::getTypeInfo()))
		{
			renderer = static_cast<tpRenderer*>(item.get());
			if ( renderer->getTraits() = traits )
			{
				tpLogNotify( "%s loaded %s",__FUNCTION__, item->getType()->getName());

				return renderer;

			} else {

				renderer = 0;
			}

		}
	}

	return renderer;
}




