
#include <tp/string.h>
#include <tp/arguments.h>
#include <tp/log.h>
#include <tp/module.h>
#include <tp/sort.h>
#include <tp/stringtokenizer.h>
#include <tp/node.h>
#include <tp/renderer.h>
#include <tp/rendersurface.h>
#include <tp/thread.h>
#include <tp/timer.h>

struct tpSurfaceHandler {

	tpRefPtr<tpCamera> camera;
	tpVec3r campos;

	tpSurfaceHandler(tpCamera* camera_)
		: camera(camera_)
		, campos(tpVec3r(2,2,2))
	{}

	void onRenderSurface(tpRenderSurfaceEvent& e) {

//		tpLogMessage("Mouse: %d %d (%d) [%d] Key: %d %d",
//					 e.getMousePosition()[0],e.getMousePosition()[1],
//					 e.getMouseKey(),e.getMouseState(),
//					 e.getKeyCode(),e.getKeyState());

		if (e.getKeyCode() == 27 && e.getKeyState() == tpRenderSurfaceEvent::kKeyUp) {
			e.getRenderSurface()->setDone();
		}

		if (e.getMouseState() == tpRenderSurfaceEvent::kMouseDown) {
			campos[0] += 0.5;
			camera->setViewLookAt(campos,tpVec3r(0,0,0),tpVec3r(0,1,0));
		}

		e.setHandled(true);
	}
};

int main(int argc,char* argv[])
{
	tpModuleManager::get()->load("obj,3ds,jpeg");

	tpString file,plugins;
	tpArguments args(&argc,argv);

	if (args.get("--plugins",plugins)) {}
	if (args.get("--file",file)) {}

	tpRefNode root = tpNode::read(file);

	if (!root.isValid()) {
		tpLogError("Can't load file");
		return -1;
	}

	tpRenderSurfaceTraits traits;
	traits.setSize(640,480).setPosition(10,10).setTitle("Tacit Pixel 3");

	tpRefPtr<tpRenderer> renderer = tpRenderer::create();
	tpRefPtr<tpRenderSurface> rendersurface = tpRenderSurface::create(&traits);
	rendersurface->setContext(0);

	if (!renderer.isValid() || !rendersurface.isValid() || !rendersurface->hasContext()) return -1;

	tpRefPtr<tpCamera> camera = renderer->getActiveCamera();

	camera->setProjectionPerspective(60.0f,1.3f,0.1f,1000.0f);
	camera->setViewLookAt(tpVec3r(2,2,2),tpVec3r(0,0,0),tpVec3r(0,1,0));
	camera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);

	camera->setClearColor(tpVec4f(0.5f,0.5f,0.9f,1.0f));
	camera->setViewport(tpVec4i(0,0,640,480));

	tpUInt frames = 0;
	tpDouble time = 0;

	tpTimer timer;

	if (rendersurface.isValid() && renderer.isValid())
	{
		tpSurfaceHandler handler(camera.get());

		rendersurface->getEventHandler().attach<tpRenderSurfaceEvent,tpSurfaceHandler>(&handler,&tpSurfaceHandler::onRenderSurface);

		rendersurface->show(true);


		while (rendersurface->isValid())
		{
			timer.start();

			if (rendersurface->getContext()->makeCurrent())
			{
				(*renderer)(root.get());

				rendersurface->getContext()->swapBuffers();
				frames++;
			}

			time += timer.getElapsed(tpTimer::kTimeSeconds);

			if (time > 5) {
				tpLogNotify("fps %3.3f",(float)frames/time);
				time = 0;
				frames = 0;
			}

			rendersurface->update();

			tpThread::yield();
		}
	}

	return 0;
}
