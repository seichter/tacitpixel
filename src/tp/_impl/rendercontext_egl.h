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
#ifndef TPRENDERCONTEXTEGL_H
#define TPRENDERCONTEXTEGL_H

#include "tp/egl.h"
#include "tp/rendercontext.h"
#include "tp/renderer.h"
#include "tp/log.h"

class TP_API tpRenderContextEGL : public tpRenderContext {
public:

    TP_TYPE_DECLARE

    tpRenderContextEGL();

    bool bind(tpRenderTarget *target);
    void destroy();

    bool makeCurrent();
    bool swapBuffers();

    tpUInt getRendererTraits() const;

    tpUInt getVisualId() const;

protected:

    EGLDisplay egl_display;
    EGLSurface egl_surface;
    EGLContext egl_context;

    tpUInt mVisualId;
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
