#include "rendercontext_eagl.h"


#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/EAGL.h>

#include "tp/rendertarget.h"
#include "tp/module.h"
#include "tp/log.h"


@interface EAGLView : UIView {
	
	EAGLContext* context;
	GLuint frameBuffer, renderBuffer, depthBuffer;	
}

@end

@implementation EAGLView 

+(Class)layerClass {
	return [CAEAGLLayer class];
}

- (void)create {
	
	// should make this configurable
	context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
	
	[EAGLContext setCurrentContext:context];
	
	CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
	
	eaglLayer.opaque = YES;
	eaglLayer.contentsScale = 2.f; //!< for high resolution devices
	eaglLayer.drawableProperties = 
		[NSDictionary dictionaryWithObjectsAndKeys:
		[NSNumber numberWithBool:NO], 
		 kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

		
	glGenFramebuffers(1, &frameBuffer);
    glGenRenderbuffers(1, &renderBuffer);
    
    glBindFramebuffer(GL_FRAMEBUFFER_OES, frameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER_OES, renderBuffer);
	
	//    [eaglcontext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*)self.layer];
	
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:eaglLayer];
	
    glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, renderBuffer);
    
	
	int backingWidth(0), backingHeight(0);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &backingHeight);
	
	tpLogMessage("EAGL View %dx%d",backingWidth,backingHeight);
    
	//    if (USE_DEPTH_BUFFER)
    {
        glGenRenderbuffersOES(1, &depthBuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthBuffer);
        glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, backingWidth, backingHeight);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthBuffer);
    }
    
    if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
    {
        NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        //return NO;
    }
	
}

- (void) destroy {
	
	glDeleteFramebuffers(1, &frameBuffer);
	glDeleteRenderbuffers(1, &renderBuffer);
	glDeleteRenderbuffers(1, &depthBuffer);
	
	frameBuffer = renderBuffer = depthBuffer = 0;
}

- (bool) makeCurrent {

	bool ok = [EAGLContext setCurrentContext:context];
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, frameBuffer);
	
	return ok;
}

-(bool) swapBuffers {
	
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, renderBuffer);
	
	//show the render buffer
	return [context presentRenderbuffer:GL_RENDERBUFFER_OES];

}


@end


tpRenderContextEAGL::tpRenderContextEAGL() 
: tpRenderContext()
, useRenderBuffer(false)
, eaglview(nil)
{
	mName = "UIView/EAGL";
}

bool
tpRenderContextEAGL::init(tpRenderTarget *target)
{
	UIView* view = (UIView*)target->getWindow();
		
	eaglview = [[EAGLView alloc] initWithFrame:[view frame]];
	[eaglview create];
	
	[view addSubview:eaglview];
	
	return true;	
}


bool
tpRenderContextEAGL::makeCurrent()
{
	return [eaglview makeCurrent];
}

bool
tpRenderContextEAGL::swapBuffers()
{
	return [eaglview swapBuffers];
}

void
tpRenderContextEAGL::destroy()
{
	[eaglview destroy];
	[eaglview release];
}


TP_TYPE_REGISTER(tpRenderContextEAGL,tpRenderContext,RenderContextEAGL);
TP_MODULE_REGISTER(RenderContextEAGL,tpRenderContextEAGL);
TP_MODULE_USE(RenderContextEAGL)

