#include <tp/viewer.h>


#include <tp/vec.h>
#include <tp/logutils.h>
#include <tp/font.h>
#include <tp/module.h>
#include <tp/rendersurface.h>
#include <tp/renderer.h>
#include <tp/thread.h>


tpViewer::tpViewer()
	: tpReferenced()
	, mScene(new tpScene())
{}

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
tpViewer::onSurfaceEvent(tpRenderSurfaceEvent& e)
{
}

/*virtual*/ void
tpViewer::_dispatch(tpRenderSurfaceEvent& e)
{
	this->onSurfaceEvent(e);
}

int
tpViewer::run() {

	// now also bind the event handler
	mSurface->getEventHandler().attach<tpRenderSurfaceEvent,tpViewer>(this,&tpViewer::_dispatch);

	while (mSurface->isValid()) {
		this->frame();
	}

	return 0;
}

