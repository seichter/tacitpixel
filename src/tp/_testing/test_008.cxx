#include <tp/vec.h>
#include <tp/logutils.h>
#include <tp/font.h>
#include <tp/module.h>

#include <tp/viewer.h>

class tpMyViewer : public tpViewer {
public:
	tpMyViewer() : tpViewer() {}

	void
	onSurfaceEvent(tpRenderSurfaceEvent& e) {
		if (e.getKeyCode() == 27 && e.getKeyState() == tpRenderSurfaceEvent::kKeyUp) {
			e.getRenderSurface()->setDone();
			e.setHandled();
		}
		tpLogMessage("E!");
	}
};


int main(int argc,char* argv[])
{
	tpModuleManager::get()->load("jpg,3ds");

	tpRefPtr<tpFont> font = new tpFont();
	font->load(argv[1]);
	font->getImage()->write("font.jpg");
	tpRefPtr<tpPrimitive> prim = new tpPrimitive();
	font->text("Test",*prim);


	tpString vs;
	prim->toString(vs);

	tpLogNotify("Vertices: %d\n",prim->getVertexCount(),vs.c_str());

	tpRefPtr<tpNode> root = new tpNode("Root");

	tpPrimitive* axis = tpPrimitiveFactory::get()->create(tpPrimitiveFactory::kAxis);

	root->addChild(axis);

	tpRefPtr<tpMyViewer> viewer = new tpMyViewer();

	viewer->setScene(root.get());
	viewer->create();
	viewer->run();

	return 0;
}
