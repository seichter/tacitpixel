#include "tp/egl.h"
#include "tp/log.h"
#include "tp/renderer.h"

#include "GL/gl.h"

tpEGL::tpEGL()
{
	getFunctions()
			.push(&GetError,"eglGetError")
			.push(&Initialize,"eglInitialize")
			.push(&GetDisplay,"eglGetDisplay")
			.push(&GetConfigs,"eglGetConfigs")
			.push(&ChooseConfig,"eglChooseConfig")
			.push(&CreateContext,"eglCreateContext")
			.push(&CreateWindowSurface,"eglCreateWindowSurface")
			.push(&CreatePbufferSurface,"eglCreatePbufferSurface")
			.push(&MakeCurrent,"eglMakeCurrent")
			.push(&SwapBuffers,"eglSwapBuffers")
			.push(&QuerySurface,"eglQueryContext")
			.push(&GetConfigAttrib,"eglGetConfigAttrib")
			.push(&QueryString,"eglQueryString")
			.push(&BindAPI,"eglBindAPI")
			.push(&GetProcAddress,"eglGetProcAddress")
			;
}

tpEGL* tpEGL::get( bool destroy /*= false*/ )
{
	static tpRefPtr<tpEGL> mEGL;
	if (!mEGL.isValid()) mEGL = new tpEGL;
	if (destroy) mEGL = 0L;
	return mEGL.get();
}

