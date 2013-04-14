
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
#include <tp/dialogs.h>
#include <tp/system.h>



class tpViewerShow : public tpViewer {

public:


	virtual void
	onSurfaceEvent(tpWindowEvent& e)
	{

		tpLogMessage("Mouse: %d %d (%d) [%d] Key: %d (%c) %d",
					 e.getMousePosition()(0),e.getMousePosition()(1),
					 e.getMouseKey(),e.getMouseState(),
					 e.getKeyCode(),e.getKeyCode(),e.getKeyState());

		tpCamera* camera = mScene->getCamera();

		tpLog::get() << camera->getTranslation() << "\n";

		if (e.getKeyState() == tpWindowEvent::kKeyUp)
		{
			switch (e.getKeyCode()) {
				case 27:
				case 'q':
				case 'Q': e.getRenderSurface()->setDone();
					break;
				case 'o':
				case 'O' :
					{
						static tpString searchpath;
						if (searchpath.isEmpty()) searchpath = tpSystem::get()->getCWD();
						tpString filename = tpDialogs::fileSelector("Open File",searchpath,"",0);
						if (!filename.isEmpty()) {
							tpRefPtr<tpNode> node = tpNode::read(filename);
							if (node.isValid()) {
								mScene->getCamera()->removeAllChildren();
								mScene->getCamera()->addChild(node.get());
							}
						}
					}
				default:
					break;

			}
		}

		if (e.getKeyState() == tpWindowEvent::kKeyDown)
		{

			tpVec3r pos = camera->getTranslation();
			tpMat44r vmat = camera->getView();

			switch (e.getKeyCode()) {
				case 'x':
				case 'X':
					pos += tpVec3r::All(.5);
					break;
				case 'z':
				case 'Z':
					pos -= tpVec3r::All(.5);
					break;

			case 'g':
			case 'G':
				vmat.rotate(tpVec3r(0,0,1),5);
				camera->setView(vmat);
				e.setHandled(true);
				return;
						break;
				case 'a':
				case 'A':
					pos(0) -= 1;
					break;
				case 'd':
				case 'D':
					pos(0) += 1;
					break;

				case 'w':
				case 'W':
					pos(2) -= 1;
					break;
				case 's':
				case 'S':
					pos(2) += 1;
					break;

			default:
					break;
			}

			camera->setTranslation(pos);
		}

		if (e.getMouseState() == tpWindowEvent::kMouseDown) {

			tpVec3r delta(0.05,0,0);
			tpVec3r pos = camera->getTranslation();

			if (e.getMouseKey() == tpWindowEvent::kMouseKeyLeft) {
				pos -= delta;
			} else {
				pos += delta;
			}

			camera->setTranslation(pos);
		}

		e.setHandled(true);
	}
};


int main(int argc,char* argv[])
{
	tpModuleManager::get()->load("obj,3ds,jpg");

	tpString file,plugins;
	tpArguments args(&argc,argv);

	if (args.get("--plugins",plugins)) {}
	if (args.get("--file",file)) {}

	tpRefNode root = new tpNode;

	tpRefNode scene = (file == "axis")
			? tpPrimitiveFactory::create(tpPrimitiveFactory::kAxis)
			:  tpNode::read(file);

	if (!scene.isValid() && !(file == "empty")) {
		tpLogError("Can't load file");
		return -1;
	}

	if (!scene.isValid())
		scene = new tpNode();

	tpRefPtr<tpTransform> st = new tpTransform();
	st->setMatrix(tpMat44r::Identity());
	st->setMatrix(tpMat44r::Rotation(tpVec3r(1,0,0),-90) *       tpMat44r::Rotation(tpVec3r(0,1,0),-90));

	st->addChild(scene.get());
	root->addChild(st.get());


	tpRefPtr<tpTransform> lt = new tpTransform();
	tpRefPtr<tpLight> l = new tpLight();

	lt->addChild(l.get());

	lt->setMatrix(tpMat44Op::translation<tpReal>(10,10,10));

	l->setAmbientColor(tpVec4f(0.1f,0.1f,0.1f,1.f));
	l->setDiffuseColor(tpVec4f(.8f,.8f,.8f,1.f));
	l->setPosition(tpVec4f(0.f,1.f,0.f,0.f));

	root->addChild(lt.get());

	tpRefPtr<tpViewer> viewer = new tpViewerShow;

	viewer->getScene().getCamera()->setViewLookAt(tpVec3r(5,1,5),tpVec3r(0,0,0),tpVec3r(0,1,0));
	viewer->getScene().getCamera()->setProjectionPerspective(45,1.3,.1,100);

	viewer->getScene().getCamera()->setClearColor(tpVec4f(.3,.4,.5,1));
	viewer->getScene().getCamera()->setClearFlags(tpCamera::kClearDepth | tpCamera::kClearColor);

	viewer->getScene().getCamera()->addChild(root.get());

	viewer->create();
	viewer->run();

	return 0;
}
