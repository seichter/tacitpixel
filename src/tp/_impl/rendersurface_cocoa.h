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


#ifdef __OBJC__
	#import <Cocoa/Cocoa.h>
#else
	struct NSWindow;
	struct NSOpenGLContext;
	struct NSObject;
#endif

#include <tp/rendersurface.h>
#include <tp/log.h>




class tpRenderSurfaceCocoa : public tpRenderSurface {
public:

	TP_TYPE_DECLARE;

	tpRenderSurfaceCocoa( tpRenderSurfaceTraits* traits );

//	virtual bool makeCurrent();
//	virtual bool swapBuffers();

//	virtual void frame();

//	virtual tpString getString(tpUInt glenum);

	virtual bool show(bool doShow);

	virtual void update();

	void setCaption(const tpString& caption);

	tpRawPtr getWindow() { return window; }

protected:

	NSWindow *window;
	NSObject *delegate;

	virtual ~tpRenderSurfaceCocoa();

};


#endif
