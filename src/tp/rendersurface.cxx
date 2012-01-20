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

#include "tp/rendersurface.h"

#include "tp/renderer.h"
#include "tp/node.h"
#include "tp/camera.h"
#include "tp/log.h"
#include "tp/timer.h"
#include "tp/module.h"

//////////////////////////////////////////////////////////////////////////

tpRenderSurfaceTraits::tpRenderSurfaceTraits()
	: title("GL Window"),
	size(tpVec2i(320,240)),
	position(tpVec2i(0,0))
{
}

tpVec2i tpRenderSurfaceTraits::getSize() const
{
	return size;
}

tpRenderSurfaceTraits& tpRenderSurfaceTraits::setSize( tpVec2i val )
{
	size = val; return *this;
}

tpRenderSurfaceTraits& tpRenderSurfaceTraits::setSize( tpInt width, tpInt height )
{
	return setSize(tpVec2i(width,height));
}

tpString tpRenderSurfaceTraits::getTitle() const
{
	return title;
}

tpRenderSurfaceTraits& tpRenderSurfaceTraits::setTitle( const tpString& val )
{
	title = val; return *this;
}

bool tpRenderSurfaceTraits::useDefaultSize()
{
	return (size[0] < 0 || size[1] < 0);
}

tpRenderSurfaceTraits& tpRenderSurfaceTraits::setDefaultSize()
{
	return setSize(-1,-1);
}

tpVec2i tpRenderSurfaceTraits::getPosition() const
{
	return position;
}

tpRenderSurfaceTraits& tpRenderSurfaceTraits::setPosition( tpVec2i val )
{
	position = val;
	return *this;
}

tpRenderSurfaceTraits& tpRenderSurfaceTraits::setPosition( tpInt x, tpInt y )
{
	return setPosition( tpVec2i( x, y ) );
}

tpRenderSurfaceTraits& tpRenderSurfaceTraits::setDefaultPosition()
{
	return setPosition(-1,-1);
}

bool tpRenderSurfaceTraits::useDefaultPosition()
{
	return (position[0] < 0 || position[1] < 0);
}

void* tpRenderSurfaceTraits::getWindowHandle() const
{
	return window_handle;
}

tpRenderSurfaceTraits& tpRenderSurfaceTraits::setWindowHandle( void* val )
{
	window_handle = val; return *this;
}

bool tpRenderSurfaceTraits::isFullscreen() const
{
	return fullscreen;
}

tpRenderSurfaceTraits& tpRenderSurfaceTraits::setFullscreen( bool val /*= true*/ )
{
	fullscreen = val; return *this;
}


//////////////////////////////////////////////////////////////////////////

#if defined(_WIN32)
#elif defined(TP_USE_COCOA)
	#include "_impl/rendersurface_cocoa.h"
#elif defined(TP_USE_X11)
	#include "_impl/rendersurface_x11.h"
#endif


tpRenderSurface* tpRenderSurface::create( tpRenderSurfaceTraits* traits /* =0 */)
{
	tpRenderSurface* surface(0);
#if 0
	tpRenderSurfaceFactory* surface_factory(0);

	tpLogNotify("%d modules",tpModuleManager::get()->getModules().getSize());

	const tpModuleList& modules = tpModuleManager::get()->getModules();

	for (tpModuleList::const_iterator iter = modules.begin(); iter != modules.end(); ++iter)
	{
		if ((*iter)->getType()->isOfType(tpRenderSurfaceFactory::getTypeInfo()))
		{
			surface_factory = static_cast<tpRenderSurfaceFactory*>(iter->get());
		}
	}


	if (surface_factory)
	{

		surface = surface_factory->create( traits );

	} else {

		tpLogError("%s no render surface factory available",__FUNCTION__);
	}

#endif

#if defined(TP_USE_COCOA)
	surface = new tpRenderSurfaceCocoa(traits);
#elif defined(TP_USE_X11)
	surface = new tpRenderSurfaceX11(traits);
#endif

	return surface;
}


tpRenderSurface::tpRenderSurface()
	: tpRenderTarget()
	, mDone(false)
{
}


tpRenderSurface::tpRenderSurface( tpRenderSurfaceTraits* traits )
	: tpRenderTarget()
	, mDone(false)
{
}

tpRenderSurface::~tpRenderSurface()
{
}

tpString
tpRenderSurface::getName() const
{
	return tpString("RenderSurface");
}

tpRenderContext::tpRenderContext()
	: tpReferenced()
{
}

tpRenderTarget::tpRenderTarget()
	: tpReferenced()
{
}

TP_TYPE_REGISTER(tpRenderSurfaceFactory,tpReferenced,RenderSurfaceFactory);
TP_TYPE_REGISTER(tpRenderContext,tpReferenced,RenderContext);
TP_TYPE_REGISTER(tpRenderTarget,tpReferenced,RenderTarget);
TP_TYPE_REGISTER(tpRenderSurface,tpRenderTarget,RenderSurface);





