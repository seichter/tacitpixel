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

tpRenderSurface* tpRenderSurface::create( tpRenderSurfaceTraits* traits /* =0 */)
{
	tpRenderSurface* surface(0);
	tpRenderSurfaceFactory* surface_factory(0);

	const tpModuleList& modules = tpModuleManager::get()->getModules();

	for (tpUInt i = 0; i < modules.getSize(); i++)
	{		
		tpRefPtr<tpReferenced> item = modules[i];

		if (item->getType()->isOfType(tpRenderSurfaceFactory::getTypeInfo())) 
		{
			surface_factory = reinterpret_cast<tpRenderSurfaceFactory*>(item.get());
		}
	}

		
	if (surface_factory) 
	{

		surface = surface_factory->create( traits );

		// this should be done by the user application
		if (surface)
		{
			surface->setRenderer(surface->createDefaultRenderer());
		}

	} else {
		
		tpLogError("%s no render surface factory available",__FUNCTION__);
	}
	
	return surface;
}

tpRenderSurface::tpRenderSurface( tpRenderSurfaceTraits* traits ) : m_done(false)
{
	m_camera = new tpCamera();
}

tpRenderSurface::~tpRenderSurface()
{
}

tpString tpRenderSurface::getName() const
{
	return tpString("RenderSurface");
}

tpRenderer* tpRenderSurface::createDefaultRenderer() const
{
	return 0;
}

void tpRenderSurface::setRenderer( tpRenderer* renderer )
{
	m_renderer = renderer;
}

tpRenderer* tpRenderSurface::getRenderer()
{
	return m_renderer.get();
}

const tpRenderer* tpRenderSurface::getRenderer() const
{
	return m_renderer.get();
}

void tpRenderSurface::setSceneNode( tpNode* node )
{
	m_root = node;
}

tpNode* tpRenderSurface::getSceneNode()
{
	return m_root.get();
}

const tpNode* tpRenderSurface::getSceneNode() const
{
	return m_root.get();
}

void tpRenderSurface::frame()
{	
	if (this->makeCurrent() && !m_done && this->getRenderer())
	{
		this->getRenderer()->apply(m_root.get(),m_camera.get());

		//tpLogNotify("%s - %d",__FUNCTION__,this->swapBuffers());
		
		this->swapBuffers();
	}
}

void tpRenderSurface::setCamera( tpCamera* camera )
{
	m_camera = camera;
}

tpCamera* tpRenderSurface::getCamera()
{
	return m_camera.get();
}

const tpCamera* tpRenderSurface::getCamera() const
{
	return m_camera.get();
}

void tpRenderSurface::setDone( bool isDone /*= true*/ )
{
	m_done = isDone;
}

bool tpRenderSurface::isDone() const
{
	return m_done;
}


void tpRenderSurface::setMouseAdapter( tpMouseAdapter* mouseadapter )
{
	m_mouseadapter = mouseadapter;
}

tpMouseAdapter* tpRenderSurface::getMouseAdapter()
{
	return m_mouseadapter.get();
}


TP_TYPE_REGISTER(tpRenderSurfaceFactory,tpReferenced,RenderSurfaceFactory);
TP_TYPE_REGISTER(tpRenderSurface,tpReferenced,RenderSurface);

