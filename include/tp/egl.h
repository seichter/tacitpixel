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

	typedef EGLint (__TP_CALL *eglGetError)(void);
	typedef EGLBoolean (__TP_CALL *eglInitialize)(EGLDisplay dpy, EGLint *major, EGLint *minor);
	typedef EGLDisplay (__TP_CALL * eglGetDisplay)(NativeDisplayType native_display);
	typedef EGLBoolean (__TP_CALL * eglGetConfigs)(EGLDisplay dpy,EGLConfig* config, EGLint config_size, EGLint* num_config );
	typedef EGLBoolean (__TP_CALL * eglChooseConfig)(EGLDisplay display,EGLint const * attrib_list,EGLConfig * configs, EGLint config_size, EGLint * num_config);
	typedef EGLContext (__TP_CALL * eglCreateContext)(EGLDisplay display,EGLConfig config,EGLContext share_context,EGLint const * attrib_list);
	typedef EGLSurface (__TP_CALL * eglCreateWindowSurface)(EGLDisplay display,EGLConfig config,NativeWindowType native_window,EGLint const * attrib_list);
	typedef EGLBoolean (__TP_CALL * eglMakeCurrent)(EGLDisplay display,EGLSurface draw,EGLSurface read,EGLContext context);
	typedef EGLBoolean (__TP_CALL * eglSwapBuffers)(EGLDisplay display,EGLSurface surface);
	typedef EGLBoolean (__TP_CALL * eglQuerySurface)(const EGLDisplay display, EGLSurface surface, EGLint attribute, EGLint * value);
	typedef const char* (__TP_CALL * eglQueryString)(EGLDisplay dpy,int);

	static tpEGL* get(bool destroy = false);

	static tpEGL& a() { return *get(); } 

protected:

	tpEGL();
};

#endif