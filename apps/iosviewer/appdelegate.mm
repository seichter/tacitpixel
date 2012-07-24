
#import "appdelegate.h"

#include <tp/rendertarget.h>
#include <tp/window.h>
#include <tp/log.h>
#include <tp/module.h>
#include "tp/primitive.h"
#include "tp/light.h"
#include "tp/system.h"

class tpRenderTargetUIView : public tpRenderTarget {

	UIView* uiview; 

public:	
	
	tpRenderTargetUIView(UIView* uiview_) 
		: tpRenderTarget()
		, uiview(uiview_)
	{}
	
	tpRawPtr getWindow() { return uiview; }
	
	
};

@implementation TacitAppDelegate

@synthesize uiwindow;

- (id) init
{
	self = [super init];
	if (self) 
	{
	}
	
	return self;
}

- (void)dealloc
{
    [uiwindow release];
    [super dealloc];
}


extern "C" void tpModule_RenderContextEAGL();
extern "C" void tpModule_RendererGL1x();

-(void) applicationDidFinishLaunching:(UIApplication *)application
{
	
	tpModule_RenderContextEAGL();
	tpModule_RendererGL1x();
	
	self.uiwindow = [[UIWindow alloc] initWithFrame:[UIScreen mainScreen].bounds];
	UIView* view  = [[UIView alloc] initWithFrame:self.uiwindow.bounds];
	
	[self.uiwindow addSubview:view];
	
	tgt = new tpRenderTargetUIView(view);
	
	ctx = tpRenderContext::create("EAGL");
	
	if (ctx)
	{
		ctx->init(tgt);
	} else {
		tpLogError("Could not get EAGL context");
	}
	
	renderer = tpRenderer::create(tpRenderer::kOpenGLES1);
		
	tpLogMessage("Resource %s",tpSystem::get()->getResourcePath().c_str());
	tpLogMessage("Executable %s",tpSystem::get()->getExecutablePath().c_str());
	
	scene = new tpScene;
	
	tpRefPtr<tpCamera> camera = scene->getCamera();
	camera->setName("Perspective");
	
	camera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	camera->setClearColor(tpVec4f(0,.15f,.3f,1.f));
	
	camera->setViewport(tpVec4i(0,0,640,960));

	camera->setProjectionPerspective(33,(float)640/960,0.1,100);
	camera->setViewLookAt(tpVec3r(2,2,2),tpVec3r(0,0,0),tpVec3r(0,1,0));
	
	tpPrimitive* prim = tpPrimitiveFactory::create(tpPrimitiveFactory::kSphere);
	prim->addRenderFlag(tpRenderFlag::kColorMaterial);
	
	tpRefPtr<tpLight> light = new tpLight();
	light->setAmbientColor(tpVec4f(0.1f,0.1f,0.1f,1.f));
	
	prim->setUniformColor(tpVec4f(.1,1,1,1));
	
	
	camera->addChild(light.get());
	camera->addChild(prim);
	
	tpLogMessage("Renderer 0x%x",(tpUInt)renderer);
	
	timer = [NSTimer scheduledTimerWithTimeInterval:0.01
											 target:self
										   selector:@selector(updateMethod:)
										   userInfo:NULL
											repeats:YES];
	
	[self.uiwindow layoutSubviews];
	[self.uiwindow makeKeyAndVisible];

//	UIViewController *testViewController = [[UIViewController alloc] init];
//	[testViewController setView:[[Director sharedDirector] ]];
//	[testViewController presentModalViewController:testView animated:YES];

	
	tpLogMessage("Application launch finished");	
}

-(void) updateMethod: (NSTimer*)theTimer
{

	if (ctx->makeCurrent()) {
		
		//(*renderer)(scene.get());
		
		renderer->render(scene.get());
		
		if (!ctx->swapBuffers())
		{
			tpLogMessage("Error swap buffers");
		}

	}
}

@end