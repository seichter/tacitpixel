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

#include "tp/renderer.h"
#include "tp/module.h"
#include "tp/log.h"

tpRenderer::tpRenderer()
{
}

tpRenderer::~tpRenderer()
{
}


tpRenderer* tpRenderer::create( tpUInt backend )
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
			if ( renderer && renderer->implementsBackend() == backend )
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


TP_TYPE_REGISTER(tpRenderer,tpReferenced,Renderer);

