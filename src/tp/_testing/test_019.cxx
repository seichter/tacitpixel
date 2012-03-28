#include <tp/viewer.h>
#include <tp/primitive.h>
#include <tp/viewer.h>
#include <tp/light.h>
#include <tp/logutils.h>
#include <tp/transform.h>
#include <tp/matop.h>
#include <tp/ui.h>


int main(int argc, char* argv[])
{
	tpRefPtr<tpViewer> viewer = new tpViewer();
	viewer->create();

	tpUI::setFont(argv[1]);

	tpString loremIpsum =
	"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod\n"
	"tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim\n"
	"veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea\n"
	"commodo consequat. Duis aute irure dolor in reprehenderit in voluptate\n"
	"velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat\n"
	"cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id\n"
	"est laborum.";

	tpNode* button = tpUI::Button("Cancel",tpRect(-50,-10,100,20));
	tpNode* txtbox = tpUI::Text(loremIpsum,tpPoint(0,-20));

	viewer->getScene().getCamera()->setClearColor(tpVec4f(.1,.2,.5,1.));

	tpLight* uilight = new tpLight();

	// set to screen size
	viewer->getScene().getCamera()->setProjectionOrtho(-320,320,-240,240,-1,1);
	viewer->getScene().getCamera()->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	viewer->getScene().getCamera()->addChild(button);
	viewer->getScene().getCamera()->addChild(uilight);
	viewer->getScene().getCamera()->addChild(txtbox);

	return viewer->run();
}
