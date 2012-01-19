
#include <tp/rendersurface.h>


int main(int argc, char* argv[]) {


	tpRefPtr<tpRenderSurface> window = tpRenderSurface::create();

	window->show(true);

	while (window->isValid()) {
		window->update();
	}

	return 0;
}
