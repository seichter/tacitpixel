#include <tp/vec.h>
#include <tp/logutils.h>
#include <tp/font.h>
#include <tp/module.h>
#include <tp/light.h>
#include <tp/text.h>

#include <tp/viewer.h>
#include <tp/transform.h>

class tpMyViewer : public tpViewer {
public:
	tpMyViewer() : tpViewer() {}

	tpRefPtr<tpText> text;
	tpTimer t;

	void
	onSurfaceEvent(tpWindowEvent& e)
	{
		if (e.getKeyCode() == 27 && e.getKeyState() == tpWindowEvent::kKeyUp) {
			e.getRenderSurface()->setDone();
			e.setHandled();
		}

		text->set(tpString::format("%3.3f",t.getElapsed(tpTimer::kTimeSeconds)));
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



    tpMat44r m; m.identity();  m.scale(.001,.001,.001);
	t->setMatrix(m);

	tpRefPtr<tpLight> light = new tpLight();
	light->setPosition(tpVec3f(5,5,5));
	light->setAmbientColor(tpVec4f(0.1f,0.1f,0.1f,1.f));

	root->addChild(light.get());
	root->addChild(axis);
	root->addChild(t);

	viewer->text = text;


	viewer->getScene().getActiveCamera()->addChild(root.get());
	viewer->getScene().getActiveCamera()->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	viewer->getScene().getActiveCamera()->setClearColor(tpVec4f(0,.15f,.3f,1.f));



	viewer->create();
	return viewer->run();
}
