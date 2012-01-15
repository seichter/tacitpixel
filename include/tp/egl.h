#ifndef TP_EGL_H
#define TP_EGL_H


#include <tp/rtl.h>

#if defined(_WIN32)
#	define __TP_CALL __stdcall
#else
#	define __TP_CALL
#endif

typedef int EGLint;
typedef unsigned int EGLBoolean;

typedef void *EGLConfig;
typedef void *EGLContext;
typedef void *EGLDisplay;
typedef void *EGLSurface;

typedef void* NativeDisplayType;
typedef void* NativePixmapType;
typedef void* NativeWindowType;
typedef void* NativeGLContext;

class TP_API tpEGL : public tpRTL {
public:

	// signatures

	typedef EGLint (__TP_CALL *eglGetErrorT)(void);
	typedef EGLBoolean (__TP_CALL *eglInitializeT)(EGLDisplay dpy, EGLint *major, EGLint *minor);
	typedef EGLDisplay (__TP_CALL * eglGetDisplayT)(NativeDisplayType native_display);
	typedef EGLBoolean (__TP_CALL * eglGetConfigsT)(EGLDisplay dpy,EGLConfig* config, EGLint config_size, EGLint* num_config );
	typedef EGLBoolean (__TP_CALL * eglChooseConfigT)(EGLDisplay display,EGLint const * attrib_list,EGLConfig * configs, EGLint config_size, EGLint * num_config);
	typedef EGLContext (__TP_CALL * eglCreateContextT)(EGLDisplay display,EGLConfig config,EGLContext share_context,EGLint const * attrib_list);
	typedef EGLSurface (__TP_CALL * eglCreateWindowSurfaceT)(EGLDisplay display,EGLConfig config,NativeWindowType native_window,EGLint const * attrib_list);
	typedef EGLBoolean (__TP_CALL * eglMakeCurrentT)(EGLDisplay display,EGLSurface draw,EGLSurface read,EGLContext context);
	typedef EGLBoolean (__TP_CALL * eglSwapBuffersT)(EGLDisplay display,EGLSurface surface);
	typedef EGLBoolean (__TP_CALL * eglQuerySurfaceT)(const EGLDisplay display, EGLSurface surface, EGLint attribute, EGLint * value);
	typedef const char* (__TP_CALL * eglQueryStringT)(EGLDisplay dpy,int);

	// functoids
	tpFunctoidImpl<eglGetErrorT> GetError;
	tpFunctoidImpl<eglInitializeT> Initialize;
	tpFunctoidImpl<eglGetDisplayT> GetDisplay;
	tpFunctoidImpl<eglGetConfigsT> GetConfigs;
	tpFunctoidImpl<eglChooseConfigT> ChooseConfig;
	tpFunctoidImpl<eglCreateContextT> CreateContext;
	tpFunctoidImpl<eglCreateWindowSurfaceT> CreateWindowSurface;
	tpFunctoidImpl<eglMakeCurrentT> MakeCurrent;
	tpFunctoidImpl<eglSwapBuffersT> SwapBuffers;
	tpFunctoidImpl<eglQuerySurfaceT> QuerySurface;
	tpFunctoidImpl<eglQueryStringT> QueryString;

	static tpEGL* get(bool destroy = false);

	static tpEGL& a() { return *get(); }

protected:

	tpEGL();
};

#endif
