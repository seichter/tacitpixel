#ifndef TPRENDERCONTEXTGLX_H
#define TPRENDERCONTEXTGLX_H

#include "tp/egl.h"
#include "tp/rendercontext.h"
#include "tp/renderer.h"
#include "tp/log.h"

class TP_API tpRenderContextEGL : public tpRenderContext {
public:

    TP_TYPE_DECLARE

    EGLDisplay egl_display;
    EGLSurface egl_surface;
    EGLContext egl_context;

    bool bind(tpRenderTarget *target);
    void destroy();

    bool makeCurrent();
    bool swapBuffers();

    tpUInt getRendererTraits() const;

    tpUInt getVisualId() const;
};


class TP_API tpRenderTargetEGLPbuffer : public tpRenderTarget {
protected:
    tpVec2i mSize;
public:
    tpRenderTargetEGLPbuffer(int width, int height)
        : tpRenderTarget()
        , mSize(tpVec2i(width,height))
    {
    }

    tpInt getWidth() const { return mSize(0); }
    tpInt getHeight() const { return mSize(1); }

};


#endif
