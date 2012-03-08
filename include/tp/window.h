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
#include <tp/image.h>
#include <tp/event.h>

#include <tp/rendercontext.h>
#include <tp/rendertarget.h>

//////////////////////////////////////////////////////////////////////////

class tpWindow;
class tpWindowTraits;
class tpRenderBuffer;

class TP_API tpWindowFactory : public tpReferenced {
protected:
	friend class tpWindow;
public:
	TP_TYPE_DECLARE
	virtual tpWindow* create( tpWindowTraits* ) = 0;
};


class TP_API tpRenderBufferFactory : public tpReferenced {
protected:
	friend class tpRenderBuffer;
public:
	TP_TYPE_DECLARE
	virtual tpRenderBuffer* create( const tpSize& size, tpUInt pixelformat ) = 0;
};

class tpWindowEvent : public tpEvent {

	tpUInt mMouseKey;
	tpUInt mMouseState;
	tpVec2i mMousePosition;

	tpULong mKeyCode;
	tpUInt mKeyState;

	tpWindow* mRenderSurface;

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

	tpWindowEvent(tpWindow* surface)
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

	tpWindow* getRenderSurface() { return mRenderSurface; }
	void setRenderSurface(tpWindow* surface) { mRenderSurface = surface; }

};



//////////////////////////////////////////////////////////////////////////

class TP_API tpRenderBuffer : public tpRenderTarget {
public:

	TP_TYPE_DECLARE

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

class TP_API tpWindow : public tpRenderTarget {
public:

	TP_TYPE_DECLARE

	static tpWindow* create( tpWindowTraits* traits = 0);

	virtual void destroy() = 0;

	virtual bool show(bool doShow) = 0;

	virtual void update() = 0;

	virtual tpString getName() const;

	virtual tpVec2i getSize() const;
	virtual void setSize(tpInt w, tpInt h);

	virtual void setPosition(tpInt x, tpInt y);
	virtual tpVec2i getPosition() const;

	virtual void setCaption(const tpString& ) {}
	virtual tpString getCaption() { return tpString(); }

	bool isValid() const { return !mDone; }

	void setDone(bool done = true) { mDone = done; }

	tpEventHandler& getEventHandler() { return mEventHandler; }


	virtual void setClientAreaSize(tpUInt w, tpUInt h);
	virtual tpVec2i getClientAreaSize() const;


protected:

	tpWindow();
	tpWindow( tpWindowTraits* traits );

	tpEventHandler mEventHandler;

	bool mDone;

	virtual ~tpWindow();
};


//////////////////////////////////////////////////////////////////////////


/**
 * \class tpRenderSurfaceTraits
 *
 * Initializes a window with predefined traits
 */
class TP_API tpWindowTraits {

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

	tpWindowTraits();


	tpWindowTraits& setEmbedded(bool flag) { embedded = true; return *this; }
	bool getEmbedded() const { return embedded; }


	tpWindowTraits& setRenderBackend(tpUInt be) { renderbackend = be; return *this; }
	tpUInt getRenderBackend() const { return renderbackend; }


	tpVec2i getSize() const;
	tpWindowTraits& setSize(tpVec2i val);
	tpWindowTraits& setSize(tpInt width, tpInt height);

	tpWindowTraits& setDefaultSize();
	//tpRenderSurfaceTraits& setBitPerPixel(tpUInt val);
	//tpRenderSurfaceTraits& setAntiAlias(bool val);

	bool useDefaultSize();

	tpVec2i getPosition() const;
	tpWindowTraits& setPosition(tpVec2i val);
	tpWindowTraits& setPosition(tpInt width, tpInt height);
	tpWindowTraits& setDefaultPosition();

	bool useDefaultPosition();

	tpString getTitle() const;
	tpWindowTraits& setTitle(const tpString& val);

	void* getWindowHandle() const;
	tpWindowTraits& setWindowHandle(void* val);

	bool isFullscreen() const;
	tpWindowTraits& setFullscreen(bool val = true);

};


#endif
