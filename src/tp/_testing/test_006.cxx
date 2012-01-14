
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

class tpFunctoidBase;

typedef tpArray<tpFunctoidBase*> tpFunctoidArray; 


class tpFunctoidBase {
protected:
	tpString signature;
public:
	explicit tpFunctoidBase(const tpString& name) : signature(name) {}

	virtual void assign(const tpLibrary& lib) = 0;

	static void load(const tpLibrary* lib,tpFunctoidArray& farray)
	{
		for(tpFunctoidArray::iterator it = farray.begin();
			it != farray.end();
			++it)
		{
			(*it)->assign(*lib);
		}
	}
};

template <typename T>
class tpFunctoid : public tpFunctoidBase
{
public:

	explicit tpFunctoid(const tpString& name) : tpFunctoidBase(name) {}

	T f;


	
	void assign(const tpLibrary& lib)
	{
		f = reinterpret_cast<T>(lib.getAddress(tpFunctoidBase::signature));
	}
};



int main(int argc, char* argv[])
{

	tpArguments args(&argc,argv);

	tpString file;
	args.get("--file",file);
	tpEGL::get()->open(file);

	typedef int (__TP_CALL *eglGetError_fT)(void);
	typedef EGLDisplay (__TP_CALL *eglGetDisplay_fT)(NativeDisplayType);
	typedef EGLBoolean (__TP_CALL *eglInitialize_fT)(EGLDisplay dpy, EGLint *major, EGLint *minor);

	tpFunctoid<eglGetError_fT> eglError("eglGetError");
	tpFunctoid<eglGetDisplay_fT> eglGetDisplay("eglGetDisplay");
	tpFunctoid<eglInitialize_fT> eglInitialize("eglInitialize");

	tpFunctoidArray farray;
	farray.add(&eglError);
	farray.add(&eglGetDisplay);
	farray.add(&eglInitialize);

	tpFunctoidBase::load(tpEGL::get()->getLibrary().get(),farray);


	tpInt major(0),minor(0);
	EGLDisplay d = eglGetDisplay.f(0);
	eglInitialize.f(d,&major,&minor);

	tpLogMessage("EGL initialized with %d.%d",major,minor);

	return 0;
}

