#include <tp/vec.h>
#include <tp/logutils.h>
#include <tp/font.h>
#include <tp/module.h>
#include <tp/light.h>

#include <tp/viewer.h>
#include <tp/transform.h>

class tpMyViewer : public tpViewer {
public:
	tpMyViewer() : tpViewer() {}

	void
	onSurfaceEvent(tpRenderSurfaceEvent& e) {
		if (e.getKeyCode() == 27 && e.getKeyState() == tpRenderSurfaceEvent::kKeyUp) {
			e.getRenderSurface()->setDone();
			e.setHandled();
		}
	}
};


int main(int argc,char* argv[])
{
	tpModuleManager::get()->load("jpg,3ds");

	tpRefPtr<tpFont> font = new tpFont();
	font->load(argv[1]);
	font->getImage()->write("font.jpg");
	tpRefPtr<tpPrimitive> prim = new tpPrimitive();
	font->text("Tacit Pixel",*prim);

	tpRefPtr<tpNode> root = new tpNode("Root");

	tpPrimitive* axis = tpPrimitiveFactory::get()->create(tpPrimitiveFactory::kAxis);

	tpTransform* t = new tpTransform;
	t->addChild(prim.get());



	tpMat44r m; m.identity();  m.scale(.01,.01,.01);
	t->setMatrix(m);

	tpRefPtr<tpLight> light = new tpLight();
	light->setPosition(tpVec3f(5,5,5));
	light->setAmbientColor(tpVec4f(0.1f,0.1f,0.1f,1.f));

	root->addChild(light.get());
	root->addChild(axis);
	root->addChild(t);

	tpRefPtr<tpMyViewer> viewer = new tpMyViewer();


	viewer->getScene().getActiveCamera()->addChild(root.get());
	viewer->getScene().getActiveCamera()->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	viewer->getScene().getActiveCamera()->setClearColor(tpVec4f(0,.15f,.3f,1.f));



	viewer->create();
	viewer->run();

	return 0;
}
