
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
#include <tp/transform.h>



int main(int argc, char* argv[])
{
	
	tpRefPtr<tpLibrary> mod_gl = tpLibrary::load("tacit_gl");
	//tpRefPtr<tpLibrary> mod_3ds = tpLibrary::load("tacit_3ds");
	tpRefPtr<tpLibrary> mod_obj = tpLibrary::load("tacit_obj");
	

	tpRefPtr<tpPrimitive> p = 
		tpPrimitiveFactory::get()->create(tpPrimitiveFactory::kAxis);
		
	tpRefPtr<tpNode> n = tpNode::read(argv[1]);
		

	//new tpPrimitive(tpPrimitive::kTriangleStrip);
	//p->addVertex(tpVec3r(1,1,0),tpVec3r(0,0,1),tpVec2f(0.f,0.f));
	//p->addVertex(tpVec3r(0,1,0),tpVec3r(0,0,1),tpVec2f(0.f,0.f),tpVec4r(0,1,0,1));
	//p->addVertex(tpVec3r(1,0,0),tpVec3r(0,0,1),tpVec2f(0.f,0.f),tpVec4r(1,0,0,1));
	//p->addVertex(tpVec3r(0,0,0),tpVec3r(0,0,1),tpVec2f(0.f,0.f));

	tpRefPtr<tpNode> root = new tpNode();

	tpRefPtr<tpTransform> t = new tpTransform();
	t->getMatrix().setIdentity();
	t->getMatrix().translate(-2.0,1.0,-2.0);
	
	t->addChild(p.get());

	t->addChild(n.get());

	root->addChild(p.get());
	root->addChild(t.get());
	
	tpRenderSurfaceTraits traits;
	traits.setSize(640,480).setPosition(10,10);

	tpRefPtr<tpRenderSurface> rendersurface = tpRenderSurface::create(&traits);

	tpRefPtr<tpCamera> camera = new tpCamera;

	camera->setProjectionPerspective(45.0f,1.3f,0.1f,1000.0f);
	camera->setViewLookAt(tpVec3r(0,1,1),tpVec3r(0,0,0),tpVec3r(0,1,0));

	camera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	camera->setClearColor(tpVec4f(0.5f,0.5f,0.9f,1.0f));
	camera->setViewport(tpVec4i(0,0,640,480));
	
	if (rendersurface.isValid())
	{

		rendersurface->show(true);

		rendersurface->setCamera(camera.get());
		rendersurface->setSceneNode(root.get());

		while (rendersurface->isDone() == false) {
			rendersurface->frame();
		}

		rendersurface = 0;
	}

	return 0;
}
