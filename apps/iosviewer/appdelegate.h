
#import <UIKit/UIKit.h>

#include "tp/rendercontext.h"
#include "tp/renderer.h"
#include "tp/scene.h"
#include "tp/refptr.h"

@interface TacitAppDelegate : UIResponder <UIApplicationDelegate>
{
	UIWindow* uiwindow;
	tpRenderContext* ctx;
	tpRenderTarget* tgt;
	tpRenderer* renderer;
	tpRefPtr<tpScene> scene;
	
	NSTimer* timer;
	
@private
}

@property (readwrite, retain) UIWindow *uiwindow;

@end