#ifndef TP_EGL_H
#define TP_EGL_H


#include <tp/rtl.h>
#include <tp/rendersurface.h>

#if defined(_WIN32)
#	define __TP_CALL __stdcall
#else
#	define __TP_CALL
#endif

typedef int EGLint;
typedef unsigned int EGLBoolean;
typedef unsigned short EGLenum;

typedef void *EGLConfig;
typedef void *EGLContext;
typedef void *EGLDisplay;
typedef void *EGLSurface;

typedef void* NativeDisplayType;
typedef void* NativePixmapType;
typedef void* NativeWindowType;
typedef void* NativeGLContext;


/* EGL aliases */
#define EGL_FALSE			0
#define EGL_TRUE			1

/* Out-of-band handle values */
/* These values may vary depending on semantics of native concepts */
#define EGL_DEFAULT_DISPLAY		((void *)0)
#define EGL_NO_CONTEXT			((EGLContext)0)
#define EGL_NO_DISPLAY			((EGLDisplay)0)
#define EGL_NO_SURFACE			((EGLSurface)0)

/* Config attributes */
#define EGL_BUFFER_SIZE			0x3020
#define EGL_ALPHA_SIZE			0x3021
#define EGL_BLUE_SIZE			0x3022
#define EGL_GREEN_SIZE			0x3023
#define EGL_RED_SIZE			0x3024
#define EGL_DEPTH_SIZE			0x3025
#define EGL_STENCIL_SIZE		0x3026
#define EGL_CONFIG_CAVEAT		0x3027
#define EGL_CONFIG_ID			0x3028
#define EGL_LEVEL			0x3029
#define EGL_MAX_PBUFFER_HEIGHT		0x302A
#define EGL_MAX_PBUFFER_PIXELS		0x302B
#define EGL_MAX_PBUFFER_WIDTH		0x302C
#define EGL_NATIVE_RENDERABLE		0x302D
#define EGL_NATIVE_VISUAL_ID		0x302E
#define EGL_NATIVE_VISUAL_TYPE		0x302F
#define EGL_PRESERVED_RESOURCES		0x3030
#define EGL_SAMPLES			0x3031
#define EGL_SAMPLE_BUFFERS		0x3032
#define EGL_SURFACE_TYPE		0x3033
#define EGL_TRANSPARENT_TYPE		0x3034
#define EGL_TRANSPARENT_BLUE_VALUE	0x3035
#define EGL_TRANSPARENT_GREEN_VALUE	0x3036
#define EGL_TRANSPARENT_RED_VALUE	0x3037
#define EGL_NONE			0x3038	/* Attrib list terminator */
#define EGL_BIND_TO_TEXTURE_RGB		0x3039
#define EGL_BIND_TO_TEXTURE_RGBA	0x303A
#define EGL_MIN_SWAP_INTERVAL		0x303B
#define EGL_MAX_SWAP_INTERVAL		0x303C
#define EGL_LUMINANCE_SIZE		0x303D
#define EGL_ALPHA_MASK_SIZE		0x303E
#define EGL_COLOR_BUFFER_TYPE		0x303F
#define EGL_RENDERABLE_TYPE		0x3040
#define EGL_CONTEXT_CLIENT_VERSION 0x3098

/* Config attribute mask bits */
#define EGL_PBUFFER_BIT			0x0001	/* EGL_SURFACE_TYPE mask bits */
#define EGL_PIXMAP_BIT			0x0002	/* EGL_SURFACE_TYPE mask bits */
#define EGL_WINDOW_BIT			0x0004	/* EGL_SURFACE_TYPE mask bits */
#define EGL_VG_COLORSPACE_LINEAR_BIT	0x0020	/* EGL_SURFACE_TYPE mask bits */
#define EGL_VG_ALPHA_FORMAT_PRE_BIT	0x0040	/* EGL_SURFACE_TYPE mask bits */
#define EGL_MULTISAMPLE_RESOLVE_BOX_BIT 0x0200	/* EGL_SURFACE_TYPE mask bits */
#define EGL_SWAP_BEHAVIOR_PRESERVED_BIT 0x0400	/* EGL_SURFACE_TYPE mask bits */

