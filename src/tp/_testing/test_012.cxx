
#include <tp/primitive.h>
#include <tp/viewer.h>
#include <tp/light.h>


int main(int argc, char* argv[])
{
	tpRefPtr<tpViewer> viewer = new tpViewer();
	viewer->create();

	tpPrimitive* prim = tpPrimitiveFactory::create(tpPrimitiveFactory::kSphere);
    prim->setMaterial(tpMaterial::createDefault(.5,.6,.1,.85));

	tpRefPtr<tpCamera> camera = viewer->getScene().getCamera();
	camera->setName("Perspective");

	camera->setProjectionPerspective(33,1.3,1,1000);
	camera->setViewLookAt(tpVec3r(2,2,2),tpVec3r(0,0,0),tpVec3r(0,1,0));
	camera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	camera->setClearColor(tpVec4f(0,.15f,.3f,1.f));

	tpRefPtr<tpLight> light = new tpLight();
//	light->setPosition(tpVec3f(5,5,5));
	light->setAmbientColor(tpVec4f(0.1f,0.1f,0.1f,1.f));

	camera->addChild(light.get());

	viewer->getScene().getCamera()->addChild(prim);

	return viewer->run();
}
