/*
 * Copyright (C) 1999-2013 Hartmut Seichter
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

#include "tp/rendercontext.h"
#include "tp/stringtokenizer.h"
#include "tp/log.h"
#include "tp/module.h"

tpRenderContext::tpRenderContext()
	: tpReferenced()
{
}

tpRenderContext::~tpRenderContext()
{
}

tpString
tpRenderContext::getString(const tpUInt &e)
{
	return tpString();
}

void
tpRenderContext::getExtensions(tpStringArray& extensions) const
{
	tpString ext = this->getExtensions();
	extensions = tpStringTokenizer::split(ext," ");
}

void*
tpRenderContext::getProcAddress(const char* name)
{
	return 0L;
}

void tpRenderContext::reportToConsole()
{
	tpLogNotify("Context OpenGL %s\n\tExtensions: %s",
				this->getVersion().c_str(),
				this->getExtensions().c_str()
				);
}

tpRenderContext*
tpRenderContext::create(const tpString& cfg)
{
	
	tpRenderContext* rendercontext = 0;

	tpModuleList modules = tpModuleManager::get()->getModules();

	tpLogNotify("Found %d modules",modules.getSize());

	for (tpUInt i = 0; i < modules.getSize(); i++)
	{
		tpRefPtr<tpReferenced> item = modules[i];

		if (item->getType()->isOfType(tpRenderContext::getTypeInfo()))
		{
			rendercontext = static_cast<tpRenderContext*>(item.get());
			
			tpLogNotify("Found %s",rendercontext->getName().c_str());
			
			if (cfg.isEmpty()) {
				return rendercontext;
			} else if (rendercontext->getName().contains(cfg))
			{
				return rendercontext;
			}
		}
	}

	return rendercontext;
}


TP_TYPE_REGISTER(tpRenderContext,tpReferenced,RenderContext);
