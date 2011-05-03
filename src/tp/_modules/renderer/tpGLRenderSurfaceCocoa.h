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

#ifndef TPGLRENDERSURFACECOCOA_H
#define TPGLRENDERSURFACECOCOA_H

#include <Cocoa/Cocoa.h>

#include "tpGLRenderer.h"

#include <tp/rendersurface.h>
#include <tp/log.h>


class tpGLRenderSurfaceCocoa : public tpRenderSurface {
public:
	
	TP_TYPE_DECLARE;
	
	tpGLRenderSurfaceCocoa( tpRenderSurfaceTraits* traits );
	
	virtual bool makeCurrent();
	virtual bool swapBuffers();
	
	virtual tpString getString(tpUInt glenum);
	
	virtual bool show(bool doShow);	
	
	
	tpVoid setCaption(const tpString& caption);
	
	
	tpRenderer* createDefaultRenderer() const 
	{
		return new tpGLRenderer;
	}
	
	NSWindow* getWindow() { return window; }
	
protected:
	
//	NSAutoreleasePool *pool;
	NSWindow *window;
	NSOpenGLContext *oglcontext;
	NSObject *delegate;
	
	virtual ~tpGLRenderSurfaceCocoa();
	
};


#endif
