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

#ifndef TPGLRENDERSURFACEX11_H
#define TPGLRENDERSURFACEX11_H


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>


#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#elif defined(__unix)
//#include <GL/gl.h>
//#include <GL/glext.h>
#include <GL/glx.h>
#endif


//#include "tpGL.h"

#include <tp/rendersurface.h>
#include <tp/log.h>


class tpRenderContextGLX : public tpRenderContext {

	bool makeCurrent();
	bool swapBuffers();

};

class tpRenderContextEGL : public tpRenderContext {

	bool makeCurrent();
	bool swapBuffers();

};

class tpGLRenderSurfaceX11 : public tpRenderSurface {
public:

	TP_TYPE_DECLARE;

	tpGLRenderSurfaceX11( tpRenderSurfaceTraits* traits );

	void setDisplay(const tpString&);

	tpString getString(tpUInt glenum);


	virtual tpVoid frame();

	virtual tpString getName() const { return tpString("Twisted Pair OpenGL surface 1.0"); }	


protected:

	virtual ~tpGLRenderSurfaceX11();

	tpGLRenderSurfaceX11 &operator= (const tpGLRenderSurfaceX11 &) { return *this; }

	void doCreate( tpRenderSurfaceTraits* traits );
	void doLinking( tpRenderSurfaceTraits* traits );


	void doCreateEGL();
	void doCreateGLX(XVisualInfo*);

	void doKill();
	bool show(bool doShow);

	void setCaption(const tpString& caption);


private:

	GLXContext cx;
	Display *dpy;
	Window win;
	Atom wmDeleteWindow;
		

	EGLContext egl_context;
	EGLSurface egl_surface;
	EGLDisplay egl_display;



};


#endif
