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
