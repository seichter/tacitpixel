#include <tp/viewer.h>


#include <tp/vec.h>
#include <tp/logutils.h>
#include <tp/font.h>
#include <tp/module.h>
#include <tp/rendersurface.h>
#include <tp/renderer.h>
#include <tp/thread.h>


tpViewer::tpViewer() : tpReferenced() {}


void
tpViewer::setScene(tpNode* scene) {
	mScene = scene;
}

tpNode*
tpViewer::getScene() { return mScene.get(); }


bool
tpViewer::create(const tpString& title/* = "tpViewer"*/,
				 tpUInt w/* = 640*/, tpUInt h /*= 480*/,
				 tpUInt x /*= 0*/, tpUInt y /*= 0*/,
				 tpUInt flags /*= 0*/) {

	tpRenderSurfaceTraits traits;
	traits.setSize(w,h).setPosition(x,y).setTitle(title);

	mRenderer = tpRenderer::create();
	mSurface = tpRenderSurface::create(&traits);
	mSurface->setContext(0);

	if (!mRenderer.isValid() || !mSurface.isValid() || !mSurface->hasContext()) return false;

	mCamera = mRenderer->getActiveCamera();
	mCamera->setName("Default");

	mCamera->setProjectionPerspective(60.0f,1.3f,0.1f,1000.0f);
	mCamera->setViewLookAt(tpVec3r(2,2,2),tpVec3r(0,0,0),tpVec3r(0,1,0));


	mCamera->setClearFlags(tpCamera::kClearColor | tpCamera::kClearDepth);
	mCamera->setClearColor(tpVec4f(0.5f,0.5f,0.9f,1.0f));

	mCamera->setViewport(tpVec4i(0,0,w,h));

	mSurface->show(true);

	return true;

}

void
tpViewer::frame() {

	if (mSurface->getContext()->makeCurrent())
	{
		(*mRenderer)(mScene.get());

		mSurface->getContext()->swapBuffers();
	}

	mSurface->update();

	tpThread::yield();
}

/*virtual*/ void
tpViewer::onSurfaceEvent(tpRenderSurfaceEvent& e) {
}

void
tpViewer::run() {

	// now also bind the event handler
	mSurface->getEventHandler().attach<tpRenderSurfaceEvent,tpViewer>(this,&tpViewer::onSurfaceEvent);

	while (mSurface->isValid()) {
		this->frame();
	}
}

