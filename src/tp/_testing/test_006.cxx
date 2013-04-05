
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
#include <tp/thread.h>

//#include <GL/gl.h>


int main(int argc, char* argv[])
{

	tpArguments args(&argc,argv);

	tpString file;
	args.get("--egl",file);

	tpEGL::get()->load(file);

	// we need traits here
    tpWindowTraits traits;

    tpRefPtr<tpWindow> surface = tpWindow::create(&traits);

	surface->setCaption("Tacit Pixel EGL Test");
	surface->show(true);

    tpRefPtr<tpRenderContext> render_context = tpRenderContext::create("EGL");

    render_context->bind(surface.get());
    render_context->makeCurrent();

	while (surface->isValid()) {

		surface->update();

        if(render_context->makeCurrent()) {

            render_context->swapBuffers();
        }

        tpThread::yield();
	}

	return 0;
}

