#include <tp/vec.h>
#include <tp/logutils.h>
#include <tp/font.h>
#include <tp/module.h>
#include <tp/light.h>
#include <tp/text.h>

#include <tp/viewer.h>
#include <tp/transform.h>
#include <tp/dialogs.h>
#include <tp/system.h>

class tpMyViewer : public tpViewer {
public:
	tpMyViewer() : tpViewer() {}

	tpRefPtr<tpText> text;
	tpTimer t;

	void
	onSurfaceEvent(tpWindowEvent& e)
	{
		if (e.getKeyState() == tpWindowEvent::kKeyUp) {

			switch (e.getKeyCode()) {
				case 27:
				case 'q':
				case 'Q':
					e.getRenderSurface()->setDone(true);
					e.setHandled();
					break;
				case 'F':
				case 'f':
					tpDialogs::showMessage("Tacit Pixel","This is the font rendering example",
						tpDialogs::kButtonsOk | tpDialogs::kStyleInfo);
					break;
				case 'S':
				case 's':
					tpDialogs::fileSelector("Open File",tpSystem::get()->getCWD(),"",tpDialogs::kButtonsOkCancel);
					break;

			}
		}

		e.setHandled();

		text->set(tpString::format("tacit pixel %3.3f",t.getElapsed(tpTimer::kTimeSeconds)));
	}
};


int main(int argc,char* argv[])
{

	if (argc < 2) {
		tpLogError("No font file provided - usage %s font.ttf",argv[0]);
		return 0;
	}

	tpModuleManager::get()->load("jpg,3ds");

	tpRefPtr<tpMyViewer> viewer = new tpMyViewer();


	tpRefPtr<tpFont> font = new tpFont();
	font->load(argv[1]);
	font->getImage()->write("font.jpg");
	tpRefPtr<tpPrimitive> prim = new tpPrimitive();
	font->text("Tacit Pixel",*prim);

	tpRefPtr<tpNode> root = new tpNode("Root");

	tpPrimitive* axis = tpPrimitiveFactory::create(tpPrimitiveFactory::kAxis);

	tpText* text = new tpText();
	text->setFont(argv[1]);
	text->set("x");

	tpTransform* t = new tpTransform;
//	t->addChild(prim.get());
	t->addChild(text);



	tpMat44r m(tpMat44r::Identity()); m.scale(.001,.001,.001);
	t->setMatrix(m);

	tpRefPtr<tpTransform> lt = new tpTransform();
	lt->setMatrix(tpMat44Op::translation(5.f,5.f,5.f));

	tpRefPtr<tpLight> light = new tpLight();
	light->setAmbientColor(tpVec4f(0.1f,0.1f,0.1f,1.f));

	lt->addChild(light.get());

	root->addChild(lt.get());
	root->addChild(axis);
	root->addChild(t);

	viewer->text = text;


	viewer->getScene().getActiveCamera()->addChild(root.get());
	viewer->getScene().getActiveCamera()->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	viewer->getScene().getActiveCamera()->setClearColor(tpVec4f(0,.15f,.3f,1.f));

	viewer->getScene().getActiveCamera()->setViewLookAt(tpVec3r(2,2,2),tpVec3r(0,0,0),tpVec3r(0,1,0));
	viewer->getScene().getActiveCamera()->setProjectionPerspective(35,1.3,0.1,100);



	viewer->create();
	return viewer->run();
}
