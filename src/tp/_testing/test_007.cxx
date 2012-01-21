
#include <tp/rendersurface.h>
#include <tp/log.h>
#include <tp/thread.h>
#include <tp/renderer.h>

int main(int argc, char* argv[]) {

	tpRefPtr<tpRenderSurface> window = tpRenderSurface::create();

    window->setContext(0);

    tpLogNotify("%s",window->getContext()->getVendor().c_str());
    tpLogNotify("%s",window->getContext()->getRenderer().c_str());
    tpLogNotify("%s",window->getContext()->getVersion().c_str());

	window->setCaption("Tacit Pixel 3");

	window->show(true);

    tpRefPtr<tpRenderBuffer> pixmap = tpRenderBuffer::create(tpSize(320,240),tpPixelFormat::kRGB_888);

    tpRefPtr<tpImage> pixmap_output = new tpImage();
    //pixmap->copy(*pixmap_output);

    pixmap_output->write("test_007.jpg");

    tpRefPtr<tpRenderer> renderer = tpRenderer::create();

	while (window->isValid()) {

        window->getContext()->wait(tpRenderContext::kWaitGL);
		window->update();

        tpThread::yield();
	}




	tpLogNotify("Done");

	return 0;
}
