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

