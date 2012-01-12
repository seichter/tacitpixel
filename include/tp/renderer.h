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

#ifndef TP_RENDERER_H
#define TP_RENDERER_H


#include <tp/node.h>
#include <tp/camera.h>

class tpCamera;
class tpRendererTraits;
class tpRenderContext;


struct tpRendererTraits {

	bool isFixedFunction;

	tpRendererTraits() : isFixedFunction(true) {}

	bool operator == (const tpRendererTraits& rhs) const {
		return isFixedFunction == rhs.isFixedFunction;
	}
};

typedef tpArray<tpRefPtr<tpCamera> > tpRefCameraArray;


class TP_API tpRenderer : public tpReferenced {
public:

	TP_TYPE_DECLARE;

	tpRenderer();

	tpRenderer(const tpRenderer& r);

	tpRenderer& operator = (const tpRenderer& rhs);

	virtual void operator()(tpNode* node) = 0;

	virtual const tpRendererTraits& getTraits() const = 0;

	static tpRenderer* create(const tpRendererTraits& traits = tpRendererTraits());

	tpCamera* getActiveCamera();

	void setActiveCamera(tpUInt camera);

	void addCamera(tpCamera* camera, bool makeActive = true);

	void removeCamera(tpCamera *camera);

protected:

	tpRefCameraArray mCameras;
	tpUInt mActiveCamera;

	virtual ~tpRenderer();
};




#endif
