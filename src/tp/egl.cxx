/*
 * Copyright (C) 1999-2013 Hartmut Seichter
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "tp/egl.h"
#include "tp/log.h"
#include "tp/renderer.h"


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
//	glClearColor(1.f,0,0,1);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

bool
tpRenderContextEGL::init(tpRenderTarget* target)
{
	/* get an EGL display connection */
    egl_display =
            tpEGL::a().GetDisplay.f((EGLNativeDisplayType)(target)
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



    tpArray<EGLint> attributes;

//	attributes.add(EGL_RENDERABLE_TYPE).add(EGL_OPENGL_ES2_BIT);
    attributes.add(EGL_RENDERABLE_TYPE).add(EGL_OPENGL_BIT);

    switch (target->getTargetType())
    {
        case tpRenderTarget::kWindow:

            tpLogMessage("%s - rendertarget - Window",__FUNCTION__);
            attributes.add(EGL_SURFACE_TYPE).add(EGL_WINDOW_BIT);
            attributes.add(EGL_DEPTH_SIZE).add(8);

//			attributes.add(EGL_RED_SIZE).add(1);
//			attributes.add(EGL_GREEN_SIZE).add(1);
//			attributes.add(EGL_BLUE_SIZE).add(1);

//            attributes.add(EGL_NATIVE_VISUAL_ID).add(0x21);

//            attributes.add(EGL_NATIVE_RENDERABLE).add(EGL_TRUE);

            break;
        case tpRenderTarget::kPbuffer:

            // pbuffers in GLES only with GLES 1
            attributes.back() = EGL_OPENGL_ES_BIT;

            attributes.add(EGL_SURFACE_TYPE).add(EGL_PBUFFER_BIT);
//			attributes.add(EGL_WIDTH).add(target->getWidth());
//			attributes.add(EGL_HEIGHT).add(target->getHeight());
            break;
    }

    attributes.add(EGL_NONE);



	tpArray<EGLint> context_attributes;

	EGLConfig egl_config = 0;
	EGLint num_config = 1;

	/* get an appropriate EGL frame buffer configuration */
    if (!tpEGL::a().ChooseConfig.f(egl_display, &attributes.front(), &egl_config, 1, &num_config))
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

    /* bind the API */
    if(!tpEGL::a().BindAPI.f(EGL_OPENGL_API))
    {
        tpLogError("%s - eglBindAPI failed",__FUNCTION__);
    }



	// here we need to set the native

//	context_attributes.add(EGL_CONFIG_ID).add(0);
//	context_attributes.add(EGL_CONTEXT_CLIENT_VERSION).add(2);
    context_attributes.add(EGL_NONE).add(EGL_NONE);


	/* create an EGL rendering context */
	egl_context = tpEGL::a().CreateContext.f(egl_display, egl_config, EGL_NO_CONTEXT,
										context_attributes.getSize() ? (const EGLint*)context_attributes.getData() : 0L);



    if (EGL_NO_CONTEXT == egl_context) {
		tpLogError("%s - eglCreateContext failed (0x%x)",__FUNCTION__,tpEGL::a().GetError.f());

	} else {

        tpLogMessage("%s - eglCreateContext succeeded (0x%x)",__FUNCTION__,tpEGL::a().GetError.f());
	}

	/* create an EGL window surface */
	/* This needs to be adapted for ES 1.x */
	if (target->getTargetType() == tpRenderTarget::kWindow)
	{
		egl_surface = tpEGL::a().CreateWindowSurface.f(egl_display,
													   egl_config,
                                                       (EGLNativeWindowType)target->getWindow(),
													   NULL);
        if (egl_surface)
        {
            tpLogMessage("%s - eglCreateWindowSurface succeeded 0x%x",__FUNCTION__,tpEGL::a().GetError.f());


            if(EGL_FALSE == tpEGL::a().MakeCurrent.f(egl_display,egl_surface,egl_surface,egl_context))
            {
                tpLogMessage("%s - eglMakeCurrent failed 0x%x",__FUNCTION__,tpEGL::a().GetError.f());
            }

			tpInt w(0),h(0), wb(0);
            if (tpEGL::a().QuerySurface.f(egl_display,egl_surface,EGL_HEIGHT,&h)) {
                tpLogError("Surface: %d",h);
            } else {
                tpLogError("Can't query surface 0x%x",tpEGL::a().GetError.f());
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
