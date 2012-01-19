#include "rendercontext_glx.h"

#include <tp/log.h>

bool
tpRenderContextGLX::create(tpRenderTarget *target)
{
	tpInt glx_major(0), glx_minor(0), screen(0);

	this->display = reinterpret_cast<Display*>(target->getDisplay());
	this->window =  reinterpret_cast<Window>(target->getWindow());

	if (False == glXQueryExtension(display,0,0)) {

		tpLogError("X server has no OpenGL GLX extension");
		return false;
	} else {

		glXQueryVersion(display,&glx_major,&glx_minor);
		tpLogNotify("%s - GLX Extension %d.%d",__FUNCTION__,glx_major,glx_minor);
	}

	const unsigned int depth_bits = 24;

	tpArray<int> configuration;
	configuration
			.add(GLX_DOUBLEBUFFER)
			.add(GLX_RGBA)
			.add(GLX_DEPTH_SIZE).add(depth_bits)
			.add(None);

	XVisualInfo* vi = 0;

	vi = glXChooseVisual(display, screen, &configuration[0]);

	if (vi == NULL)
	{
		if (vi == NULL) tpLogError("no appropriate RGB visual with depth buffer");
		vi = glXChooseVisual(display, screen, &configuration[0]);
	}
}

bool
tpRenderContextGLX::makeCurrent() {
	return (True == glXMakeCurrent(display,window,glxcontext));
}

bool
tpRenderContextGLX::swapBuffers() {
	glXSwapBuffers(display,window);
	return true;
}

void
tpRenderContextGLX::destroy() {
	// \todo implement!
}

TP_TYPE_REGISTER(tpRenderContextGLX,tpRenderContext,RenderContextX11);
