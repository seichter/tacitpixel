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

#include "window_x11.h"
#include "rendercontext_glx.h"

#include <tp/log.h>
#include <tp/image.h>
#include <tp/module.h>
#include <tp/version.h>
#include <tp/mutex.h>
#include <tp/scopelock.h>


#include <X11/Xutil.h>
#include <X11/XKBlib.h>


tpWindowX11::tpWindowX11( tpWindowTraits* traits )
    : tpWindow( traits )
	, dpy(0)
	, win(0)
{
	doCreate( traits );
}

void
tpWindowX11::doCreate( tpWindowTraits* traits ) {

	XVisualInfo *vi(0L);
	long screen(0L);

	XSetWindowAttributes swa;
	// XWMHints       *wmHints;

    tpString s_display;

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
    swa.event_mask = ExposureMask | StructureNotifyMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | KeyPressMask | KeyReleaseMask;
	unsigned int mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;


	int width(640),height(480);
	int pos_x(0),pos_y(0);

	if (traits)
	{
        width = traits->getSize()(0);
        height = traits->getSize()(1);
        pos_x = traits->getPosition()(0);
        pos_y = traits->getPosition()(1);
	}

    XVisualInfo *visInfo(0), visTemplate;

    if (traits && traits->getVisualId())
    {
        visTemplate.visualid = 0x21;
        int num_visuals = 0;
        visInfo = XGetVisualInfo(dpy, VisualIDMask, &visTemplate, &num_visuals);
    }





//    tpLogMessage("Visuals: %d",num_visuals);

//    for (int k = 0; k < num_visuals;++k) {
//        tpLog::get().printf("vid: 0x%x",visInfo->visualid);
//        visInfo++;
//    }


    if (visInfo)
    {
        win = XCreateWindow( dpy, root_window, pos_x, pos_y, width, height, 0, visInfo->depth,
                             InputOutput, visInfo->visual, mask, &swa);

    } else {
        win = XCreateWindow( dpy, root_window, pos_x, pos_y, width, height, 0, CopyFromParent, InputOutput, CopyFromParent, mask, &swa);
    }

    Atom wmDelete=XInternAtom(dpy, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(dpy, win, &wmDelete, 1);

    {
        XSizeHints sizehints;
        sizehints.x = pos_x;
        sizehints.y = pos_y;
        sizehints.width  = width;
        sizehints.height = height;
        sizehints.flags = USSize | USPosition;
        XSetNormalHints(dpy, win, &sizehints);
//        XSetStandardProperties(x_dpy, win, name, name,
//                                None, (char **)NULL, 0, &sizehints);
//
    }



	if (traits) setCaption(traits->getTitle());

    delete vi;

    XMapWindow(dpy, win);
    XFlush(dpy);

}

bool
tpWindowX11::show(bool doShow)
{
    if (doShow) {
        XRaiseWindow(dpy,win);
    } else {
        XLowerWindow(dpy,win);
    }

	return true;
}

tpVec2i
tpWindowX11::getSize() const {
    tpVec2i r;
    XWindowAttributes xwa;
    XGetWindowAttributes(dpy,win,&xwa);
    r(0) = xwa.width;
    r(1) = xwa.height;

    return r;
}

tpVec2i
tpWindowX11::getClientAreaSize() const
{
    return getSize();
}


void
tpWindowX11::setClientAreaSize(tpUInt w, tpUInt h)
{
    setSize(w,h);
}


void
tpWindowX11::setCaption(const tpString& caption)
{
	XStoreName(dpy,win,caption.c_str());
}

void
tpWindowX11::update()
{

    //if (XEventsQueued(dpy,QueuedAlready)) {
	
    while(XPending(dpy)) {

		XEvent event;
        XNextEvent(dpy,&event);

        tpWindowEvent e(this);

        tpPoint point;

        Window temp_win;
        unsigned int mask;
        XQueryPointer(dpy,win,&temp_win,&temp_win,&point.x,&point.y,&point.x,&point.y,&mask);
        e.setMousePosition(point.x,point.y);
        e.setRenderSurface(this);

        bool submit = false;

		switch (event.type) {
        case DestroyNotify:
        case ClientMessage:
            tpLogNotify("%s - request to close",__FUNCTION__);
            this->setDone();
            submit = true;
            break;
        case ConfigureNotify:
            e.setId(tpWindowEvent::kWindowSize);
            submit = true;
            //tpLogNotify("%s - configure",__FUNCTION__);
			break;
		case KeyPress:
		case KeyRelease:
			{
                KeyCode kc = event.xkey.keycode;
#if TP_USE_OLD_X11
                KeySym ks = XKeycodeToKeysym(dpy, kc, 0);
#else
                KeySym ks = XkbKeycodeToKeysym(dpy, kc, 0, 0);
#endif
                tpString kstr = XKeysymToString(ks);
                tpLogNotify("%s - key pressed %d (%s)",__FUNCTION__,kstr.c_str()[0],kstr.c_str());
                e.setKeyCode(kstr.c_str()[0]);
                e.setKeyState((event.type == KeyPress) ?
                                  tpWindowEvent::kKeyDown :
                                  tpWindowEvent::kKeyUp );
			}
            submit = true;
			break;
		case ButtonPress:
        case ButtonRelease:
            e.setMouseKey(event.xbutton.button);
            e.setMouseState((event.type == ButtonPress) ?
                                tpWindowEvent::kMouseDown :
                                tpWindowEvent::kMouseUp);
            tpLogNotify("%s - button released/pressed (%d)",__FUNCTION__,event.xbutton.button);
            submit = true;
			break;
        case Expose:
        case MapNotify:
        case ReparentNotify:
            // just ignore those
            break;
		default:
			tpLogNotify("%s - got an unknown event %d",__FUNCTION__,event.type);
			break;
		}

        if (submit) {
            if (!this->getEventHandler().process(e))
            {
                //XPutBackEvent(dpy,&event);
            }
            XFlush(dpy);
        }
    }
}

void
tpWindowX11::destroy()
{
    mContext = 0;

	XDestroyWindow(dpy,win);
	XCloseDisplay(dpy);
}

tpWindowX11::~tpWindowX11()
{
	destroy();
}

void
tpWindowX11::setContext(tpRenderContext* context)
{
    if (context == 0)
    {
        context = new tpRenderContextGLX();
    }

    tpRenderTarget::setContext(context);
}




////////////////////////////////////////////////////////////////////////////

class tpWindowFactoryX11 : public tpWindowFactory {
public:

    TP_TYPE_DECLARE

    tpWindowFactoryX11() : tpWindowFactory()
    {
        tpLogNotify("%s X11 RenderSurface",tpGetVersionString());
    }

    tpWindow* create( tpWindowTraits* traits )
    {
        return new tpWindowX11( traits );
    }
};

TP_TYPE_REGISTER(tpWindowX11,tpWindow,WindowX11);
TP_TYPE_REGISTER(tpWindowFactoryX11,tpWindowFactory,WindowFactoryX11);
TP_MODULE_REGISTER(x11surface,tpWindowFactoryX11)


#endif


