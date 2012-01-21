#include "rendercontext_glx.h"

#if defined(TP_USE_X11)

#include <tp/rendersurface.h>
#include <tp/log.h>

tpRenderContextGLX::tpRenderContextGLX()
    : tpRenderContext()
    , display(0)
    , window(0)
{
}

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

    if (target->getType()->isOfType(tpRenderSurface::getTypeInfo()))
    {

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

        glxcontext = glXCreateContext(display,vi,0,True);

        if (glxcontext) {

            if (this->makeCurrent())
            {
                mVersion = this->getString(GL_VERSION);
                mVendor = this->getString(GL_VENDOR);
                mRenderer = this->getString(GL_RENDERER);
                mExtensions = this->getString(GL_EXTENSIONS);
                return true;
            }
        }
    } else {

        //window = glXCreateGLXPixmap(display,vi,window);

    }

    return false;
}

void
tpRenderContextGLX::wait(tpUInt e) {
    switch (e) {
    case kWaitUI:
        glXWaitX();
        break;
    case kWaitGL:
        glXWaitGL();
        break;
    default:
        break;
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

tpString
tpRenderContextGLX::getString(const tpUInt& e)
{
    const tpChar* str = reinterpret_cast<const tpChar*>(glGetString(e));
    return tpString(str);
}

TP_TYPE_REGISTER(tpRenderContextGLX,tpRenderContext,RenderContextX11);

#endif
