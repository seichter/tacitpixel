
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
#include <tp/rendersurface.h>


int main(int argc, char* argv[])
{

	tpArguments args(&argc,argv);

	tpString file;
	args.get("--egl",file);

	tpEGL::get()->open(file);

	tpRefPtr<tpRenderSurface> surface = tpRenderSurface::create();
	surface->show(true);

	tpRenderContextEGL egl_context;

	egl_context.create(surface.get());

	while (!surface->isDone()) {

	}

	return 0;
}

