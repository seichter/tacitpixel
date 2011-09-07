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

#ifndef TPGLRENDERSURFACEEMBEDDED_H
#define TPGLRENDERSURFACEEMBEDDED_H

#include <tp/rendersurface.h>
#include <tp/log.h>

#include "tpGL.h"

class tpGLRenderSurfaceEmbedded : public tpRenderSurface {
public:

	TP_TYPE_DECLARE;
	
	tpGLRenderSurfaceEmbedded( tpRenderSurfaceTraits* traits );

	tpString getString(tpUInt glenum);

	bool makeCurrent();
	bool swapBuffers();
	void frame();

	virtual tpString getName() const { return tpString("Twisted Pair OpenGL surface 1.0"); }

	tpRenderer* createDefaultRenderer() const;


protected:

	virtual ~tpGLRenderSurfaceEmbedded();

	tpGLRenderSurfaceEmbedded &operator= (const tpGLRenderSurfaceEmbedded &) { return *this; }

	bool show(bool doShow);


private:

	tpString _classname;

};


#endif