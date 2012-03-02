/*
 * Copyright (C) 1999-2011 Hartmut Seichter
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

#ifndef TP_RENDERTARGET_H
#define TP_RENDERTARGET_H

#include <tp/refptr.h>
#include <tp/referenced.h>
#include <tp/rendercontext.h>

class TP_API tpRenderTarget : public tpReferenced {
public:

    TP_TYPE_DECLARE

    enum {
        kPbuffer = 0,   //!< pixelbuffer
        kPixmap,        //!< pixmap
        kWindow         //!< on-screen window
    };

    /**
      * Default c'tor
      */
    tpRenderTarget();

    /**
      * Get the type of the implementation!
      */
    tpUChar getTargetType() const { return kWindow; }

    /**
      * Returns the window handle. This handle is being
      * used for creating the tpRenderContext
      * @return pointer to handle
      */
    virtual tpRawPtr getWindow() { return 0L; }

    /**
      * Returns the display handle. This handle is beign
      * used for creating the tpRenderContext
      * @return pointer to handle
      */
    virtual tpRawPtr getDisplay() { return 0L; }


    /**
      * Set the current context. If you provide 0L as
      * input, the tpRenderTarget implementation will
      * create a default context.
      *
      * @param context 0 for default or a pointer to a already existing context
      */
    virtual void setContext(tpRenderContext *context);

    /**
      * Return the handle for the render context.
      *
      * @return pointer to context
      */
    virtual tpRenderContext *getContext();

    /**
      * convenience method for checking if the context is already set
      *
      * @return true if the this instance holds already a context
      */
    bool hasContext() const { return mContext.isValid(); }


protected:

    tpRefPtr<tpRenderContext> mContext;

};


#endif
