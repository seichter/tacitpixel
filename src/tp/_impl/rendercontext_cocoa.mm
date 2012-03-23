#import "tp/window.h"
#import "rendercontext_cocoa.h"
#import "tp/module.h"

#import <OpenGL/gl.h>


tpRenderContextCocoa::tpRenderContextCocoa()
	: tpRenderContext()
{
	mName = "Cocoa/CGL";
}

bool tpRenderContextCocoa::init(tpRenderTarget *target)
{
	NSOpenGLPixelFormatAttribute window_attribs[] =
	{
		NSOpenGLPFANoRecovery,
		NSOpenGLPFAAccelerated,
		NSOpenGLPFADepthSize,     (NSOpenGLPixelFormatAttribute)32,
		NSOpenGLPFAColorSize,     (NSOpenGLPixelFormatAttribute)32,
		NSOpenGLPFAAlphaSize,     (NSOpenGLPixelFormatAttribute)8,
		NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1,
		NSOpenGLPFASamples,       (NSOpenGLPixelFormatAttribute)4, // anti alias
		NSOpenGLPFAStencilSize,   (NSOpenGLPixelFormatAttribute)0, // stencil buffer
		NSOpenGLPFADoubleBuffer,
		(NSOpenGLPixelFormatAttribute)nil
	};

	NSOpenGLPixelFormat *format(nil);

	format = [[NSOpenGLPixelFormat alloc] initWithAttributes:window_attribs];

	//CGLCreateContext(CGLPixelFormatObject)
	oglcontext = [[NSOpenGLContext alloc] initWithFormat:format shareContext:NULL];

	[format release];

	cglcontext = (CGLContextObj) [oglcontext CGLContextObj];

	CGLSetCurrentContext(cglcontext);

	// @todo implement way to handle this via traits
	tpInt swap_interval = (true) ? 1 : 0;
	CGLSetParameter(cglcontext, kCGLCPSwapInterval, &swap_interval);

	NSWindow* window = (NSWindow*)target->getWindow();

	if (this->makeCurrent())
	{
		mVersion = this->getString(GL_VERSION);
		mVendor = this->getString(GL_VENDOR);
		mRenderer = this->getString(GL_RENDERER);
		mExtensions = this->getString(GL_EXTENSIONS);

		[oglcontext setView:[window contentView]];

		return true;
	}

	return false;
}

bool
tpRenderContextCocoa::makeCurrent()
{
	CGLError error = CGLLockContext((CGLContextObj)[oglcontext CGLContextObj]);

	if (kCGLNoError == error )
	{
		[oglcontext makeCurrentContext];
		[oglcontext update]; // need this to keep the context in sync with glViewport

		return true;

	} else {

		tpLogError("CGL error %s",CGLErrorString(error));
	}

	return false;
}

bool
tpRenderContextCocoa::swapBuffers()
{
	[oglcontext flushBuffer];

	CGLError error = CGLUnlockContext((CGLContextObj)[oglcontext CGLContextObj]);

	if (kCGLNoError == error) {
		return true;
	} else {
		tpLogError("CGL error %s",CGLErrorString(error));
	}

	return false;
}

void
tpRenderContextCocoa::destroy()
{
	[oglcontext release];
}

tpString
tpRenderContextCocoa::getString(const tpUInt& e)
{
	const tpChar* str = reinterpret_cast<const tpChar*>(glGetString(e));
	return tpString(str);
}

TP_TYPE_REGISTER(tpRenderContextCocoa,tpRenderContext,RenderContextCocoa);
TP_MODULE_REGISTER(RenderContextCocoa,tpRenderContextCocoa)
