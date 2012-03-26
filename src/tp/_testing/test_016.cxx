

#include <tp/viewer.h>



#include <tp/primitive.h>
#include <tp/viewer.h>
#include <tp/light.h>
#include <tp/logutils.h>
#include <tp/transform.h>
#include <tp/matop.h>

void
tpDebugPrimitive(const tpPrimitive& p)
{
	for (tpUInt i = 0; i < p.getVertices().getSize();++i)
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

	tpPrimitive* axis = tpPrimitiveFactory::create(tpPrimitiveFactory::kAxis);
	tpPrimitive* prim = tpPrimitiveFactory::create(tpPrimitiveFactory::kPlane);
	tpPrimitive* sphr = tpPrimitiveFactory::create(tpPrimitiveFactory::kSphere);
	tpPrimitive* sphr_opaque = tpPrimitiveFactory::create(tpPrimitiveFactory::kSphere);
	tpPrimitive* sphr_mat = tpPrimitiveFactory::create(tpPrimitiveFactory::kSphere);

	prim->addRenderFlag(tpRenderFlag::kBlending,tpRenderFlag::kBlendSrcAlpha,tpRenderFlag::kBlendOneMinusAlpha);
	prim->addRenderFlag(tpRenderFlag::kColorMaterial);
	prim->setUniformColor(tpVec4f(0.3,0.5,0.5,.3));
	prim->scale(tpVec3f(3,3,3));

	sphr->scale(tpVec3f(.3f,.3f,.3f));
	sphr->translate(tpVec3f(0,0,-1));
	sphr->addRenderFlag(tpRenderFlag::kColorMaterial);
	sphr->setUniformColor(tpVec4f(1,0,0,.3));

	sphr_opaque->scale(tpVec3f(.8,.8,.8));
	sphr_opaque->translate(tpVec3f(0,0,-2));
	sphr_opaque->setUniformColor(tpVec4f(0,1,0,1));
	sphr_opaque->addRenderFlag(tpRenderFlag::kColorMaterial);

	sphr_mat->scale(tpVec3f(.8,.8,.8));
	sphr_mat->translate(tpVec3f(-2,0,0));
	sphr_mat->setMaterial(tpMaterial::createDefault(0.64, 0.04814919, 0, .5));
	sphr_mat->addRenderFlag(tpRenderFlag::kLighting);

	tpDebugPrimitive(*prim);

    tpRefPtr<tpCamera> camera = viewer->getScene().getCamera();
	camera->setName("Perspective");

	camera->setProjectionPerspective(33,1.3,1,1000);
	camera->setViewLookAt(tpVec3r(5,5,5),tpVec3r(0,0,0),tpVec3r(0,1,0));
	camera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	camera->setClearColor(tpVec4f(0,.15f,.3f,1.f));

	tpRefPtr<tpLight> omni = new tpLight();
	omni->setPosition(tpVec4f(1,1,0,0));

	camera->addChild(omni.get());
	camera->addChild(axis);
	camera->addChild(sphr);
	camera->addChild(sphr_opaque);
	camera->addChild(sphr_mat);


    viewer->getScene().getCamera()->addChild(prim);

	return viewer->run();
}
