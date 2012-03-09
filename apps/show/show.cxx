
#include <tp/string.h>
#include <tp/arguments.h>
#include <tp/log.h>
#include <tp/logutils.h>
#include <tp/module.h>
#include <tp/sort.h>
#include <tp/stringtokenizer.h>
#include <tp/node.h>
#include <tp/renderer.h>
#include <tp/window.h>
#include <tp/thread.h>
#include <tp/timer.h>
#include <tp/primitive.h>
#include <tp/vec.h>
#include <tp/light.h>
#include <tp/viewer.h>
#include <tp/transform.h>

class tpViewerShow : public tpViewer {

public:


	virtual void
	onSurfaceEvent(tpWindowEvent& e)
	{

		tpLogMessage("Mouse: %d %d (%d) [%d] Key: %d %d",
					 e.getMousePosition()[0],e.getMousePosition()[1],
					 e.getMouseKey(),e.getMouseState(),
					 e.getKeyCode(),e.getKeyState());
		tpCamera* camera = mScene->getActiveCamera();

		tpLog::get() << camera->getTranslation() << "\n";


		if (e.getKeyCode() == 'q' && e.getKeyState() == tpWindowEvent::kKeyUp) {
			e.getRenderSurface()->setDone();
		}

		if (e.getMouseState() == tpWindowEvent::kMouseDown) {

			tpVec3r delta(0.05,0,0);

			if (e.getMouseKey() == tpWindowEvent::kMouseKeyLeft) {
				camera->setTranslation(camera->getTranslation() + delta);
			} else {
				camera->setTranslation(camera->getTranslation() - delta);
			}
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


	tpRefNode root = (file == "axis")
			? tpPrimitiveFactory::create(tpPrimitiveFactory::kAxis)
			:  tpNode::read(file);

	if (!root.isValid()) {
		tpLogError("Can't load file");
		return -1;
	}


	tpRefPtr<tpTransform> lt = new tpTransform();
	tpRefPtr<tpLight> l = new tpLight();

	lt->addChild(l.get());

	lt->setMatrix(tpMat44Op::translation<tpReal>(0,3,0));

	l->setAmbientColor(tpVec4f(0.1f,0.1f,0.1f,1.f));
	l->setDiffuseColor(tpVec4f(.8f,.8f,.8f,1.f));
	l->setPosition(tpVec4f(0.f,1.f,0.f,0.f));

	root->addChild(lt.get());

	tpRefPtr<tpViewer> viewer = new tpViewerShow;

#if 1
	viewer->getScene().getActiveCamera()->setViewLookAt(tpVec3r(5,5,5),tpVec3r(0,0,0),tpVec3r(0,1,0));
	viewer->getScene().getActiveCamera()->setProjectionPerspective(45,1.3,.1,100);
#else
	viewer->getScene().getActiveCamera()->setViewLookAt(tpVec3r(0,0,1),tpVec3r(0,0,0),tpVec3r(0,1,0));
	viewer->getScene().getActiveCamera()->setProjection(tpMat44r::Identity());
#endif

	viewer->getScene().getActiveCamera()->setClearColor(tpVec4f(.3,.4,.5,1));
	viewer->getScene().getActiveCamera()->setClearFlags(tpCamera::kClearDepth | tpCamera::kClearColor);

	viewer->getScene().getActiveCamera()->addChild(root.get());

	viewer->create();
	viewer->run();

	return 0;
}
