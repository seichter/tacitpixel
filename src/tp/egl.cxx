#include <tp/egl.h>

tpEGL::tpEGL()
{
	getFunctions()
			.push(&GetError,"eglGetError")
			.push(&Initialize,"eglInitialize")
			.push(&GetDisplay,"eglGetDisplayT");

//	tpFunctoidImpl<eglGetErrorT> GetError;
//	tpFunctoidImpl<eglInitializeT> Initialize;
//	tpFunctoidImpl<eglGetDisplayT> GetDisplay;
//	tpFunctoidImpl<eglGetConfigsT> GetConfigs;
//	tpFunctoidImpl<eglChooseConfigT> ChooseConfig;
//	tpFunctoidImpl<eglCreateContextT> CreateContext;
//	tpFunctoidImpl<eglCreateWindowSurfaceT> CreateWindowSurface;
//	tpFunctoidImpl<eglMakeCurrentT> MakeCurrent;
//	tpFunctoidImpl<eglSwapBuffersT> SwapBuffers;
//	tpFunctoidImpl<eglQuerySurfaceT> QuerySurface;
//	tpFunctoidImpl<eglQueryStringT> QueryString;

}

tpEGL* tpEGL::get( bool destroy /*= false*/ )
{
	static tpRefPtr<tpEGL> mEGL;
	if (!mEGL.isValid()) mEGL = new tpEGL;
	if (destroy) mEGL = 0L;
	return mEGL.get();
}
