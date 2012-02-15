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

#include <tp/config.h>

#if defined(TP_USE_X11)

#include "rendersurface_x11.h"
#include "rendercontext_glx.h"

#include <tp/log.h>
#include <tp/image.h>
#include <tp/module.h>
#include <tp/version.h>

#include <X11/Xutil.h>
#include <X11/keysym.h>


tpRenderSurfaceX11::tpRenderSurfaceX11( tpRenderSurfaceTraits* traits )
	: tpRenderSurface( traits )
	, dpy(0)
	, win(0)
{
	doCreate( traits );
}

void
tpRenderSurfaceX11::doCreate( tpRenderSurfaceTraits* traits ) {

	XVisualInfo *vi(0L);
	long screen(0L);

	XSetWindowAttributes swa;
	// XWMHints       *wmHints;

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

    Atom wmDelete=XInternAtom(dpy, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(dpy, win, &wmDelete, 1);

	if (traits) setCaption(traits->getTitle());

    XMapWindow(dpy, win);

    //XFlush(dpy);

}

bool
tpRenderSurfaceX11::show(bool doShow)
{
    if (doShow) {
        XRaiseWindow(dpy,win);
    } else {
        XLowerWindow(dpy,win);
    }

	return true;
}

void
tpRenderSurfaceX11::setCaption(const tpString& caption)
{
	XStoreName(dpy,win,caption.c_str());
}

void
tpRenderSurfaceX11::update()
{

	if (XPending(dpy)) {
	
		XEvent event;
		XNextEvent(dpy,&event);

        tpRenderSurfaceEvent e(this);
        //e.setMousePosition(point.x,point.y);
        e.setRenderSurface(this);
        bool submit = false;

		switch (event.type) {
		case ConfigureNotify:
			tpLogNotify("%s - configure",__FUNCTION__);
			break;
		case KeyPress:
		case KeyRelease:
			{
				XKeyEvent* ke = (XKeyEvent*)&event;
				e.setKeyCode(ke->keycode);
				tpLogNotify("%s - key pressed (%d)",__FUNCTION__,XKeycodeToKeysym(dpy,ke->keycode,0));			
			}
            submit = true;
			break;
		case ButtonPress:
			tpLogNotify("%s - button pressed",__FUNCTION__);
			break;
		case DestroyNotify:
		case ClientMessage:
			tpLogNotify("%s - request to close",__FUNCTION__);
			this->setDone();
            submit = true;
			break;
		default:
			tpLogNotify("%s - got an unknown event %d",__FUNCTION__,event.type);
			break;
		}

        if (submit) this->getEventHandler().process(e);
	}
}

void
tpRenderSurfaceX11::destroy()
{
    mContext = 0;

	XDestroyWindow(dpy,win);
	XCloseDisplay(dpy);
}

tpRenderSurfaceX11::~tpRenderSurfaceX11()
{
	destroy();
}

void
tpRenderSurfaceX11::setContext(tpRenderContext* context)
{
    if (context == 0)
    {
        context = new tpRenderContextGLX();
    }

    tpRenderTarget::setContext(context);
}



////////////////////////////////////////////////////////////////////////////

class tpRenderSurfaceFactoryX11 : public tpRenderSurfaceFactory {
public:

    TP_TYPE_DECLARE;

    tpRenderSurfaceFactoryX11() : tpRenderSurfaceFactory()
    {
        tpLogNotify("%s X11 RenderSurface",tpGetVersionString());
    }

    tpRenderSurface* create( tpRenderSurfaceTraits* traits )
    {
        return new tpRenderSurfaceX11( traits );
    }
};

TP_TYPE_REGISTER(tpRenderSurfaceX11,tpRenderSurface,RenderSurfaceX11);
TP_TYPE_REGISTER(tpRenderSurfaceFactoryX11,tpRenderSurfaceFactory,RenderSurfaceFactoryX11);
TP_MODULE_REGISTER(x11surface,tpRenderSurfaceFactoryX11)


#endif


