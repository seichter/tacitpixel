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

#include "rendersurface_x11.h"

#include <tp/module.h>

tpGLRenderSurfaceX11::tpGLRenderSurfaceX11( tpRenderSurfaceTraits* traits )
	: tpRenderSurface( traits )
	, dpy(0)
	, win(0)
{
	doLinking( traits );
	doCreate( traits );
}

static int __tempwnd_id = 0;


void tpGLRenderSurfaceX11::doLinking( tpRenderSurfaceTraits* traits )
{
	tpGL::get().load(tpGL::TP_GL_PREFETCH);
	//tpGL::get().load(TP_GL_FULL);
}


void tpGLRenderSurfaceX11::doCreateEGL()
{

	tpLogMessage("EGL!");


/* EGL aliases */
	#define EGL_FALSE			0
	#define EGL_TRUE			1

	/* Out-of-band handle values */
	/* These values may vary depending on semantics of native concepts */
	#define EGL_DEFAULT_DISPLAY		((void *)0)
	#define EGL_NO_CONTEXT			((EGLContext)0)
	#define EGL_NO_DISPLAY			((EGLDisplay)0)
	#define EGL_NO_SURFACE			((EGLSurface)0)

	/* Config attributes */
	#define EGL_BUFFER_SIZE			0x3020
	#define EGL_ALPHA_SIZE			0x3021
	#define EGL_BLUE_SIZE			0x3022
	#define EGL_GREEN_SIZE			0x3023
	#define EGL_RED_SIZE			0x3024
	#define EGL_DEPTH_SIZE			0x3025
	#define EGL_STENCIL_SIZE		0x3026
	#define EGL_CONFIG_CAVEAT		0x3027
	#define EGL_CONFIG_ID			0x3028
	#define EGL_LEVEL			0x3029
	#define EGL_MAX_PBUFFER_HEIGHT		0x302A
	#define EGL_MAX_PBUFFER_PIXELS		0x302B
	#define EGL_MAX_PBUFFER_WIDTH		0x302C
	#define EGL_NATIVE_RENDERABLE		0x302D
	#define EGL_NATIVE_VISUAL_ID		0x302E
	#define EGL_NATIVE_VISUAL_TYPE		0x302F
	#define EGL_PRESERVED_RESOURCES		0x3030
	#define EGL_SAMPLES			0x3031
	#define EGL_SAMPLE_BUFFERS		0x3032
	#define EGL_SURFACE_TYPE		0x3033
	#define EGL_TRANSPARENT_TYPE		0x3034
	#define EGL_TRANSPARENT_BLUE_VALUE	0x3035
	#define EGL_TRANSPARENT_GREEN_VALUE	0x3036
	#define EGL_TRANSPARENT_RED_VALUE	0x3037
	#define EGL_NONE			0x3038	/* Attrib list terminator */
	#define EGL_BIND_TO_TEXTURE_RGB		0x3039
	#define EGL_BIND_TO_TEXTURE_RGBA	0x303A
	#define EGL_MIN_SWAP_INTERVAL		0x303B
	#define EGL_MAX_SWAP_INTERVAL		0x303C
	#define EGL_LUMINANCE_SIZE		0x303D
	#define EGL_ALPHA_MASK_SIZE		0x303E
	#define EGL_COLOR_BUFFER_TYPE		0x303F
	#define EGL_RENDERABLE_TYPE		0x3040
	#define EGL_CONTEXT_CLIENT_VERSION 0x3098
	
	/* Config attribute mask bits */
	#define EGL_PBUFFER_BIT			0x0001	/* EGL_SURFACE_TYPE mask bits */
	#define EGL_PIXMAP_BIT			0x0002	/* EGL_SURFACE_TYPE mask bits */
	#define EGL_WINDOW_BIT			0x0004	/* EGL_SURFACE_TYPE mask bits */
	#define EGL_VG_COLORSPACE_LINEAR_BIT	0x0020	/* EGL_SURFACE_TYPE mask bits */
	#define EGL_VG_ALPHA_FORMAT_PRE_BIT	0x0040	/* EGL_SURFACE_TYPE mask bits */
	#define EGL_MULTISAMPLE_RESOLVE_BOX_BIT 0x0200	/* EGL_SURFACE_TYPE mask bits */
	#define EGL_SWAP_BEHAVIOR_PRESERVED_BIT 0x0400	/* EGL_SURFACE_TYPE mask bits */

	#define EGL_OPENGL_ES_BIT		0x0001	/* EGL_RENDERABLE_TYPE mask bits */
	#define EGL_OPENVG_BIT			0x0002	/* EGL_RENDERABLE_TYPE mask bits */
	#define EGL_OPENGL_ES2_BIT		0x0004	/* EGL_RENDERABLE_TYPE mask bits */
	#define EGL_OPENGL_BIT			0x0008	/* EGL_RENDERABLE_TYPE mask bits */

	/* QueryString targets */
	#define EGL_VENDOR			0x3053
	#define EGL_VERSION			0x3054
	#define EGL_EXTENSIONS			0x3055
	#define EGL_CLIENT_APIS			0x308D


	static EGLint const attribute_list[] = {

		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, // OPENGL_ES 1.x == 1 2.0 == 4
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT /*| EGL_PBUFFER_BIT*/,
		//EGL_DEPTH_SIZE, 16,
		/*
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE, 5,
		*/
		EGL_NONE
	};

	/* get an EGL display connection */
	//egl_display = tpGL::eglGetDisplay((NativeDisplayType)EGL_DEFAULT_DISPLAY);
	egl_display = tpGL::eglGetDisplay((NativeDisplayType)dpy);

	EGLint glMajor(0),glMinor(0);

	/* initialize the EGL display connection */
	if (!tpGL::eglInitialize(egl_display, &glMajor, &glMinor))
	{
		tpLogError("%s - eglInitialize failed",__FUNCTION__);
	} else {

		tpLogNotify("%s - EGL got implementation %d.%d\n\tVendor: %s, Version: %s\n\tExtensions:%s\n\tClient APIs:%s"
			,__FUNCTION__,glMajor,glMinor,
			tpGL::eglQueryString(egl_display,EGL_VENDOR),tpGL::eglQueryString(egl_display,EGL_VERSION),
			tpGL::eglQueryString(egl_display,EGL_EXTENSIONS),
			tpGL::eglQueryString(egl_display,EGL_CLIENT_APIS)
		);
	}


	EGLint num_config = 1;

	//if (!tpGL::eglChooseConfig(display, attribute_list, NULL, 0, &num_config))
	//{
	//	tpLogError("%s - eglChooseConfig failed",__FUNCTION__);
	//}
	//config = (EGLConfig*)malloc(num_config * sizeof(EGLConfig));

	EGLConfig config = 0;
	/* get an appropriate EGL frame buffer configuration */
	if (!tpGL::eglChooseConfig(egl_display, attribute_list, &config, num_config, &num_config))
	{
		tpLogError("%s - eglChooseConfig failed",__FUNCTION__);
	} else
	{
		tpLogNotify("%s - eglChooseConfig returned %d configuration(s)",__FUNCTION__,num_config);

		EGLBoolean status;
		int attribute, *value;
		//status = tpGL::eglGetConfigAttrib(display, config, attribute, value);
	}

	/* create an EGL window surface */
	/* This needs to be adapted for ES 1.x */
	EGLint ContextAttributes[] = 
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE, EGL_NONE 
	};

	egl_surface = tpGL::eglCreateWindowSurface(egl_display, config, (tpVoid*)win, NULL);
	if (egl_surface == 0)
	{
		tpLogError("%s - eglCreateWindowSurface failed (0x%x)",__FUNCTION__,tpGL::eglGetError());
	} else {

		tpLogMessage("%s - eglCreateWindowSurface successful",__FUNCTION__);

	}

	/* create an EGL rendering context */
	egl_context = tpGL::eglCreateContext(egl_display, config, EGL_NO_CONTEXT,(const EGLint*)&ContextAttributes);
	if (EGL_NO_CONTEXT == egl_context)
	{
		tpLogError("%s - eglCreateContext failed (%s)",__FUNCTION__,tpGL::GetString(tpGL::eglGetError()));
	
	} else {

		tpLogMessage("%s - eglCreateContext successful",__FUNCTION__);

		GLboolean res = tpGL::eglMakeCurrent(egl_display,egl_surface,egl_surface,egl_context);
		tpLogMessage("%s - eglMakeCurrent for attaching %d",__FUNCTION__,res);

#define EGL_HEIGHT 0x3056
#define EGL_WIDTH 0x3057
		tpInt w(0),h(0);
		tpGL::eglQuerySurface(egl_display,egl_surface,EGL_WIDTH,&w);
		tpGL::eglQuerySurface(egl_display,egl_surface,EGL_HEIGHT,&h);
		tpLogMessage("%s - actual surface area %dx%d",__FUNCTION__,w,h);

	}

}


