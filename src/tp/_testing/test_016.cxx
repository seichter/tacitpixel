

#include <tp/viewer.h>



#include <tp/primitive.h>
#include <tp/viewer.h>
#include <tp/light.h>
#include <tp/logutils.h>

void
tpDebugPrimitive(const tpPrimitive& p)
{
	for (int i = 0; i < p.getVertices().getSize();++i)
	{
		tpVec4f v(p.getVertices().getData()[i*4],
				  p.getVertices().getData()[i*4+1],
				  p.getVertices().getData()[i*4+2],
				  p.getVertices().getData()[i*4+3]
				  );

		tpVec4f n(p.getNormals().getData()[i*p.getNormals().getStride()],
				  p.getNormals().getData()[i*p.getNormals().getStride()+1],
				  p.getNormals().getData()[i*p.getNormals().getStride()+2],
				  p.getNormals().getData()[i*p.getNormals().getStride()+3]
				  );

		tpVec4f c(p.getColors().getData()[i*4],
				  p.getColors().getData()[i*4+1],
				  p.getColors().getData()[i*4+2],
				  p.getColors().getData()[i*4+3]
				  );


		tpLog::get() << "\nv " << v <<
						"\nn " << n <<
						"\nc " << c << "\n";
	}
}


int main(int argc, char* argv[])
{
	tpRefPtr<tpViewer> viewer = new tpViewer();
	viewer->create();

	tpPrimitive* axis = tpPrimitiveFactory::get()->create(tpPrimitiveFactory::kAxis);
	tpPrimitive* prim = tpPrimitiveFactory::get()->create(tpPrimitiveFactory::kPlane);
	tpPrimitive* sphr = tpPrimitiveFactory::get()->create(tpPrimitiveFactory::kSphere);

	prim->setAlpha(0.4f);
	prim->setUniformColor(tpVec4f(0.3,0.5,0.5,.3));

	sphr->scale(tpVec3f(.3f,.3f,.3f));
	sphr->translate(tpVec3f(0,0,-1));

	tpDebugPrimitive(*prim);

	tpRefPtr<tpCamera> camera = viewer->getScene().getActiveCamera();
	camera->setName("Perspective");

	camera->setProjectionPerspective(33,1.3,1,1000);
	camera->setViewLookAt(tpVec3r(5,5,5),tpVec3r(0,0,0),tpVec3r(0,1,0));
	camera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	camera->setClearColor(tpVec4f(0,.15f,.3f,1.f));

	tpRefPtr<tpLight> light = new tpLight();
	light->setPosition(tpVec3f(5,5,5));
	light->setAmbientColor(tpVec4f(0.1f,0.1f,0.1f,1.f));

	camera->addChild(light.get());
	camera->addChild(axis);
	camera->addChild(sphr);

	viewer->getScene().getActiveCamera()->addChild(prim);

	return viewer->run();
}
