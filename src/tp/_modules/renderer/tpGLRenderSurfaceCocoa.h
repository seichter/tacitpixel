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


#include "tpGLRenderer.h"
#include "tpRenderer.h"
#include "tpRenderSurface.h"

#include <Cocoa/Cocoa.h>


class tpGLRenderSurfaceCocoa : public tpRenderSurface {
public:
	
	TP_TYPE_DECLARE;
	
	tpGLRenderSurfaceCocoa( tpRenderSurfaceTraits* traits );
	
	virtual tpBool makeCurrent();
	virtual tpBool swapBuffers();
	
	virtual tpString getString(tpUInt glenum);
	
	virtual tpBool show(tpBool doShow);	
	
	
	tpVoid setCaption(const tpString& caption);
	
	
	tpRenderer* createDefaultRenderer() const 
	{
		return new tpGLRenderer;
	}
	
	NSWindow* getWindow() { return window; }
	
protected:
	
	NSAutoreleasePool *pool;
	NSWindow *window;
	NSOpenGLContext* oglcontext;
	NSObject* delegate;
	
	virtual ~tpGLRenderSurfaceCocoa();
	
};


#endif