
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




int main(int argc, char* argv[])
{

	tpArguments args(&argc,argv);

	tpString file;
	args.get("--file",file);

	tpEGL::get()->open(file);

	tpInt major(0),minor(0);

	EGLDisplay d = tpEGL::a().GetDisplay.f(0);
	tpEGL::a().Initialize.f(d,&major,&minor);

	tpLogMessage("EGL initialized with %d.%d",major,minor);

	return 0;
}

