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

#include "tpGLRenderSurfaceEmbedded.h"
#include "tpGLRenderer.h"

#include <tp/system.h>
#include <tp/camera.h>
#include <tp/module.h>
#include <tp/version.h>

tpGLRenderSurfaceEmbedded::tpGLRenderSurfaceEmbedded( tpRenderSurfaceTraits* traits )
: tpRenderSurface( traits )	
{

	tpGL::get().load(tpGL::TP_GL_PREFETCH);

	this->makeCurrent();

	tpLogNotify("%s - OpenGL driver\n\t%s %s %s\n%s",__FUNCTION__,
		tpGL::GetString(tpGL::VENDOR),
		tpGL::GetString(tpGL::RENDERER),
		tpGL::GetString(tpGL::VERSION),
		tpGL::GetString(tpGL::EXTENSIONS));

	tpGL::get().load(tpGL::TP_GL_FULL);
}


tpGLRenderSurfaceEmbedded::~tpGLRenderSurfaceEmbedded()
{
}

bool tpGLRenderSurfaceEmbedded::show(bool doShow) {
	return true;
}

bool tpGLRenderSurfaceEmbedded::makeCurrent()
{
	return true;
}

bool tpGLRenderSurfaceEmbedded::swapBuffers()
{
	return true;
}


tpRenderer* tpGLRenderSurfaceEmbedded::createDefaultRenderer() const
{
	return new tpGLRenderer();
}


tpString tpGLRenderSurfaceEmbedded::getString( tpUInt glenum )
{
	makeCurrent();
	tpString result;
	result.set(reinterpret_cast<const tpChar*>(tpGL::GetString(glenum)));
	return result;
}

void tpGLRenderSurfaceEmbedded::frame()
{
	tpRenderSurface::frame();
}


//////////////////////////////////////////////////////////////////////////

class tpGLRenderSurfaceEmbeddedFactory : public tpRenderSurfaceFactory {
public:

	TP_TYPE_DECLARE;

	tpGLRenderSurfaceEmbeddedFactory() 
	{
		tpLogNotify("%s OpenGL support on",tpGetVersionString());
	}

	~tpGLRenderSurfaceEmbeddedFactory()
	{
		tpLogNotify("%s OpenGL support off",tpGetVersionString());
	}

	tpRenderSurface* create( tpRenderSurfaceTraits* traits )
	{
		return new tpGLRenderSurfaceEmbedded( traits );
	}
};

TP_TYPE_REGISTER(tpGLRenderSurfaceEmbeddedFactory,tpRenderSurfaceFactory,RenderSurfaceFactoryEmbedded);
TP_TYPE_REGISTER(tpGLRenderSurfaceEmbedded,tpRenderSurface,GLRenderSurfaceEmbedded);

TP_MODULE_REGISTER(embeddedsurface,tpGLRenderSurfaceEmbeddedFactory);