void tpGLRenderSurfaceX11::doCreateGLX(XVisualInfo* vi)
{

	tpInt glx_major(0), glx_minor(0), screen(0);

	if (!tpGL::glXQueryExtension(dpy,0,0)) 
	{
		tpLogError("X server has no OpenGL GLX extension");
	}
	else {
	
		tpGL::glXQueryVersion(dpy,&glx_major,&glx_minor);
	
		tpLogNotify("%s - GLX Extension %d.%d",__FUNCTION__,glx_major,glx_minor);
	}

/*
#define GLX_RGBA 4	
#define GLX_DOUBLEBUFFER 5
#define GLX_DEPTH_SIZE 12
	
	tpInt depth_bits = 24;
	
	tpInt configuration[] = {GLX_DOUBLEBUFFER, GLX_RGBA, GLX_DEPTH_SIZE, depth_bits, None};

	
	vi = tpGL::glXChooseVisual(dpy, screen, configuration);
	
	if (vi == NULL)
	{		
		if (vi == NULL) tpLogError("no appropriate RGB visual with depth buffer");
		vi = tpGL::glXChooseVisual(dpy, screen, &configuration[0]);
	    //	m_doublebuffer = FALSE;
	};
*/
	
	cx = tpGL::glXCreateContext(dpy, vi, /* no sharing of display lists */ NULL,
	                       /* direct rendering if possible */ tpGL::_TRUE);

	if (cx == NULL) tpLogError("could not create rendering context");
}



