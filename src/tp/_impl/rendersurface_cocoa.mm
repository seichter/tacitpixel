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

#if defined(TP_USE_COCOA)

#include <tp/module.h>
#include <tp/version.h>

#import "rendersurface_cocoa.h"


@interface tpGLRenderSurfaceCocoaDelegate : NSResponder
{
	tpRenderSurfaceCocoa* rendersurface;
};
- (BOOL)acceptsFirstResponder;
- (BOOL)becomeFirstResponder;
- (BOOL)resignFirstResponder;

-(void) mouseMoved:(NSEvent*)theEvent;

@end

@implementation tpGLRenderSurfaceCocoaDelegate;

-(id)init
{
	self = [super init];
	return self;
}

-(void)setRenderSurface: (tpRenderSurfaceCocoa*)arendersurface
{
	[[NSNotificationCenter defaultCenter]
		addObserver:self
		selector:@selector(tryClose:)
		name:NSWindowWillCloseNotification
		object:(NSWindow*)arendersurface->getWindow()];

	rendersurface = arendersurface;
}

-(void)tryClose:(NSNotification*)notification
{
	[[NSNotificationCenter defaultCenter]
		removeObserver:self
		name:NSWindowWillCloseNotification
		object:(NSWindow*)rendersurface->getWindow()];

	if (rendersurface) rendersurface->setDone(true);

}

-(void) mouseMoved:(NSEvent*)theEvent
{
	tpLogNotify("Moved!");
}

- (BOOL)acceptsFirstResponder
{
	return YES;
}

- (BOOL)becomeFirstResponder
{
	return YES;
}

- (BOOL)resignFirstResponder
{
	return YES;
}

@end


tpRenderSurfaceCocoa::tpRenderSurfaceCocoa(tpRenderSurfaceTraits* traits)
	: tpRenderSurface(traits)
	, window(NULL)
	, delegate(NULL)
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	[NSApplication sharedApplication];

	int width = (traits) ? traits->getSize()[0] : 640;
	int height = (traits) ? traits->getSize()[1] : 480;

	tpLogNotify("%s creating window %dx%d",__FUNCTION__,width,height);

	window = [[NSWindow alloc]
			  initWithContentRect: NSMakeRect(0,0,width,height)
			  styleMask:NSTitledWindowMask+NSClosableWindowMask+NSResizableWindowMask
			  backing:NSBackingStoreBuffered
			  defer:false];

	[window center];
	//[window setDelegate:[NSApp delegate]];
	[window setAcceptsMouseMovedEvents:YES];
	[window makeKeyAndOrderFront:nil];
	[window isMainWindow];

	/* set notification interfaces */
	tpGLRenderSurfaceCocoaDelegate* mdelegate = [[tpGLRenderSurfaceCocoaDelegate alloc] init];
	[mdelegate setRenderSurface:this];
	delegate = mdelegate;


	/* title */
	if (traits) this->setCaption(traits->getTitle());

	/* now make application active */
	NSRunningApplication* app = [NSRunningApplication currentApplication];
	[app activateWithOptions: NSApplicationActivateAllWindows];

	[NSApp finishLaunching];

	[pool release];

}


tpRenderSurfaceCocoa::~tpRenderSurfaceCocoa()
{
	window = NULL;
}

//void
//tpRenderSurfaceCocoa::frame()
//{

//}

//bool
//tpRenderSurfaceCocoa::makeCurrent()
//{
//	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

//	NSEvent* event = [NSApp
//		nextEventMatchingMask:NSAnyEventMask
//		untilDate:nil
//		inMode:NSDefaultRunLoopMode
//		dequeue:YES];

//	if (event)
//	{
//		[NSApp sendEvent:event];
//	}

//	[pool release];

//	return true;
//}


//tpString tpRenderSurfaceCocoa::getString(tpUInt glenum)
//{
//	makeCurrent();
//	tpString result;
//	//result.set(reinterpret_cast<const tpChar*>(tpGL::GetString(glenum)));
//	return result;
//}

bool tpRenderSurfaceCocoa::show(bool doShow)
{
	[window setIsVisible:(BOOL)doShow];
	return [window isVisible];
}


void
tpRenderSurfaceCocoa::setCaption(const tpString& caption)
{
	NSString* name = [[NSString alloc] initWithCString: caption.c_str() encoding: NSASCIIStringEncoding ];
	[window setTitle:name];
	[name release];
}


////////////////////////////////////////////////////////////////////////////

//class tpRenderSurfaceFactoryCocoa : public tpRenderSurfaceFactory {
//public:

//	TP_TYPE_DECLARE;

//	tpRenderSurfaceFactoryCocoa() : tpRenderSurfaceFactory()
//	{
//		tpLogNotify("%s Cocoa rendering surface",tpGetVersionString());
//	}
//protected:
//	tpRenderSurface* create( tpRenderSurfaceTraits* traits )
//	{
//		return new tpGLRenderSurfaceCocoa( traits );
//	}
//};

////////////////////////////////////////////////////////////////////////////


//TP_TYPE_REGISTER(tpRenderSurfaceFactoryCocoa,tpRenderSurfaceFactory,RenderSurfaceFactoryCocoa);
//TP_TYPE_REGISTER(tpGLRenderSurfaceCocoa,tpRenderSurface,GLRenderSurfaceCocoa);

//TP_MODULE_REGISTER(cocoasurface,tpRenderSurfaceFactoryCocoa);

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
TP_TYPE_REGISTER(tpRenderSurfaceCocoa,tpRenderSurface,RenderSurfaceCocoa);

#endif
