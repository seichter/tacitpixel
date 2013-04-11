/*
 * Copyright (C) 1999-2013 Hartmut Seichter
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
	 
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
    mSurface->attach<tpWindowEvent,tpViewer>(this,&tpViewer::_dispatch);


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

