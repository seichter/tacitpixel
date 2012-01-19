#ifndef TP_RENDERCONTEXT_COCOA_H
#define TP_RENDERCONTEXT_COCOA_H

#ifdef __OBJC__
	#import <Cocoa/Cocoa.h>
#else
	struct NSOpenGLContext;
#endif

#include <tp/rendersurface.h>
#include <tp/log.h>


class tpRenderContextCocoa : public tpRenderContext {

	NSOpenGLContext* oglcontext;

public:

	TP_TYPE_DECLARE;

	virtual bool create(tpRenderTarget *target);
	virtual bool makeCurrent();
	virtual bool swapBuffers();
	virtual void destroy();

};

#endif
