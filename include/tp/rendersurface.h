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

#ifndef TPRENDERSURFACE_H
#define TPRENDERSURFACE_H

#include <tp/globals.h>
#include <tp/referenced.h>
#include <tp/refptr.h>
#include <tp/string.h>
#include <tp/vec.h>


//////////////////////////////////////////////////////////////////////////

class tpRenderSurface;
class tpRenderSurfaceTraits;

class TP_API tpRenderSurfaceFactory : public tpReferenced {
protected:

	friend class tpRenderSurface;

public:

	TP_TYPE_DECLARE;
	
	virtual tpRenderSurface* create( tpRenderSurfaceTraits* ) = 0;

};

//////////////////////////////////////////////////////////////////////////


class tpCallbackAdapter : public tpReferenced {
public:
	tpCallbackAdapter() {}

protected:
	virtual ~tpCallbackAdapter() {};
};

class tpMouseAdapter : public tpCallbackAdapter {
public:
	
	tpMouseAdapter() {};

	virtual void onMouseMotion( tpInt& x, tpInt& y, tpUByte& button, tpUByte& pressed ) {};
	
	virtual void onMouseWheel( tpInt& x_dir, tpInt& y_dir ) {};

protected:

	virtual ~tpMouseAdapter() {};
};


//////////////////////////////////////////////////////////////////////////

class tpRenderer;
class tpNode;
class tpCamera;

class TP_API tpRenderSurface : public tpReferenced {
public:
	
	TP_TYPE_DECLARE;

	static tpRenderSurface* create( tpRenderSurfaceTraits* traits = 0);

	tpRenderSurface( tpRenderSurfaceTraits* traits );

	virtual bool makeCurrent() = 0;
	virtual bool swapBuffers() = 0; 
	virtual bool show(bool doShow) = 0;

	virtual void frame();

	virtual tpString getName() const;

	virtual tpRenderer* createDefaultRenderer() const;


	virtual void setCaption(const tpString& caption) {};


	void setRenderer(tpRenderer* renderer);

	tpRenderer* getRenderer();

	const tpRenderer* getRenderer() const;


	void setSceneNode(tpNode* node);

	tpNode* getSceneNode();

	const tpNode* getSceneNode() const;


	void setCamera(tpCamera* camera);

	tpCamera* getCamera();

	const tpCamera* getCamera() const;



	void setDone(bool isDone = true);

	bool isDone() const;


	void setMouseAdapter(tpMouseAdapter* mouseadapter);

	tpMouseAdapter* getMouseAdapter();


protected:

	
	tpRefPtr<tpRenderer> m_renderer;
	tpRefPtr<tpNode> m_root;
	tpRefPtr<tpCamera> m_camera;

	bool m_done;

	tpRefPtr<tpMouseAdapter> m_mouseadapter;

	virtual ~tpRenderSurface();
};


//////////////////////////////////////////////////////////////////////////


/**
 * \class tpRenderSurfaceTraits
 * 
 * Initializes a window with predefined traits
 */
class TP_API tpRenderSurfaceTraits {

	tpString title;
	tpVec2i size;
	tpVec2i position;

	tpULong window_flags;

	void* window_handle;
	bool fullscreen;
	
	tpUInt depth;
	bool anti_alias;
	
	

public:

	tpRenderSurfaceTraits();

	tpVec2i getSize() const;
	tpRenderSurfaceTraits& setSize(tpVec2i val);	
	tpRenderSurfaceTraits& setSize(tpInt width, tpInt height);

	tpRenderSurfaceTraits& setDefaultSize();
	//tpRenderSurfaceTraits& setBitPerPixel(tpUInt val);
	//tpRenderSurfaceTraits& setAntiAlias(bool val);

	bool useDefaultSize();
	
	tpVec2i getPosition() const;
	tpRenderSurfaceTraits& setPosition(tpVec2i val);	
	tpRenderSurfaceTraits& setPosition(tpInt width, tpInt height);
	tpRenderSurfaceTraits& setDefaultPosition();
	
	bool useDefaultPosition();

	tpString getTitle() const;
	tpRenderSurfaceTraits& setTitle(const tpString& val);

	void* getWindowHandle() const;
	tpRenderSurfaceTraits& setWindowHandle(void* val);

	bool isFullscreen() const;
	tpRenderSurfaceTraits& setFullscreen(bool val = true);

};


#endif
