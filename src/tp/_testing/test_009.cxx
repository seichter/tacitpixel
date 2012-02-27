
#include <tp/viewer.h>
#include <tp/module.h>
#include <tp/primitive.h>
#include <tp/light.h>



int main(int argc,char* argv[]) {

	tpModuleManager::get()->load("jpg");

    tpRefPtr<tpViewer> viewer = new tpViewer();
    viewer->create();

    tpRefPtr<tpCamera> camera = viewer->getScene().getActiveCamera();
    camera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
    camera->setClearColor(tpVec4f(0,.15f,.3f,1.f));

	tpRefNode root = new tpNode();

	root->addChild(tpPrimitiveFactory::create(tpPrimitiveFactory::kAxis));
	tpPrimitive* p = tpPrimitiveFactory::create(tpPrimitiveFactory::kPlane);

    tpRefPtr<tpImage> i = tpImage::read(argv[1]);

    // only create a texture if the image is valid
    if (i.isValid()) {
        tpTexture* t = new tpTexture();
        t->setImage(i.get());
        t->setFormat(tpTexture::kFormatRGB);
        p->setTexture(t);
    }

    // add the textured plane
	root->addChild(p);

    // add some light
	tpRefPtr<tpLight> light = new tpLight();
	light->setPosition(tpVec3f(2,2,2));
	light->setAmbientColor(tpVec4f(0.1f,0.1f,0.1f,1.f));


	root->addChild(light.get());


	return viewer->run();;
}
