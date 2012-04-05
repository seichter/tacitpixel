
#include <tp/window.h>
#include <tp/image.h>

#include <tp/log.h>
#include <tp/module.h>
#include <tp/version.h>

#include <X11/Xlib.h>
#include <GL/glx.h>

class tpRenderBufferGLX : public tpRenderBuffer {

    Display* dpy;
    tpRefPtr<tpImage> image;

public:

    TP_TYPE_DECLARE

    tpRenderBufferGLX(const tpSize& size_,tpUInt pf)
        : tpRenderBuffer()
        , dpy(0L)
    {
        static tpString s_display;

        // should actually check if we can use "getenv"
        if (s_display.isEmpty()) s_display.set(getenv("DISPLAY"));

        if (s_display.isEmpty()) s_display = ":0.0";

        dpy = XOpenDisplay(s_display.c_str());

        int fbcount(0);

        tpArray<int> attribs;
        attribs.add(GLX_X_RENDERABLE).add(True)
                .add(GLX_DRAWABLE_TYPE).add(GLX_WINDOW_BIT)
                .add(GLX_RENDER_TYPE).add(GLX_RGBA_BIT)
                .add(GLX_X_VISUAL_TYPE).add(GLX_TRUE_COLOR)
                .add(GLX_DOUBLEBUFFER).add(True)
                .add(None);

//        static int visual_attribs[] =
//           {
//             GLX_X_RENDERABLE    , True,
//             GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
//             GLX_RENDER_TYPE     , GLX_RGBA_BIT,
//             GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
//             GLX_RED_SIZE        , 8,
//             GLX_GREEN_SIZE      , 8,
//             GLX_BLUE_SIZE       , 8,
//             GLX_ALPHA_SIZE      , 8,
//             GLX_DEPTH_SIZE      , 24,
//             GLX_STENCIL_SIZE    , 8,
//             GLX_DOUBLEBUFFER    , True,
//             //GLX_SAMPLE_BUFFERS  , 1,
//             //GLX_SAMPLES         , 4,
//             None
//           };

        GLXFBConfig *fbc = glXChooseFBConfig(dpy,DefaultScreen(dpy),
                                             attribs.getData(),&fbcount);
        \
        if(!fbcount) {
            tpLogError("No matching visual found");
        } else {

            int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

            int i;
            for ( i = 0; i < fbcount; i++ )
            {
              XVisualInfo *vi = glXGetVisualFromFBConfig( dpy, fbc[i] );
              if ( vi )
              {
                int samp_buf, samples;
                glXGetFBConfigAttrib( dpy, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
                glXGetFBConfigAttrib( dpy, fbc[i], GLX_SAMPLES       , &samples  );

//                tpLogNotify("  Matching fbconfig %d, visual ID 0x%2x: SAMPLE_BUFFERS = %d,"
//                        " SAMPLES = %d\n",
//                        i, vi -> visualid, samp_buf, samples );

                if ( best_fbc < 0 || samp_buf && samples > best_num_samp )
                  best_fbc = i, best_num_samp = samples;
                if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
                  worst_fbc = i, worst_num_samp = samples;
              }
              XFree( vi );
            }

            GLXFBConfig bestFbc = fbc[ best_fbc ];
        }



    }

    void destroy()
    {
    }

    void copy(tpImage& image)
    {
    }

    tpRawPtr getDisplay()
    {
        return dpy;
    }

    tpRawPtr getBuffer()
    {
        return 0;
        //return reinterpret_cast<tpRawPtr>(pixmap);
    }
};

class tpRenderBufferFactoryGLX : public tpRenderBufferFactory {

    TP_TYPE_DECLARE

public:

    tpRenderBufferFactoryGLX() : tpRenderBufferFactory()
    {
        tpLogNotify("%s GLX Framebuffer",tpGetVersionString());
    }

    tpRenderBuffer* create(const tpSize &size, tpUInt pixelformat)
    {
        return new tpRenderBufferGLX(size,pixelformat);
    }
};


TP_TYPE_REGISTER(tpRenderBufferGLX,tpRenderBuffer,RenderBufferX11);
TP_TYPE_REGISTER(tpRenderBufferFactoryGLX,tpRenderBufferFactory,RenderBufferFactoryX11);
TP_MODULE_REGISTER(x11buffer,tpRenderBufferFactoryGLX);
