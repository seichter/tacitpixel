#ifndef TPRENDERCONTEXTWGL_H
#define TPRENDERCONTEXTWGL_H

#if defined(_WIN32)

#include <tp/rendercontext.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class tpRenderContextWGL : public tpRenderContext {
public:

	TP_TYPE_DECLARE

	tpRenderContextWGL();

	virtual bool create( tpRenderTarget* target );

	virtual void destroy();

	virtual void wait( tpUInt e = kWaitNone );

	virtual bool makeCurrent();

	virtual bool swapBuffers();

	virtual tpString getString( const tpUInt& e );

	HDC _dc;
	HGLRC _context;
};

#endif


#endif