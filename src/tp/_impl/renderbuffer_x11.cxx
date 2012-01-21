
#include <tp/rendersurface.h>
#include <tp/image.h>

#include <tp/log.h>
#include <tp/module.h>

#include <X11/Xlib.h>

class tpRenderBufferX11 : public tpRenderBuffer {

    Display* dpy;
    Pixmap pixmap;
    tpSize size;
    tpUInt depth;

public:

    TP_TYPE_DECLARE

    tpRenderBufferX11(const tpSize& size_,tpUInt pf)
        : tpRenderBuffer()
        , dpy(0L)
        , pixmap(0)
        , size(size_)
        , depth(pf)
    {
        static tpString s_display;

        // should actually check if we can use "getenv"
        if (s_display.isEmpty()) s_display.set(getenv("DISPLAY"));

        if (s_display.isEmpty()) s_display = ":0.0";

        dpy = XOpenDisplay(s_display.c_str());

        long screen = XDefaultScreen( dpy );

        depth = DefaultDepth(dpy, screen);

        pixmap = XCreatePixmap(dpy, InputOnly, size.width, size.height, depth);

        tpLogMessage("%s using display %s created pixmap on %d",__FUNCTION__,s_display.c_str(),pixmap);

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

    tpRawPtr getDisplay()
    {
        return dpy;
    }

    tpRawPtr getBuffer()
    {
        return reinterpret_cast<tpRawPtr>(pixmap);
    }
};

class tpRenderBufferFactoryX11 : public tpRenderBufferFactory {

    TP_TYPE_DECLARE

    tpRenderBuffer* create(const tpSize &size, tpUInt pixelformat)
    {
        return new tpRenderBufferX11(size,pixelformat);
    }
};


TP_TYPE_REGISTER(tpRenderBufferX11,tpRenderBuffer,RenderBufferX11);
TP_TYPE_REGISTER(tpRenderBufferFactoryX11,tpRenderBufferFactory,RenderBufferFactoryX11);
TP_MODULE_REGISTER(x11buffer,tpRenderBufferFactoryX11);
