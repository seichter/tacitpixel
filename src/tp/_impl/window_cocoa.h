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

#include <tp/config.h>

#if 1 //defined(TP_USE_COCOA)

#ifdef __OBJC__
	#import <Cocoa/Cocoa.h>
#else
	struct NSWindow;
	struct NSOpenGLContext;
	struct NSObject;
#endif

#include <tp/window.h>

class tpWindowCocoa : public tpWindow {
public:

	TP_TYPE_DECLARE

	tpWindowCocoa( tpWindowTraits *traits );

	virtual bool show(bool doShow);

	virtual void destroy();

	virtual void update();

	virtual tpVec2i getSize() const;
	virtual void setSize(tpInt w, tpInt h);


	void setCaption(const tpString& caption);

	tpRawPtr getWindow() { return window; }

	void setContext(tpRenderContext *context);

protected:

	NSWindow *window;
	NSObject *delegate;

	virtual ~tpWindowCocoa();

};

#endif

#endif
