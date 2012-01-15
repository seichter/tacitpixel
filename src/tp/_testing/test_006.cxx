
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

	int major, minor;

	EGLDisplay d = tpEGL::a().GetDisplay.f(0);
	tpEGL::a().Initialize.f(d,&major,&minor);
	tpString EGLVendor = tpEGL::a().QueryString.f(d,EGL_VENDOR);
	tpString EGLAPIs = tpEGL::a().QueryString.f(d,EGL_CLIENT_APIS);

	tpLogMessage("EGL %d.%d",major,minor);
	tpLogMessage("\tVendor",EGLVendor.c_str());
	tpLogMessage("\tAPIs",EGLAPIs.c_str());

	tpRenderContextEGL eglctx;
	tpRenderTarget egltrg;
	eglctx.create(&egltrg);

	return 0;
}

