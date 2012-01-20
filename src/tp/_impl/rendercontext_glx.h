#ifndef TPRENDERCONTEXTGLX_H
#define TPRENDERCONTEXTGLX_H

#if defined(TP_USE_X11)

#include <tp/rendersurface.h>

#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>


class tpRenderContextGLX : public tpRenderContext {

	GLXContext glxcontext;

	Display* display;
	Window window;

public:

	TP_TYPE_DECLARE

	bool create(tpRenderTarget *target);

	bool makeCurrent();
	bool swapBuffers();

	void destroy();
};

#endif

#endif
