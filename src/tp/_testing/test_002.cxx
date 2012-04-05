
#include <tp/factory.h>
#include <tp/node.h>
#include <tp/log.h>
#include <tp/image.h>
#include <tp/imageop.h>
#include <tp/library.h>
#include <tp/module.h>
#include <tp/window.h>
#include <tp/camera.h>
#include <tp/primitive.h>
#include <tp/transform.h>
#include <tp/renderer.h>
#include <tp/stringformater.h>
#include <tp/thread.h>



void report(const tpMat44r& mat)
{
	tpString mStr; mStr << mat;
	printf("%s",mStr.c_str());
}



int main(int argc, char* argv[])
{
	tpRefPtr<tpLibrary> mod_surface = tpLibrary::load("tacit_glsurface");
	tpRefPtr<tpLibrary> mod_renderer = tpLibrary::load("tacit_glrenderer");
	tpRefPtr<tpLibrary> mod_3ds = tpLibrary::load("tacit_3ds");
	tpRefPtr<tpLibrary> mod_obj = tpLibrary::load("tacit_obj");
	tpRefPtr<tpLibrary> mod_png = tpLibrary::load("tacit_png");
	tpRefPtr<tpLibrary> mod_jpg = tpLibrary::load("tacit_jpg");

	tpRefPtr<tpPrimitive> p =
		tpPrimitiveFactory::create(tpPrimitiveFactory::kAxis);

	tpRefPtr<tpNode> n = tpNode::read(argv[1]);

	if (!n.isValid()) n = tpPrimitiveFactory::create(tpPrimitiveFactory::kAxis);

	tpRefPtr<tpNode> root = new tpNode();

	// first one
	tpRefPtr<tpTransform> t = new tpTransform();
    tpMat44r mat(tpMat44r::Identity());
	mat.setTranslation(0,0,-1);

	t->setMatrix(mat);
	t->addChild(n.get());

	root->addChild(t.get());

	tpWindowTraits traits;
	traits.setSize(640,480).setPosition(10,10).setTitle("Tacit Pixel 3");


	tpRefPtr<tpWindow> rendersurface = tpWindow::create(&traits);


    rendersurface->setContext(0);


    // need for getting the correct renderer
    tpRefPtr<tpRenderer> renderer = tpRenderer::create(rendersurface->getContext()->getRendererTraits());

	if (!renderer.isValid()) {
		tpLogError("Could not initialize renderer");
		exit(-1);
	}

//	tpRefPtr<tpCamera> camera = renderer->getActiveCamera();

//	camera->setProjectionPerspective(60.0f,1.3f,0.1f,1000.0f);
//	camera->setViewLookAt(tpVec3r(2,2,2),tpVec3r(0,0,0),tpVec3r(0,1,0));

//	camera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
//	camera->setClearColor(tpVec4f(0.5f,0.5f,0.9f,1.0f));
//	camera->setViewport(tpVec4i(0,0,640,480));

	if (rendersurface.isValid())
	{
		rendersurface->show(true);

		while (rendersurface->isValid()) {

			rendersurface->update();

//			rendersurface->makeCurrent();

//			(*renderer)(root.get());

//			rendersurface->swapBuffers();

			tpThread::yield();
		}

		rendersurface = 0;
		renderer = 0;
//		camera = 0;
	}

	return 0;
}
