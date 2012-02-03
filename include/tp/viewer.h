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

#ifndef TPVIEWER_H
#define TPVIEWER_H

#include <tp/referenced.h>
#include <tp/refptr.h>
#include <tp/rendersurface.h>
#include <tp/renderer.h>
#include <tp/camera.h>

class tpViewer : public tpReferenced {

	tpRefPtr<tpRenderSurface> mSurface;
	tpRefPtr<tpRenderer> mRenderer;
	tpRefPtr<tpCamera> mCamera;
	tpRefPtr<tpNode> mScene;

public:

	tpViewer();

	bool create(const tpString &title = "tpViewer", tpUInt w = 640, tpUInt h = 480, tpUInt x = 0, tpUInt y = 0, tpUInt flags = 0);

	void frame();

	void run();

	virtual void onSurfaceEvent(tpRenderSurfaceEvent &e);

	void setScene(tpNode *scene);
	tpNode *getScene();

};

#endif
