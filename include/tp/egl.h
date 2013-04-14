/*
 * Copyright (C) 1999-2012 Hartmut Seichter
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef TP_EGL_H
#define TP_EGL_H


#include <tp/rtl.h>
#include <tp/window.h>

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

typedef void* EGLNativeDisplayType;
typedef void* EGLNativePixmapType;
typedef void* EGLNativeWindowType;
typedef void* EGLNativeGLContext;


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
#define EGL_HEIGHT			0x3056
#define EGL_WIDTH			0x3057
#define EGL_LARGEST_PBUFFER		0x3058
#define EGL_TEXTURE_FORMAT		0x3080
#define EGL_TEXTURE_TARGET		0x3081
#define EGL_MIPMAP_TEXTURE		0x3082
#define EGL_MIPMAP_LEVEL		0x3083
#define EGL_RENDER_BUFFER		0x3086
#define EGL_VG_COLORSPACE		0x3087
#define EGL_VG_ALPHA_FORMAT		0x3088
#define EGL_HORIZONTAL_RESOLUTION	0x3090
#define EGL_VERTICAL_RESOLUTION		0x3091
#define EGL_PIXEL_ASPECT_RATIO		0x3092
#define EGL_SWAP_BEHAVIOR		0x3093
#define EGL_MULTISAMPLE_RESOLVE		0x3099

/* APIs */
#define EGL_OPENGL_ES_API		0x30A0
#define EGL_OPENVG_API			0x30A1
#define EGL_OPENGL_API			0x30A2


/* Error codes */

#define EGL_SUCCESS             0x3000
//#define EGL_NOT_INITIALIZED					= 0x3001
//#define EGL_BAD_ACCESS					= 0x3002
//#define EGL_BAD_ALLOC					= 0x3003
//#define EGL_BAD_ATTRIBUTE					= 0x3004
//#define EGL_BAD_CONFIG					= 0x3005
//#define EGL_BAD_CONTEXT					= 0x3006
//#define EGL_BAD_CURRENT_SURFACE				= 0x3007
//#define EGL_BAD_DISPLAY					= 0x3008
//#define EGL_BAD_MATCH					= 0x3009
//#define EGL_BAD_NATIVE_PIXMAP				= 0x300A
//#define EGL_BAD_NATIVE_WINDOW				= 0x300B
//#define EGL_BAD_PARAMETER					= 0x300C
//#define EGL_BAD_SURFACE					= 0x300D
//#define EGL_CONTEXT_LOST					= 0x300E




class TP_API tpEGL : public tpRuntimeLoader {
public:

	// signatures
	typedef EGLint (__TP_CALL     *  eglGetErrorT)(void);
	typedef EGLBoolean (__TP_CALL *  eglInitializeT)(EGLDisplay dpy, EGLint *major, EGLint *minor);
	typedef EGLDisplay (__TP_CALL *  eglGetDisplayT)(EGLNativeDisplayType native_display);
	typedef EGLBoolean (__TP_CALL *  eglGetConfigsT)(EGLDisplay dpy,EGLConfig* config, EGLint config_size, EGLint* num_config );
	typedef EGLBoolean (__TP_CALL *  eglChooseConfigT)(EGLDisplay display,EGLint const * attrib_list,EGLConfig * configs, EGLint config_size, EGLint * num_config);
	typedef EGLContext (__TP_CALL *  eglCreateContextT)(EGLDisplay display,EGLConfig config,EGLContext share_context,EGLint const * attrib_list);
	typedef EGLSurface (__TP_CALL *  eglCreateWindowSurfaceT)(EGLDisplay display,EGLConfig config,EGLNativeWindowType native_window,EGLint const * attrib_list);
	typedef EGLSurface (__TP_CALL *  eglCreatePbufferSurfaceT)(EGLDisplay display,EGLConfig config,EGLint const * attrib_list);
	typedef EGLBoolean (__TP_CALL *  eglMakeCurrentT)(EGLDisplay display,EGLSurface draw,EGLSurface read,EGLContext context);
	typedef EGLBoolean (__TP_CALL *  eglSwapBuffersT)(EGLDisplay display,EGLSurface surface);
	typedef EGLBoolean (__TP_CALL *  eglQuerySurfaceT)(EGLDisplay display, EGLSurface surface, EGLint attribute, EGLint * value);
	typedef EGLBoolean (__TP_CALL *  eglQueryContextT)(EGLDisplay display, EGLContext context, EGLint attribute, EGLint * value);
	typedef EGLBoolean (__TP_CALL *  eglGetConfigAttribT)(EGLDisplay,EGLConfig,EGLint,EGLint * value);
	typedef EGLBoolean (__TP_CALL *  eglBindAPIT)(EGLenum api);
	typedef const char* (__TP_CALL*  eglQueryStringT)(EGLDisplay dpy,int);
	typedef void* (__TP_CALL      *  eglGetProcAddress)(const char*);

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
	tpFunctoidImpl<eglGetProcAddress> GetProcAddress;


	static tpEGL* get(bool destroy = false);

	static tpEGL& a() { return *get(); }

protected:

	tpEGL();
};

#endif