bool
tpRenderContextEGL::swapBuffers()
{
    glClearColor(1.f,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return (tpEGL::a().SwapBuffers.f(egl_display,egl_surface) > 0);
}

tpUInt
tpRenderContextEGL::getRendererTraits() const
{
    return (tpRenderer::kOpenGLES1);
}

bool
tpRenderContextEGL::makeCurrent() {
    return (tpEGL::a().MakeCurrent.f(egl_display,egl_surface,egl_surface,egl_context) > 0);
}

tpUInt
tpRenderContextEGL::getVisualId() const
{
    return 0x21;
}

bool tpRenderContextEGL::init(tpRenderTarget* target) {

	tpArray<EGLint> attributes;

//	attributes.add(EGL_RENDERABLE_TYPE).add(EGL_OPENGL_ES2_BIT);
//	attributes.add(EGL_RENDERABLE_TYPE).add(EGL_OPENGL_BIT);

	switch (target->getTargetType())
	{
		case tpRenderTarget::kWindow:

            tpLogError("%s - rendertarget - Window",__FUNCTION__);

            attributes.add(EGL_RED_SIZE).add(1);
            attributes.add(EGL_GREEN_SIZE).add(1);
            attributes.add(EGL_BLUE_SIZE).add(1);
            attributes.add(EGL_DEPTH_SIZE).add(1);

//            attributes.add(EGL_NATIVE_VISUAL_ID).add(0x21);

            attributes.add(EGL_NATIVE_RENDERABLE).add(EGL_TRUE);

            attributes.add(EGL_SURFACE_TYPE).add(EGL_WINDOW_BIT);

			break;
		case tpRenderTarget::kPbuffer:

			// pbuffers in GLES only with GLES 1
			attributes.back() = EGL_OPENGL_ES_BIT;

			attributes.add(EGL_SURFACE_TYPE).add(EGL_PBUFFER_BIT);
//			attributes.add(EGL_WIDTH).add(target->getWidth());
//			attributes.add(EGL_HEIGHT).add(target->getHeight());
			break;
	}

	attributes.add(EGL_NONE).add(EGL_NONE);

	/* get an EGL display connection */
    egl_display = tpEGL::a().GetDisplay.f((EGLNativeDisplayType)(target)
                                      ? target->getDisplay() : EGL_DEFAULT_DISPLAY);

	EGLint glMajor(0),glMinor(0);

	/* initialize the EGL display connection */
    if (!tpEGL::a().Initialize.f(egl_display, &glMajor, &glMinor))
	{
		tpLogError("%s - eglInitialize failed",__FUNCTION__);

	} else {

        tpLogNotify("tpEGL\n\tEGL API %d.%d\n\tEGL Vendor: %s\n\tEGL Version: %s\n\tEGL Extensions: %s\n\tEGL Client APIs: %s"
			,glMajor,glMinor,
            tpEGL::a().QueryString.f(egl_display,EGL_VENDOR),
            tpEGL::a().QueryString.f(egl_display,EGL_VERSION),
            tpEGL::a().QueryString.f(egl_display,EGL_EXTENSIONS),
            tpEGL::a().QueryString.f(egl_display,EGL_CLIENT_APIS)
		);
	}


	tpArray<EGLint> context_attributes;

    EGLConfig egl_config = 0;
    EGLint num_config = 1;

	/* get an appropriate EGL frame buffer configuration */
    if (!tpEGL::a().ChooseConfig.f(egl_display, &attributes.front(), &egl_config, num_config, &num_config))
	{
		tpLogError("%s - eglChooseConfig failed",__FUNCTION__);

	} else
	{

        tpLogNotify("%s - eglChooseConfig returned %d configuration(s)",__FUNCTION__,num_config);

        EGLint red_,green_,blue_,alpha_, vid_;

        tpEGL::a().GetConfigAttrib.f(egl_display,egl_config,EGL_RED_SIZE,&red_);
        tpEGL::a().GetConfigAttrib.f(egl_display,egl_config,EGL_GREEN_SIZE,&green_);
        tpEGL::a().GetConfigAttrib.f(egl_display,egl_config,EGL_BLUE_SIZE,&blue_);
        tpEGL::a().GetConfigAttrib.f(egl_display,egl_config,EGL_ALPHA_SIZE,&alpha_);
        tpEGL::a().GetConfigAttrib.f(egl_display,egl_config,EGL_NATIVE_VISUAL_ID,&vid_);

        tpLogNotify("%s config has %d:%d:%d:%d format (vid: 0x%x)",
                    __FUNCTION__,red_,green_,blue_,alpha_,vid_);

		//status = tpGL::eglGetConfigAttrib(display, config, attribute, value);
	}


    // here we need to set the native

//	context_attributes.add(EGL_CONFIG_ID).add(0);
//	context_attributes.add(EGL_CONTEXT_CLIENT_VERSION).add(2);
//	context_attributes.add(EGL_NONE).add(EGL_NONE);


	/* bind the API */
    if(!tpEGL::a().BindAPI.f(EGL_OPENGL_API))
    {
        tpLogError("%s - eglBindAPI failed",__FUNCTION__);
    }

	/* create an EGL rendering context */
    egl_context = tpEGL::a().CreateContext.f(egl_display, egl_config, EGL_NO_CONTEXT,
										context_attributes.getSize() ? (const EGLint*)context_attributes.getData() : 0L);
    if (EGL_NO_CONTEXT == egl_context)
	{
		tpLogError("%s - eglCreateContext failed (0x%x)",__FUNCTION__,tpEGL::a().GetError.f());

	} else {

		tpLogMessage("%s - eglCreateContext successful",__FUNCTION__);

		makeCurrent();
	}

	/* create an EGL window surface */
	/* This needs to be adapted for ES 1.x */
	if (target->getTargetType() == tpRenderTarget::kWindow)
	{
        egl_surface = tpEGL::a().CreateWindowSurface.f(egl_display,
                                                       egl_config,
                                                       (EGLNativeWindowType)target->getWindow(),
                                                       NULL);

        if (EGL_NO_SURFACE == egl_surface)
		{
			tpLogError("%s - eglCreateWindowSurface failed (0x%x)",__FUNCTION__,tpEGL::a().GetError.f());
		} else {

            tpLogError("%s - eglCreateWindowSurface succeeded",__FUNCTION__,tpEGL::a().GetError.f());


            tpInt w(0),h(0), wb(0);
            if (EGL_TRUE != tpEGL::a().QuerySurface.f(egl_display,egl_surface,EGL_VERTICAL_RESOLUTION,&w))
            {

                tpLogError("Can't query surface");
            }

            //			tpEGL::a().QuerySurface.f(display,surface,EGL_HEIGHT,&h);

//			tpEGL::a().GetConfigAttrib.f(display,config,EGL_SURFACE_TYPE,&wb);

//        tpLogMessage("%s - actual surface area %dx%d (%d)",__FUNCTION__,w,h,wb & EGL_WINDOW_BIT);

		}

	} else {

        egl_surface = tpEGL::a().CreatePbufferSurface.f(egl_display,egl_config,NULL);
        if (egl_surface == 0)
		{
			tpLogError("%s - eglCreatePbufferSurface failed (0x%x)",__FUNCTION__,tpEGL::a().GetError.f());
		}
	}



//		EGLBoolean res = tpEGL::a().MakeCurrent.f(display,surface,surface,context);
//		tpLogMessage("%s - eglMakeCurrent for attaching %d",__FUNCTION__,res);

//		tpInt w(0),h(0);
//		tpEGL::a().QuerySurface.f(display,surface,EGL_WIDTH,&w);
//		tpEGL::a().QuerySurface.f(display,surface,EGL_HEIGHT,&h);

//		tpLogMessage("%s - actual surface area %dx%d",__FUNCTION__,w,h);

//		return true;

//	}

	return false;

}

void tpRenderContextEGL::destroy() {

	//\todo implement!

}