#define EGL_OPENGL_ES_BIT		0x0001	/* EGL_RENDERABLE_TYPE mask bits */
#define EGL_OPENVG_BIT			0x0002	/* EGL_RENDERABLE_TYPE mask bits */
#define EGL_OPENGL_ES2_BIT		0x0004	/* EGL_RENDERABLE_TYPE mask bits */
#define EGL_OPENGL_BIT			0x0008	/* EGL_RENDERABLE_TYPE mask bits */

/* QueryString targets */
#define EGL_VENDOR			0x3053
#define EGL_VERSION			0x3054
#define EGL_EXTENSIONS			0x3055
#define EGL_CLIENT_APIS			0x308D

/* QuerySurface */
#define EGL_HEIGHT 0x3056
#define EGL_WIDTH 0x3057



class TP_API tpRenderContextEGL : public tpRenderContext {
public:

	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;

	bool create(tpRenderTarget *target);
	void destroy();

	bool makeCurrent();
	bool swapBuffers();
};


class TP_API tpRenderTargetEGLPbuffer : public tpRenderTarget {
protected:
	tpVec2i mSize;
public:
	tpRenderTargetEGLPbuffer(int width, int height)
		: tpRenderTarget()
		, mSize(tpVec2i(width,height))
	{
	}

	tpInt getWidth() const { return mSize[0]; }
	tpInt getHeight() const { return mSize[1]; }

};


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
	typedef EGLSurface (__TP_CALL * eglCreatePbufferSurfaceT)(EGLDisplay display,EGLConfig config,EGLint const * attrib_list);
	typedef EGLBoolean (__TP_CALL * eglMakeCurrentT)(EGLDisplay display,EGLSurface draw,EGLSurface read,EGLContext context);
	typedef EGLBoolean (__TP_CALL * eglSwapBuffersT)(EGLDisplay display,EGLSurface surface);
	typedef EGLBoolean (__TP_CALL * eglQuerySurfaceT)(const EGLDisplay display, EGLSurface surface, EGLint attribute, EGLint * value);
	typedef EGLBoolean (__TP_CALL * eglQueryContextT)(EGLDisplay display, EGLContext surface, EGLint attribute, EGLint * value);
	typedef EGLBoolean (__TP_CALL * eglGetConfigAttribT)(EGLDisplay,EGLConfig,EGLint,EGLint * value);
	typedef EGLBoolean (__TP_CALL * eglBindAPIT)(EGLenum api);
	typedef const char* (__TP_CALL * eglQueryStringT)(EGLDisplay dpy,int);

	// functoids
	tpFunctoidImpl<eglGetErrorT> GetError;
	tpFunctoidImpl<eglInitializeT> Initialize;
	tpFunctoidImpl<eglGetDisplayT> GetDisplay;
	tpFunctoidImpl<eglGetConfigsT> GetConfigs;
	tpFunctoidImpl<eglChooseConfigT> ChooseConfig;
	tpFunctoidImpl<eglCreateContextT> CreateContext;
	tpFunctoidImpl<eglCreateWindowSurfaceT> CreateWindowSurface;
	tpFunctoidImpl<eglCreatePbufferSurfaceT> CreatePbufferSurface;
	tpFunctoidImpl<eglMakeCurrentT> MakeCurrent;
	tpFunctoidImpl<eglSwapBuffersT> SwapBuffers;
	tpFunctoidImpl<eglQuerySurfaceT> QuerySurface;
	tpFunctoidImpl<eglQueryContextT> QueryContext;
	tpFunctoidImpl<eglQueryStringT> QueryString;
	tpFunctoidImpl<eglGetConfigAttribT> GetConfigAttrib;
	tpFunctoidImpl<eglBindAPIT> BindAPI;


	static tpEGL* get(bool destroy = false);

	static tpEGL& a() { return *get(); }

protected:

	tpEGL();
};

#endif