void tpGLRenderSurfaceX11::doCreate( tpRenderSurfaceTraits* traits ) {
	
	tpGL::get();

	XVisualInfo *vi(0L);
	long screen(0L);
	
	XSetWindowAttributes swa;
	// XWMHints       *wmHints;

#if 1	
	int depth_bits = 24;
	
#define GLX_RGBA 4	
#define GLX_DOUBLEBUFFER 5
#define GLX_DEPTH_SIZE 12
	
	//int configuration[] = {GLX_DOUBLEBUFFER, GLX_RGBA, GLX_DEPTH_SIZE, depth_bits, None};

#endif


	static tpString s_display;

	// should actually check if we can use "getenv"
	if (s_display.isEmpty()) s_display.set(getenv("DISPLAY"));
	
	if (s_display.isEmpty()) s_display = ":0.0";
	
	dpy = XOpenDisplay(s_display.c_str());	

	tpLogMessage("tpGLWindow::create() on display %s",s_display.c_str());
	
	if (dpy == NULL) tpLogError("could not open display %s",s_display.c_str());
	
	screen = XDefaultScreen( dpy );

	Window root_window = RootWindow(dpy, screen);

	int depth = DefaultDepth(dpy, screen);
	vi = new XVisualInfo;
	XMatchVisualInfo( dpy, screen, depth, TrueColor, vi);
	if (!vi)
	{
		tpLogError("Error: Unable to acquire visual with %dbpp\n",depth);
	}

	Colormap colormap = XCreateColormap( dpy, root_window, vi->visual, AllocNone );
	swa.colormap = colormap;

	// this is desktop GL stuff - egl needs different treatment

#if 0
	swa.colormap = XCreateColormap(dpy, 
		RootWindow(dpy, vi->screen),
        vi->visual, AllocNone);
#endif

	//swa.border_pixel = 0;
	//swa.background_pixel = 0;

	swa.event_mask = ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | KeyPressMask;
	unsigned int mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;


	int width(640),height(480);
	int pos_x(0),pos_y(0);

	if (traits) 
	{
		width = traits->getSize()[0];
		height = traits->getSize()[1];
		pos_x = traits->getPosition()[0];
		pos_y = traits->getPosition()[1];
	}


	win = XCreateWindow( dpy, root_window, pos_x, pos_y, width, height, 0, CopyFromParent, InputOutput, CopyFromParent, mask, &swa);
/*
	win = XCreateWindow(dpy, root_window,
	                    pos_x, pos_y, width, height,
	                    0, vi->depth, InputOutput, vi->visual,
	                    mask, 
						&swa);	
*/

	if (traits) setCaption(traits->getTitle());

	
	XMapWindow(dpy, win);
	XMapRaised(dpy, win);
	XFlush(dpy);


	XSync(dpy, 0 );

#if 0
	XSizeHints sh;
	sh.flags = 0;
	sh.flags |= USSize;
	sh.flags &= 0x7;
	sh.flags |= USPosition;
	sh.flags &= 0xB;
	sh.x = 0;
	sh.y = 0;
	sh.width  = width;
	sh.height = height;

	const char* title = "tpWindow";
	
	XSetStandardProperties(dpy,win,title,title,None,0,0,&sh);
	// XSetWMHints(dpy, win, wmHints);


	
	wmDeleteWindow = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dpy, win, &wmDeleteWindow, 1);
	
	
	XMapWindow(dpy, win);
	
		
	XFlush(dpy );
    XSync(dpy, 0 );

	tpLogNotify("%s created window",__FUNCTION__);

