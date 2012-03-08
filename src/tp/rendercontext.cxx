#include <tp/rendercontext.h>
#include <tp/stringtokenizer.h>

#include <tp/log.h>

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

TP_TYPE_REGISTER(tpRenderContext,tpReferenced,RenderContext);
