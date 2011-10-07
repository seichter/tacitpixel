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

#ifndef TPGLRENDERSURFACEWIN32_H
#define TPGLRENDERSURFACEWIN32_H

#include "tpRenderSurface.h"
#include "tpLog.h"
#include "tpGL.h"

class tpGLRenderSurfaceWin32 : public tpRenderSurface {
public:

	tpGLRenderSurfaceWin32( tpRenderSurfaceTraits* traits );

	HWND  getHandle() const    { return _handle; }
	HDC   getDC() const        { return _dc; }
	HGLRC getContext() const   { return _context; }

	tpString getString(tpUInt glenum);

	tpBool makeCurrent();
	tpBool swapBuffers();

	virtual tpString getName() const { return tpString("Twisted Pair OpenGL surface 1.0"); }

	tpRenderer* createDefaultRenderer() const;


protected:

	virtual ~tpGLRenderSurfaceWin32();

	tpGLRenderSurfaceWin32 &operator= (const tpGLRenderSurfaceWin32 &) { return *this; }

	void doCreate();
	void doKill();
	tpBool show(tpBool doShow);


private:
	HWND _handle;
	HDC _dc;
	HGLRC _context;
	HINSTANCE _instance;
	tpString _classname;

	EGLDisplay display;
	EGLConfig config;
	EGLContext context;
	EGLSurface surface;
	EGLint num_config;
};


#endif