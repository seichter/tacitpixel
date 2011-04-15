
#include <tp/factory.h>
#include <tp/node.h>
#include <tp/log.h>
#include <tp/image.h>
#include <tp/imageop.h>
#include <tp/library.h>
#include <tp/module.h>
#include <tp/rendersurface.h>
#include <tp/camera.h>
#include <tp/primitive.h>



int main(int argc, char* argv[])
{
	
	tpRefPtr<tpLibrary> gllib = tpLibrary::load("tacit_gl");

	tpRefPtr<tpRenderSurface> rendersurface = tpRenderSurface::create();

	tpRefPtr<tpCamera> camera = new tpCamera;

	camera->setProjectionPerspective(30.0f,1.3f,1,1000);
	camera->setViewLookAt(tpVec3r(0,0,0),tpVec3r(0,0,-25),tpVec3r(0,1,0));

	camera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	camera->setClearColor(tpVec4f(0.5f,0.5f,0.9f,1.0f));
	camera->setViewport(tpVec4i(0,0,320,240));

	//tpRefPtr<tpPrimitive> p = new tpPrimitive(tpPrimitive::kLines);

	//p->addVertex(tpVec3r(1,1,0),tpVec3r(0,0,1),tpVec2f(0.f,0.f));
	//p->addVertex(tpVec3r(0,1,0),tpVec3r(0,0,1),tpVec2f(0.f,0.f));
	//p->addVertex(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec2f(0.f,0.f));

	
	if (rendersurface.isValid())
	{

		rendersurface->show(true);

		rendersurface->setCamera(camera.get());
		rendersurface->setSceneNode(new tpNode());

		while (rendersurface->isDone() == false) {
			rendersurface->frame();
		}

		rendersurface = 0;
	}

	return 0;
}