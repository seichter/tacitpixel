/*
* Twisted Pair Visualization Engine
*
* Copyright (c) 1999-2009 Hartmut Seichter 
* 
* This library is open source and may be redistributed and/or modified under  
* the terms of the Twisted Pair License (TPL) version 1.0 or (at your option) 
* any later version. The full license text is available in the LICENSE file 
* included with this distribution, and on the technotecture.com website.
*
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
