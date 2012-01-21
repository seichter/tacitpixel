
#include <tp/rendersurface.h>
#include <tp/image.h>

#include <tp/log.h>

#include <X11/Xlib.h>

class tpRenderBufferX11 : public tpRenderTarget {

    Display* dpy;
    Pixmap pixmap;
    tpSize size;
    tpUInt depth;

public:

    tpRenderBufferX11()
        : tpRenderTarget()
        , dpy(0L)
        , pixmap(0)
    {
    }

    void create(const tpSize& size_)
    {
        static tpString s_display;

        // should actually check if we can use "getenv"
        if (s_display.isEmpty()) s_display.set(getenv("DISPLAY"));

        if (s_display.isEmpty()) s_display = ":0.0";

        dpy = XOpenDisplay(s_display.c_str());

        long screen = XDefaultScreen( dpy );

        depth = DefaultDepth(dpy, screen);

        tpLogMessage("%s using display %s",__FUNCTION__,s_display.c_str());

        size = size_;

        pixmap = XCreatePixmap(dpy, InputOnly, size.width, size.height, depth);

    }

    void destroy()
    {
        XFreePixmap(dpy,pixmap);
    }

    void copy(tpImage& image)
    {
        GC gs = DefaultGC(dpy,0);

        XImage* xim = XGetImage(dpy, pixmap, 0, 0, size.width, size.height, AllPlanes, ZPixmap);

        if (xim)
        {
            image.allocate(size.width,size.height);
            image.assign((const void*)(&(xim->data[0])));
            //XDestroyImage(xim);
        }

    }
};
