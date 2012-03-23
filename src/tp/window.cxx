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

#include "tp/window.h"

#include "tp/renderer.h"
#include "tp/node.h"
#include "tp/camera.h"
#include "tp/log.h"
#include "tp/timer.h"
#include "tp/module.h"


//////////////////////////////////////////////////////////////////////////

tpWindow*
tpWindow::create( tpWindowTraits* traits /* =0 */)
{
	tpWindowFactory* surface_factory(0);

	const tpModuleList& modules = tpModuleManager::get()->getModules();

	for (tpModuleList::const_iterator iter = modules.begin();
		 iter != modules.end();
		 ++iter)
	{
		if ((*iter)->getType()->isOfType(tpWindowFactory::getTypeInfo()))
		{
			surface_factory = static_cast<tpWindowFactory*>(iter->get());
			return surface_factory->create( traits );
		}
	}

	return 0L;
}

tpWindow::tpWindow()
	: tpRenderTarget()
	, mDone(false)
{
}


tpWindow::tpWindow( tpWindowTraits* traits )
	: tpRenderTarget()
	, mDone(false)
{
}

tpWindow::~tpWindow()
{
}


tpString
tpWindow::getName() const
{
	return tpString("RenderSurface");
}

tpVec2i tpWindow::getSize() const
{
	return tpVec2i(-1,-1);
}

void tpWindow::setSize( tpInt w, tpInt h )
{
}

tpVec2i
tpWindow::getClientAreaSize() const
{
	return tpVec2i(-1,-1);
}

void
tpWindow::setClientAreaSize( tpUInt w, tpUInt h )
{
}

void tpWindow::setPosition( tpInt x, tpInt y )
{

}

tpVec2i tpWindow::getPosition() const
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
	if (mContext.isValid()) mContext->init(this);
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

tpWindowTraits::tpWindowTraits()
	: title("GL Window"),
	size(tpVec2i(320,240)),
	position(tpVec2i(0,0))
{
}

tpVec2i tpWindowTraits::getSize() const
{
	return size;
}

tpWindowTraits& tpWindowTraits::setSize( tpVec2i val )
{
	size = val; return *this;
}

tpWindowTraits& tpWindowTraits::setSize( tpInt width, tpInt height )
{
	return setSize(tpVec2i(width,height));
}

tpString tpWindowTraits::getTitle() const
{
	return title;
}

tpWindowTraits& tpWindowTraits::setTitle( const tpString& val )
{
	title = val; return *this;
}

bool tpWindowTraits::useDefaultSize()
{
	return (size(0) < 0 || size(1) < 0);
}

tpWindowTraits& tpWindowTraits::setDefaultSize()
{
	return setSize(-1,-1);
}

tpVec2i tpWindowTraits::getPosition() const
{
	return position;
}

tpWindowTraits& tpWindowTraits::setPosition( tpVec2i val )
{
	position = val;
	return *this;
}

tpWindowTraits& tpWindowTraits::setPosition( tpInt x, tpInt y )
{
	return setPosition( tpVec2i( x, y ) );
}

tpWindowTraits& tpWindowTraits::setDefaultPosition()
{
	return setPosition(-1,-1);
}

bool tpWindowTraits::useDefaultPosition()
{
	return (position(0) < 0 || position(1) < 0);
}

void* tpWindowTraits::getWindowHandle() const
{
	return window_handle;
}

tpWindowTraits& tpWindowTraits::setWindowHandle( void* val )
{
	window_handle = val; return *this;
}

bool tpWindowTraits::isFullscreen() const
{
	return fullscreen;
}

tpWindowTraits& tpWindowTraits::setFullscreen( bool val /*= true*/ )
{
	fullscreen = val; return *this;
}

TP_TYPE_REGISTER(tpWindowFactory,tpReferenced,RenderSurfaceFactory);
TP_TYPE_REGISTER(tpRenderBufferFactory,tpReferenced,RenderBufferFactory);
TP_TYPE_REGISTER(tpRenderTarget,tpReferenced,RenderTarget);
TP_TYPE_REGISTER(tpWindow,tpRenderTarget,RenderSurface);
TP_TYPE_REGISTER(tpRenderBuffer,tpRenderTarget,RenderBuffer);





