
#include <tp/string.h>
#include <tp/arguments.h>
#include <tp/log.h>
#include <tp/logutils.h>
#include <tp/module.h>
#include <tp/sort.h>
#include <tp/stringtokenizer.h>
#include <tp/node.h>
#include <tp/renderer.h>
#include <tp/rendersurface.h>
#include <tp/thread.h>
#include <tp/timer.h>
#include <tp/primitive.h>
#include <tp/vec.h>
#include <tp/light.h>
#include <tp/viewer.h>

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

			if (e.getMouseKey() == tpRenderSurfaceEvent::kMouseKeyLeft) {
				campos[0] += 0.5;
			} else {
				campos[0] -= 0.5;
			}

			tpLogNotify("Camera pos: %3.1f %3.1f %3.1f",campos[0],campos[2],campos[2]);

			camera->setViewLookAt(campos,tpVec3r(0,0,0),tpVec3r(0,1,0));
		}

		e.setHandled(true);
	}
};

tpNode*
createAxis()
{
	tpPrimitive* axis = new tpPrimitive(tpPrimitive::kLines,
										tpPrimitive::kAttributeVertex | tpPrimitive::kAttributeColors);


	axis->addVertex(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec2r(0,1),tpVec4r(1,1,1,1));
	axis->addVertex(tpVec3r(1,0,0),tpVec3r(0,0,1),tpVec2r(0,1),tpVec4r(1,0,0,1));

	axis->addVertex(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec2r(0,1),tpVec4r(1,1,1,1));
	axis->addVertex(tpVec3r(0,1,0),tpVec3r(0,0,1),tpVec2r(0,1),tpVec4r(0,1,0,1));

	axis->addVertex(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec2r(0,1),tpVec4r(1,1,1,1));
	axis->addVertex(tpVec3r(0,0,1),tpVec3r(0,0,1),tpVec2r(0,1),tpVec4r(0,0,1,1));

	return axis;

}

int main(int argc,char* argv[])
{
	tpModuleManager::get()->load("obj,3ds,jpeg");

	tpString file,plugins;
	tpArguments args(&argc,argv);

	if (args.get("--plugins",plugins)) {}
	if (args.get("--file",file)) {}


	tpRefNode root = (file == "axis") ? createAxis() :  tpNode::read(file);

	if (!root.isValid()) {
		tpLogError("Can't load file");
		return -1;
	}


	tpRefPtr<tpLight> l = new tpLight();
	l->setPosition(tpVec3f(5,5,5));
	l->setAmbientColor(tpVec4f(0.1f,0.1f,0.1f,1.f));

	root->addChild(l.get());


    tpRefPtr<tpViewer> viewer = new tpViewer();
    viewer->create();
    viewer->setScene(root.get());
    viewer->run();

	return 0;
}
