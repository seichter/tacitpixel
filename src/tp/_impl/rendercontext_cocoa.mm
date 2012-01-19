#import "rendercontext_cocoa.h"


bool tpRenderContextCocoa::create(tpRenderTarget *target)
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
	oglcontext = [[NSOpenGLContext alloc] initWithFormat:format shareContext:NULL];

	[format release];

	CGLContextObj cgl_context = (CGLContextObj) [oglcontext CGLContextObj];

	CGLSetCurrentContext(cgl_context);

	// @todo implement way to handle this via traits
	tpInt swap_interval = (true) ? 1 : 0;
	CGLSetParameter(cgl_context, kCGLCPSwapInterval, &swap_interval);

	NSWindow* window = (NSWindow*)target->getWindow();

	[oglcontext setView:[window contentView]];

	return true;
}

bool tpRenderContextCocoa::makeCurrent()
{
	[oglcontext makeCurrentContext];
}

bool tpRenderContextCocoa::swapBuffers()
{
	[oglcontext flushBuffer];
}

void tpRenderContextCocoa::destroy()
{
	[oglcontext release];
}

TP_TYPE_REGISTER(tpRenderContextCocoa,tpRenderContext,RenderContextCocoa);
