#include <tp/egl.h>

#include <tp/log.h>

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
			;
}

tpEGL* tpEGL::get( bool destroy /*= false*/ )
{
	static tpRefPtr<tpEGL> mEGL;
	if (!mEGL.isValid()) mEGL = new tpEGL;
	if (destroy) mEGL = 0L;
	return mEGL.get();
}

bool tpRenderContextEGL::swapBuffers() {
	return tpEGL::a().SwapBuffers.f(display,surface);
}

bool tpRenderContextEGL::makeCurrent() {
	return tpEGL::a().MakeCurrent.f(display,surface,surface,context);
}

bool tpRenderContextEGL::create(tpRenderTarget* target) {

	tpArray<EGLint> attributes;

//	attributes.add(EGL_RENDERABLE_TYPE).add(EGL_OPENGL_ES2_BIT);
	attributes.add(EGL_RENDERABLE_TYPE).add(EGL_OPENGL_BIT);

	switch (target->getTargetType())
	{
		case tpRenderTarget::kWindow:
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
	display = tpEGL::a().GetDisplay.f((NativeDisplayType)(target) ? target->getDisplay() : EGL_DEFAULT_DISPLAY);

	EGLint glMajor(0),glMinor(0);

	/* initialize the EGL display connection */
	if (!tpEGL::a().Initialize.f(display, &glMajor, &glMinor))
	{
		tpLogError("%s - eglInitialize failed",__FUNCTION__);

	} else {

		tpLogNotify("%s - EGL got implementation %d.%d\n\tVendor: %s, Version: %s\n\tExtensions:%s\n\tClient APIs:%s"
			,__FUNCTION__,glMajor,glMinor,
			tpEGL::a().QueryString.f(display,EGL_VENDOR),
			tpEGL::a().QueryString.f(display,EGL_VERSION),
			tpEGL::a().QueryString.f(display,EGL_EXTENSIONS),
			tpEGL::a().QueryString.f(display,EGL_CLIENT_APIS)
		);
	}

	tpEGL::a().BindAPI.f(EGL_OPENGL_BIT);

	tpArray<EGLint> context_attributes;
	EGLConfig config = 0L;

	EGLint num_config = 1;
	/* get an appropriate EGL frame buffer configuration */
	if (!tpEGL::a().ChooseConfig.f(display, &attributes.front(), &config, num_config, &num_config))
	{
		tpLogError("%s - eglChooseConfig failed",__FUNCTION__);
	} else
	{
		tpLogNotify("%s - eglChooseConfig returned %d configuration(s)",__FUNCTION__,num_config);

		EGLint red_,green_,blue_,alpha_;

		tpEGL::a().GetConfigAttrib.f(display,config,EGL_RED_SIZE,&red_);
		tpEGL::a().GetConfigAttrib.f(display,config,EGL_GREEN_SIZE,&green_);
		tpEGL::a().GetConfigAttrib.f(display,config,EGL_BLUE_SIZE,&blue_);
		tpEGL::a().GetConfigAttrib.f(display,config,EGL_ALPHA_SIZE,&alpha_);

		tpLogNotify("%s config has %d:%d:%d:%d format",__FUNCTION__,red_,green_,blue_,alpha_);

		//status = tpGL::eglGetConfigAttrib(display, config, attribute, value);
	}



//	context_attributes.add(EGL_CONFIG_ID).add(0);
//	context_attributes.add(EGL_CONTEXT_CLIENT_VERSION).add(2);
//	context_attributes.add(EGL_NONE).add(EGL_NONE);

	/* create an EGL rendering context */
	context = tpEGL::a().CreateContext.f(display, config, EGL_NO_CONTEXT,/*(const EGLint*)&context_attributes.front()*/0L);
	if (EGL_NO_CONTEXT == context)
	{
		tpLogError("%s - eglCreateContext failed (0x%x)",__FUNCTION__,tpEGL::a().GetError.f());

	} else {

		tpLogMessage("%s - eglCreateContext successful",__FUNCTION__);

		makeCurrent();

		tpInt w(0),h(0);
		tpEGL::a().QuerySurface.f(display,surface,EGL_WIDTH,&w);
		tpEGL::a().QuerySurface.f(display,surface,EGL_HEIGHT,&h);

		tpLogMessage("%s - actual surface area %dx%d",__FUNCTION__,w,h);

	}

	/* create an EGL window surface */
	/* This needs to be adapted for ES 1.x */
	if (target->getTargetType() == tpRenderTarget::kWindow)
	{
		surface = tpEGL::a().CreateWindowSurface.f(display,config,target->getWindow(),NULL);

		if (EGL_NO_SURFACE == surface)
		{
			tpLogError("%s - eglCreateWindowSurface failed (0x%x)",__FUNCTION__,tpEGL::a().GetError.f());
		}

	} else {

		surface = tpEGL::a().CreatePbufferSurface.f(display,config,NULL);
		if (surface == 0)
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
