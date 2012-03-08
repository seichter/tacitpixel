#include "rendercontext_glx.h"

#if defined(TP_USE_X11)

#include <tp/window.h>
#include <tp/log.h>

#define GLX_SAMPLE_BUFFERS_SGIS         100000
#define GLX_SAMPLES_SGIS                100001

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
    tpString glx_extensions;

	this->display = reinterpret_cast<Display*>(target->getDisplay());
	this->window =  reinterpret_cast<Window>(target->getWindow());

	if (False == glXQueryExtension(display,0,0)) {

		tpLogError("X server has no OpenGL GLX extension");
		return false;
	} else {

		glXQueryVersion(display,&glx_major,&glx_minor);
        glx_extensions = glXQueryExtensionsString(display,0);


        tpLogNotify("%s - GLX Extension %d.%d\n%s",
                    __FUNCTION__,glx_major,glx_minor,
                    glx_extensions.c_str());
	}

    if (target->getType()->isOfType(tpWindow::getTypeInfo()))
    {


        const unsigned int depth_bits = 24;

        tpArray<int> configuration;
        configuration
                .add(GLX_DOUBLEBUFFER)
                .add(GLX_RGBA)
//                .add(GLX_RED_SIZE).add(8)
//                .add(GLX_GREEN_SIZE).add(8)
//                .add(GLX_BLUE_SIZE).add(8)
//                .add(GLX_ALPHA_SIZE).add(8)
//                .add(GLX_BUFFER_SIZE).add(24)
                .add(GLX_DEPTH_SIZE).add(depth_bits);

        if (glx_extensions.find("GLX_ARB_multisample") != tpString::kNotFound)
        {
            //configuration.add(GLX_SAMPLE_BUFFERS_ARB).add(1);
            //configuration.add(GLX_SAMPLE_BUFFERS_ARB).add(1);
        }


        configuration.add(None);


        XVisualInfo* vi = 0;

        vi = glXChooseVisual(display, screen, &configuration[0]);

        if (vi == NULL)
        {
            tpLogError("%s GLX could not find a matching visual",__FUNCTION__);
        }

        glxcontext = glXCreateContext(display,vi,0,True);

        if (glxcontext) {

            if (this->makeCurrent())
            {
                mVersion = this->getString(GL_VERSION);
                mVendor = this->getString(GL_VENDOR);
                mRenderer = this->getString(GL_RENDERER);
                mExtensions = this->getString(GL_EXTENSIONS);

                reportToConsole();

                glEnable(GL_MULTISAMPLE_ARB);

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

    glXMakeCurrent(display,window,glxcontext);
    glXDestroyContext(display,glxcontext);
	// \todo implement!
}

tpRenderContextGLX::~tpRenderContextGLX()
{
    this->destroy();
}

tpString
tpRenderContextGLX::getString(const tpUInt& e)
{
    const tpChar* str = reinterpret_cast<const tpChar*>(glGetString(e));
    return tpString(str);
}

void*
tpRenderContextGLX::getProcAddress(const char *name)
{
    return (void*)glXGetProcAddressARB((const unsigned char*)name);
}

TP_TYPE_REGISTER(tpRenderContextGLX,tpRenderContext,RenderContextX11);

#endif
