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

tpRenderSurface*
tpRenderSurface::create( tpRenderSurfaceTraits* traits /* =0 */)
{
	tpRenderSurfaceFactory* surface_factory(0);

	const tpModuleList& modules = tpModuleManager::get()->getModules();

	for (tpModuleList::const_iterator iter = modules.begin();
		 iter != modules.end();
		 ++iter)
	{
		if ((*iter)->getType()->isOfType(tpRenderSurfaceFactory::getTypeInfo()))
		{
			surface_factory = static_cast<tpRenderSurfaceFactory*>(iter->get());
			return surface_factory->create( traits );
		}
	}

	return 0L;
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

tpVec2i tpRenderSurface::getSize() const
{
	return tpVec2i(-1,-1);
}


//////////////////////////////////////////////////////////////////////////


tpRenderTarget::tpRenderTarget()
	: tpReferenced()
	, mContext(0)
{
}

void
tpRenderTarget::setContext(tpRenderContext* context)
{
	mContext = context;
	if (mContext.isValid()) mContext->create(this);
}

tpRenderContext*
tpRenderTarget::getContext()
{
	return mContext.get();
}

//////////////////////////////////////////////////////////////////////////

tpRenderBuffer::tpRenderBuffer()
{
}

tpRenderBuffer::~tpRenderBuffer()
{
}

/*static*/ tpRenderBuffer*
tpRenderBuffer::create(const tpSize& size, const tpUInt pixelformat) {

	tpRenderBufferFactory* factory(0);

	const tpModuleList& modules = tpModuleManager::get()->getModules();

	for (tpModuleList::const_iterator iter = modules.begin();
		 iter != modules.end();
		 ++iter)
	{
		if ((*iter)->getType()->isOfType(tpRenderBufferFactory::getTypeInfo()))
		{
			factory = static_cast<tpRenderBufferFactory*>(iter->get());
			return factory->create( size, pixelformat );
		}
	}

	return 0L;
}

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

TP_TYPE_REGISTER(tpRenderSurfaceFactory,tpReferenced,RenderSurfaceFactory);
TP_TYPE_REGISTER(tpRenderBufferFactory,tpReferenced,RenderBufferFactory);
TP_TYPE_REGISTER(tpRenderTarget,tpReferenced,RenderTarget);
TP_TYPE_REGISTER(tpRenderSurface,tpRenderTarget,RenderSurface);
TP_TYPE_REGISTER(tpRenderBuffer,tpRenderTarget,RenderBuffer);