#endif

	if (tpGL::get().hasEGL()){
		// non-EGL version

		doCreateEGL();
	
	} else {
		doCreateGLX(vi);
		
	}
	
	XSync(dpy, 0 );

	this->makeCurrent();
	
	tpLogNotify("%s - OpenGL driver\n\tVendor:%s\n\tRenderer:%s\n\tVersion:%s\n\tExtensions:\n\t%s",
				__FUNCTION__,
				tpGL::GetString(tpGL::VENDOR),
				tpGL::GetString(tpGL::RENDERER),
				tpGL::GetString(tpGL::VERSION),
				tpGL::GetString(tpGL::EXTENSIONS));
	
	tpGL::get().load(tpGL::TP_GL_FULL);
}

tpGLRenderSurfaceX11::~tpGLRenderSurfaceX11()
{
	doKill();
}

void tpGLRenderSurfaceX11::doKill()
{
	if (tpGL::glXDestroyContext) tpGL::glXDestroyContext(dpy,cx);

	XDestroyWindow(dpy,win);
	XCloseDisplay(dpy);
	
}

bool tpGLRenderSurfaceX11::show(bool doShow) {

	return true;

}

bool 
tpGLRenderSurfaceX11::makeCurrent()
{
//!\TODO: clear this mess up!
	if (tpGL::eglMakeCurrent) {
		return tpGL::eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context) != false ? true : false;
	} else if (tpGL::glXMakeCurrent) {
		return tpGL::glXMakeCurrent(dpy,win,cx) == true ? true : false;
	}

	return false;
}

bool tpGLRenderSurfaceX11::swapBuffers()
{
	if (tpGL::eglSwapBuffers) 
	{
		return tpGL::eglSwapBuffers(egl_display, egl_surface);
	} else if (tpGL::glXSwapBuffers) {
		tpGL::glXSwapBuffers(dpy,win);
		return true;
	}

	//tpLogError("%s - no swapbuffer!",__FUNCTION__);

	return false;
}


tpRenderer* tpGLRenderSurfaceX11::createDefaultRenderer() const
{
	return new tpGLRenderer();
}

tpString tpGLRenderSurfaceX11::getString( tpUInt glenum )
{
	makeCurrent();
	tpString result;
	result.set(reinterpret_cast<const tpChar*>(tpGL::GetString(glenum)));
	return result;
}

tpVoid tpGLRenderSurfaceX11::frame()
{
	tpRenderSurface::frame();
}

tpVoid tpGLRenderSurfaceX11::setCaption(const tpString& caption)
{
	XStoreName(dpy,win,caption.c_str());
}


//////////////////////////////////////////////////////////////////////////

class tpRenderSurfaceFactoryX11 : public tpRenderSurfaceFactory {
public:

	TP_TYPE_DECLARE;

	tpRenderSurfaceFactoryX11() : tpRenderSurfaceFactory()
	{
		//tpLogMessage("%s - added X11 Surface Factory");
	}

	tpRenderSurface* create( tpRenderSurfaceTraits* traits )
	{
		return new tpGLRenderSurfaceX11( traits );
	}
};


TP_TYPE_REGISTER(tpRenderSurfaceFactoryX11,tpRenderSurfaceFactory,RenderSurfaceFactoryX11);
TP_TYPE_REGISTER(tpGLRenderSurfaceX11,tpRenderSurface,GLRenderSurfaceX11);

TP_MODULE_REGISTER(x11surface,tpRenderSurfaceFactoryX11);


bool tpRenderContextGLX::makeCurrent()
{
	return true;
}

bool tpRenderContextGLX::swapBuffers()
{
	return true;
}

bool tpRenderContextEGL::makeCurrent()
{
	return true;
}

