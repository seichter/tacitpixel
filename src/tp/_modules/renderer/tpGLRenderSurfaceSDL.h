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

#ifndef TPGLRENDERSURFACESDL_H 
#define TPGLRENDERSURFACESDL_H

#include <tpRenderSurface.h>

#include <SDL.h>


class tpGLRenderSurfaceSDL : public tpRenderSurface
{
public:
	tpGLRenderSurfaceSDL( tpRenderSurfaceTraits* traits );
	
	virtual tpBool makeCurrent();
	virtual tpBool swapBuffers();
	
	virtual tpString getString(tpUInt glenum);
	
	virtual tpBool show(tpBool doShow);	
	
	virtual tpVoid frame();
	
protected:
	
	SDL_Surface *m_surface;
	
	virtual ~tpGLRenderSurfaceSDL();
	
};

#endif
