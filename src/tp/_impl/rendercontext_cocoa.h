#ifndef TP_RENDERCONTEXT_COCOA_H
#define TP_RENDERCONTEXT_COCOA_H

#ifdef __OBJC__
	#import <Cocoa/Cocoa.h>
#else
	struct NSOpenGLContext;
#endif

#include <tp/rendercontext.h>
#include <tp/log.h>


class tpRenderContextCocoa : public tpRenderContext {

	NSOpenGLContext* oglcontext;
	CGLContextObj cglcontext;

public:

	TP_TYPE_DECLARE

	tpRenderContextCocoa();

	virtual bool bind(tpRenderTarget *target);
	virtual bool makeCurrent();
	virtual bool swapBuffers();
	virtual void destroy();

	tpString getString(const tpUInt &e);


	tpUInt getRendererTraits() const;

};

#endif
