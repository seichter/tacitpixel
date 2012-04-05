#include <tp/viewer.h>


#include <tp/vec.h>
#include <tp/logutils.h>
#include <tp/font.h>
#include <tp/module.h>
#include <tp/window.h>
#include <tp/renderer.h>
#include <tp/thread.h>


tpViewer::tpViewer()
	: tpReferenced()
	, mScene(new tpScene())
{
}

bool
tpViewer::create(const tpString& title/* = "tpViewer"*/,
				 tpUInt w/* = 640*/, tpUInt h /*= 480*/,
				 tpUInt x /*= 0*/, tpUInt y /*= 0*/,
				 tpUInt flags /*= 0*/) {

	tpWindowTraits traits;
	traits.setSize(w,h).setPosition(x,y).setTitle(title);

	// setup renderer and viewer
	mSurface = tpWindow::create(&traits);
	mSurface->setContext(0);

    // need to fix
    mRenderer = tpRenderer::create(mSurface->getContext()->getRendererTraits());

	if (!mRenderer.isValid() || !mSurface.isValid() || !mSurface->hasContext()) return false;

	// now also bind the event handler
	mSurface->getEventHandler().attach<tpWindowEvent,tpViewer>(this,&tpViewer::_dispatch);


	tpVec2i s = mSurface->getClientAreaSize();
	mScene->getCamera()->setViewport(tpVec4i(0,0,s(0),s(1)));

	return true;

}

void
tpViewer::frame() {

    if (mSurface->getContext()->makeCurrent())
	{
        mRenderer->render(mScene.get());
		mSurface->getContext()->swapBuffers();
	}

	mSurface->update();

	tpThread::yield();
}

/*virtual*/ void
tpViewer::onSurfaceEvent(tpWindowEvent& e)
{
}

/*virtual*/ void
tpViewer::_dispatch(tpWindowEvent& e)
{
	if (e.getId() == tpWindowEvent::kWindowSize)
	{
		tpVec2i size = e.getRenderSurface()->getClientAreaSize();
		this->getScene().getCamera()->setViewport(tpVec4i(0,0,size(0),size(1)));
		e.setHandled(true);

		this->frame();

		return;

	}

	this->onSurfaceEvent(e);
}

int
tpViewer::run() {

	mSurface->show(true);

	while (mSurface->isValid()) {
		this->frame();
	}

	mSurface = 0;
	mScene = 0;

	return 0;
}

