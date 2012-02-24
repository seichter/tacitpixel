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
#include <tp/rendercontext.h>
#include <tp/image.h>
#include <tp/event.h>


//////////////////////////////////////////////////////////////////////////

class tpRenderBuffer;
class tpRenderSurface;
class tpRenderSurfaceTraits;

class TP_API tpRenderSurfaceFactory : public tpReferenced {
protected:
	friend class tpRenderSurface;
public:
	TP_TYPE_DECLARE;
	virtual tpRenderSurface* create( tpRenderSurfaceTraits* ) = 0;
};


class TP_API tpRenderBufferFactory : public tpReferenced {
protected:
	friend class tpRenderBuffer;
public:
	TP_TYPE_DECLARE;
	virtual tpRenderBuffer* create( const tpSize& size, tpUInt pixelformat ) = 0;
};

//////////////////////////////////////////////////////////////////////////

//class tpRenderSurfaceCallback : public tpReferenced {
//public:

//	enum {
//		kMouseNone,
//		kMouseMove,
//		kMouseUp,
//		kMouseDown
//	};

//	virtual bool onMouseMotion( const tpInt& h, const tpInt& v) { return false; }

//	virtual bool onMouseClick( const tpInt& h, const tpInt& v, const tpUShort& state ) { return false; }

//};


class tpRenderSurfaceEvent : public tpEvent {

	tpUInt mMouseKey;
	tpUInt mMouseState;
	tpVec2i mMousePosition;

	tpULong mKeyCode;
	tpUInt mKeyState;

	tpRenderSurface* mRenderSurface;

public:

	enum {
		kWindowSize = 0x4000
	};

	enum {
		kMouseNone = 0,
		kMouseMove,
		kMouseUp,
		kMouseDown
	};

	enum {
		kMouseKeyNone = 0,
		kMouseKeyLeft,
		kMouseKeyMiddle,
		kMouseKeyRight
	};

	enum {
		kKeyNone = 0,
		kKeyUp,
		kKeyDown
	};

	tpRenderSurfaceEvent(tpRenderSurface* surface)
		: mRenderSurface(surface)
		, mMouseKey(0)
		, mMouseState(0)
		, mMousePosition(tpVec2i(-1,-1))
		, mKeyCode(0)
		, mKeyState(0)
	{}

	tpUInt getMouseKey() const { return mMouseKey; }
	void setMouseKey(tpUInt key) { mMouseKey = key; }

	tpUInt getMouseState() const { return mMouseState; }
	void setMouseState(tpUInt state) { mMouseState = state; }

	tpVec2i getMousePosition() const { return mMousePosition; }
	void setMousePosition(int v1, int v2) { mMousePosition = tpVec2i(v1,v2); }

	tpULong getKeyCode() const { return mKeyCode; }
	void setKeyCode(tpULong code) { mKeyCode = code; }

	tpUInt getKeyState() const { return mKeyState; }
	void setKeyState(tpUInt state) { mKeyState = state; }

	tpRenderSurface* getRenderSurface() { return mRenderSurface; }
	void setRenderSurface(tpRenderSurface* surface) { mRenderSurface = surface; }

};

//////////////////////////////////////////////////////////////////////////

class TP_API tpRenderTarget : public tpReferenced {
public:

	TP_TYPE_DECLARE;

	enum {
		kPbuffer = 0,
		kPixmap,
		kWindow
	};

	tpRenderTarget();
	tpUChar getTargetType() const { return kWindow; }

	virtual tpRawPtr getWindow() { return 0L; }
	virtual tpRawPtr getDisplay() { return 0L; }


	virtual void setContext(tpRenderContext *context);

	virtual tpRenderContext *getContext();

	bool hasContext() const { return mContext.isValid(); }

//	virtual tpInt getWidth() const { return 0; }
//	virtual tpInt getHeight() const { return 0; }

protected:

	tpRefPtr<tpRenderContext> mContext;

};

//////////////////////////////////////////////////////////////////////////

class TP_API tpRenderBuffer : public tpRenderTarget {
public:

	TP_TYPE_DECLARE;

	static tpRenderBuffer* create(const tpSize& size, const tpUInt pixelformat);

	virtual void destroy() = 0;
	virtual void copy(tpImage& image) = 0;

	virtual tpRawPtr getDisplay() = 0;
	virtual tpRawPtr getBuffer() = 0;

protected:

	tpRenderBuffer();
	virtual ~tpRenderBuffer();
};



//////////////////////////////////////////////////////////////////////////

class TP_API tpRenderSurface : public tpRenderTarget {
public:

	TP_TYPE_DECLARE

	static tpRenderSurface* create( tpRenderSurfaceTraits* traits = 0);

	virtual void destroy() = 0;

	virtual bool show(bool doShow) = 0;

	virtual void update() = 0;

	virtual tpString getName() const;

	virtual tpVec2i getSize() const;

	virtual void setCaption(const tpString& ) {}
	virtual tpString getCaption() { return tpString(); }

	bool isValid() const { return !mDone; }

	void setDone(bool done = true) { mDone = done; }

	tpEventHandler& getEventHandler() { return mEventHandler; }




protected:

	tpRenderSurface();
	tpRenderSurface( tpRenderSurfaceTraits* traits );

	tpEventHandler mEventHandler;

	bool mDone;

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


	tpUInt renderbackend;
	bool embedded;

public:

	enum {
		kRenderDefault = 0x0,
		kRenderOpenGL,
		kRenderOpenGLES1,
		kRenderOpenGLES2
	};

	tpRenderSurfaceTraits();


	tpRenderSurfaceTraits& setEmbedded(bool flag) { embedded = true; return *this; }
	bool getEmbedded() const { return embedded; }


	tpRenderSurfaceTraits& setRenderBackend(tpUInt be) { renderbackend = be; return *this; }
	tpUInt getRenderBackend() const { return renderbackend; }


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
