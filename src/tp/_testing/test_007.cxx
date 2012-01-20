
#include <tp/rendersurface.h>
#include <tp/log.h>

int main(int argc, char* argv[]) {

	tpRefPtr<tpRenderSurface> window = tpRenderSurface::create();

	window->setCaption("Tacit Pixel 3");

	window->show(true);

	while (window->isValid()) {
		window->update();
	}

	tpLogNotify("Done");

	return 0;
}
