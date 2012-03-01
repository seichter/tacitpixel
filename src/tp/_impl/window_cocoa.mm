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

#import "window_cocoa.h"

#if 1//defined(TP_USE_COCOA)

#import "rendercontext_cocoa.h"

#include <ApplicationServices/ApplicationServices.h>

#include <tp/module.h>
#include <tp/version.h>
#include <tp/logutils.h>


@interface tpWindowCocoaDelegate : NSResponder
{
	tpWindowCocoa* rendersurface;
};
- (BOOL)acceptsFirstResponder;
- (BOOL)becomeFirstResponder;
- (BOOL)resignFirstResponder;

-(void) mouseMoved:(NSEvent*)theEvent;
-(void) windowDidMove: (NSNotification*)notification;

@end

@implementation tpWindowCocoaDelegate;

-(id)init
{
	self = [super init];
	return self;
}

-(void)setWindow: (tpWindowCocoa*)arendersurface
{
	[[NSNotificationCenter defaultCenter]
		addObserver:self
		selector:@selector(tryClose:)
		name:NSWindowWillCloseNotification
		object:(NSWindow*)arendersurface->getWindow()];

	[[NSNotificationCenter defaultCenter]
		addObserver:self
		selector:@selector(windowDidMove:)
		name:NSWindowDidMoveNotification
		object:(NSWindow*)arendersurface->getWindow()];

	[[NSNotificationCenter defaultCenter]
		addObserver:self
		selector:@selector(windowDidResize:)
		name:NSWindowDidResizeNotification
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
}

-(void) windowDidMove: (NSNotification*)notification
{
	tpLogMessage("Move!");
}

-(void) windowDidResize: (NSNotification*)notification
{
	tpWindowEvent e(rendersurface);
	e.setId(tpWindowEvent::kWindowSize);
	e.setRenderSurface(rendersurface);
	rendersurface->getEventHandler().process(e);
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


tpWindowCocoa::tpWindowCocoa(tpWindowTraits* traits)
	: tpWindow(traits)
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

	/* set notification interfaces */
	tpWindowCocoaDelegate* mdelegate = [[tpWindowCocoaDelegate alloc] init];
	[mdelegate setWindow:this];

	delegate = mdelegate;
	[window setDelegate:[NSApp delegate]];


	[window center];
	[window setAcceptsMouseMovedEvents:TRUE];
	[window makeKeyAndOrderFront:nil];
	[window isMainWindow];

	/* title */
	if (traits) this->setCaption(traits->getTitle());

	/* now make application active */
	NSRunningApplication* app = [NSRunningApplication currentApplication];
	[app activateWithOptions: NSApplicationActivateAllWindows];

	[NSApp finishLaunching];

	ProcessSerialNumber PSN;
	GetCurrentProcess(&PSN);
	TransformProcessType(&PSN,kProcessTransformToForegroundApplication);

	this->setSize(width,height);

	[pool release];

}


void
tpWindowCocoa::destroy()
{
	//\todo implement!
}


tpWindowCocoa::~tpWindowCocoa()
{
	window = NULL;
}

bool tpWindowCocoa::show(bool doShow)
{
	[window setIsVisible:(BOOL)doShow];
	return [window isVisible];
}

tpVec2i tpWindowCocoa::getSize() const {
	NSRect rect = [window contentRectForFrameRect:[window frame]];
	NSSize size = rect.size;

	tpVec2i s;

	s[0] =  (int)floor(size.width);
	s[1] = (int)floor(size.height);


	tpLogNotify("Size: %dx%d",s[0],s[1]);

	return s;
}

void
tpWindowCocoa::setSize(tpInt w, tpInt h) {
	NSRect rect = [window frame];
	rect.size.width = w;
	rect.size.height = h;
	[window setFrame:rect display:true animate: true];
}

void tpWindowCocoa::update()
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

	NSEvent* event = [NSApp
		nextEventMatchingMask:NSAnyEventMask
		untilDate:nil
		inMode:NSDefaultRunLoopMode
		dequeue:YES];

	if (nil != event)
	{
		NSRect size = [window contentRectForFrameRect:[window frame]];
		NSPoint point = [window mouseLocationOutsideOfEventStream];

		tpWindowEvent e(this);
		e.setMousePosition(point.x,point.y);
		e.setRenderSurface(this);


		bool submit(false);
		switch([(NSEvent *)event type])
		{

		case NSLeftMouseDown:
		{
			e.setMouseState(tpWindowEvent::kMouseDown);
			e.setMouseKey(tpWindowEvent::kMouseKeyLeft);
			submit = true;
			break;
		}
		case NSLeftMouseUp:
		{
			e.setMouseState(tpWindowEvent::kMouseUp);
			e.setMouseKey(tpWindowEvent::kMouseKeyLeft);
			submit = true;
			break;
		}
		case NSRightMouseDown:
		{
			e.setMouseState(tpWindowEvent::kMouseDown);
			e.setMouseKey(tpWindowEvent::kMouseKeyRight);
			submit = true;
			break;
		}
		case NSRightMouseUp:
		{
			e.setMouseState(tpWindowEvent::kMouseUp);
			e.setMouseKey(tpWindowEvent::kMouseKeyRight);
			submit = true;
			break;
		}
		case NSKeyDown:
		{
			NSString* str = [event characters];
			tpString s([str UTF8String]);
			e.setKeyState(tpWindowEvent::kKeyDown);
			e.setKeyCode((unsigned int)s.c_str()[0]);

			submit = true;
			break;
		}
		case NSKeyUp:
		{
			NSString* str = [event characters];
			tpString s([str UTF8String]);
			e.setKeyState(tpWindowEvent::kKeyUp);
			e.setKeyCode((unsigned int)s.c_str()[0]);

			submit = true;
			break;
		}
		case NSMouseMoved:
			submit = true;
			break;
		case NSFlagsChanged:
		default:
			break;
		}

		// we only peek into the events - put it back into the queue
		[NSApp sendEvent:event];

		if (submit) this->getEventHandler().process(e);
	}
	[pool release];

}


void
tpWindowCocoa::setCaption(const tpString& caption)
{
	NSString* name = [[NSString alloc] initWithCString: caption.c_str() encoding: NSASCIIStringEncoding ];
	[window setTitle:name];
	[name release];
}

void
tpWindowCocoa::setContext(tpRenderContext* context)
{
	if (context == 0)
	{
		context = new tpRenderContextCocoa();
	}

	tpRenderTarget::setContext(context);
}


////////////////////////////////////////////////////////////////////////////

class tpWindowCocoaFactory : public tpWindowFactory {
public:

	TP_TYPE_DECLARE

	tpWindowCocoaFactory() : tpWindowFactory()
	{
		tpLogNotify("%s Cocoa rendering surface",tpGetVersionString());
	}
protected:
	tpWindow* create( tpWindowTraits* traits )
	{
		return new tpWindowCocoa( traits );
	}
};

////////////////////////////////////////////////////////////////////////////


TP_TYPE_REGISTER(tpWindowCocoaFactory,tpWindowFactory,WindowCocoaFactory);
TP_TYPE_REGISTER(tpWindowCocoa,tpWindow,WindowCocoa);
TP_MODULE_REGISTER(cocoasurface,tpWindowCocoaFactory)

#endif
