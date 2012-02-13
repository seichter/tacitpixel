

#include <tp/viewer.h>
#include <tp/primitive.h>
#include <tp/projection.h>
#include <tp/matop.h>
#include <tp/camera.h>



int main(int argc, char* argv[])
{

	tpRefPtr<tpViewer> viewer = new tpViewer();

	tpRefPtr<tpPrimitive> axis = tpPrimitiveFactory::get()->create(tpPrimitiveFactory::kAxis);

	tpRefPtr<tpNode> root = new tpNode;

	// standard orhographic camera
	tpRefPtr<tpCamera> cam_1 = new tpCamera;
	cam_1->setName("Ortho");

	// offset camera
	tpRefPtr<tpCamera> cam_2 = new tpCamera;
	cam_2->setName("Perspective");

	viewer->getScene().addCamera(cam_1.get(),true);
	viewer->getScene().addCamera(cam_2.get(),false);


	cam_1->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	cam_1->setClearColor(tpVec4f(0,.15f,.3f,1.f));

	cam_2->setProjectionPerspective(33,1.3,1,1000);
	cam_2->setViewLookAt(tpVec3r(2,2,2),tpVec3r(0,0,0),tpVec3r(0,1,0));
	cam_1->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);



	cam_1->addChild(axis.get());
	cam_2->addChild(axis.get());

	viewer->create();

	return viewer->run();

}
