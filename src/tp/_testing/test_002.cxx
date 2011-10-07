
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
#include <tp/renderer.h>
#include <tp/stringformater.h>


tpNode* createAxis()
{
	tpPrimitive* p = new tpPrimitive(tpPrimitive::kLines);

	p->addVertex(tpVec3r(0,0,0));
	p->addVertex(tpVec3r(1,0,0),tpVec3r(0,0,1),tpVec2r(0,1),tpVec4r(1,0,0,1));
	p->addVertex(tpVec3r(0,0,0));
	p->addVertex(tpVec3r(0,1,0),tpVec3r(0,0,1),tpVec2r(0,1),tpVec4r(0,1,0,1));
	p->addVertex(tpVec3r(0,0,0));
	p->addVertex(tpVec3r(0,0,1),tpVec3r(0,0,1),tpVec2r(0,1),tpVec4r(0,0,1,1));

	return p;
}

tpNode* createNode()
{
	tpPrimitive* p = new tpPrimitive(tpPrimitive::kTriangleStrip);
	tpReal s = 2;

	p->addVertex(tpVec3r(1,1,0),tpVec3r(0,0,1));
	p->addVertex(tpVec3r(0,1,0),tpVec3r(0,0,1));
	p->addVertex(tpVec3r(1,0,0),tpVec3r(0,0,1));
	p->addVertex(tpVec3r(0,0,0),tpVec3r(0,0,1));

	tpMaterial* m = new tpMaterial();

	p->setMaterial(m);
	m->setName("Default Material");

	m->setDiffuseColor(tpVec4r(.3,0,0,1));

	//return p;
	return 0;
}


void report(const tpMat44r& mat)
{
	tpString mStr; mStr << mat;
	printf("%s",mStr.c_str());
}



int main(int argc, char* argv[])
{
	tpRefPtr<tpLibrary> mod_gl = tpLibrary::load("tacit_gl");
	tpRefPtr<tpLibrary> mod_3ds = tpLibrary::load("tacit_3ds");
	tpRefPtr<tpLibrary> mod_obj = tpLibrary::load("tacit_obj");
	tpRefPtr<tpLibrary> mod_png = tpLibrary::load("tacit_png");
	tpRefPtr<tpLibrary> mod_jpg = tpLibrary::load("tacit_jpg");

	tpRefPtr<tpPrimitive> p =
		tpPrimitiveFactory::get()->create(tpPrimitiveFactory::kAxis);

	tpRefPtr<tpNode> n = tpNode::read(argv[1]);

	if (!n.isValid()) n = createAxis();

	tpRefPtr<tpNode> root = new tpNode();

	// first one
	tpRefPtr<tpTransform> t = new tpTransform();
	tpMat44r mat; mat.identity();
	mat.setTranslation(0,0,-1);

	t->setMatrix(mat);
	t->addChild(n.get());

	root->addChild(t.get());

	tpRenderSurfaceTraits traits;
	traits.setSize(640,480).setPosition(10,10).setTitle("Tacit Pixel 3");


	tpRefPtr<tpRenderSurface> rendersurface = tpRenderSurface::create(&traits);
	tpRefPtr<tpRenderer> renderer = tpRenderer::create();

	if (!renderer.isValid()) {
		tpLogError("Could not initialize renderer");
		exit(-1);
	}

	tpRefPtr<tpCamera> camera = renderer->getActiveCamera();

	camera->setProjectionPerspective(60.0f,1.3f,0.1f,1000.0f);
	camera->setViewLookAt(tpVec3r(2,2,2),tpVec3r(0,0,0),tpVec3r(0,1,0));

	report(camera->getProjection());
	report(camera->getView());
	report(camera->getViewInverse());

	camera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	camera->setClearColor(tpVec4f(0.5f,0.5f,0.9f,1.0f));
	camera->setViewport(tpVec4i(0,0,640,480));

	if (rendersurface.isValid())
	{
		rendersurface->show(true);

		while (rendersurface->isDone() == false) {

			rendersurface->makeCurrent();

			(*renderer)(root.get());

			rendersurface->swapBuffers();
		}

		rendersurface = 0;
		renderer = 0;
		camera = 0;
	}

	return 0;
}
