
#include <tp/arguments.h>
#include <tp/library.h>
#include <tp/refptr.h>

#include <tp/referenced.h>
#include <tp/string.h>
#include <tp/array.h>
#include <tp/map.h>
#include <tp/rtl.h>
#include <tp/egl.h>
#include <tp/log.h>
#include <tp/window.h>


int main(int argc, char* argv[])
{

	tpArguments args(&argc,argv);

	tpString file;
	args.get("--egl",file);

	tpEGL::get()->load(file);

	tpRefPtr<tpWindow> surface = tpWindow::create();

	surface->setCaption("Tacit Pixel EGL Test");
	surface->show(true);

	tpRenderContextEGL egl_context;

	egl_context.create(surface.get());

	while (surface->isValid()) {
		surface->update();
	}

	return 0;
}

