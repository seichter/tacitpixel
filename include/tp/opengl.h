/*
 * Twisted Pair Visualization Engine
 *
 * Copyright (c) 1999-2009 Hartmut Seichter
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the Twisted Pair License (TPL) version 1.0 or (at your option)
 * any later version. The full license text is available in the LICENSE file
 * included with this distribution, and on the technotecture.com website.
 *
 */

#ifndef TPGL_H
#define TPGL_H 1

#include <tp/globals.h>
#include <tp/types.h>

#include <tp/referenced.h>
#include <tp/refptr.h>

#include <tp/tpGLConfig.h>



#if defined(_WIN32) || defined(_WIN32_WCE)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct HDC__		*HDC;
typedef struct HBITMAP__	*HBITMAP;
typedef struct HWND__		*HWND;
typedef struct HGLRC__		*HGLRC;

typedef HDC NativeDisplayType;
typedef HBITMAP NativePixmapType;
typedef HWND NativeWindowType;
typedef HGLRC NativeGLContext;

#else

typedef void* NativeDisplayType;
typedef void* NativePixmapType;
typedef void* NativeWindowType;
typedef void* NativeGLContext;

#endif


#if defined(TPGL_USE_X11)

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#undef Cursor

typedef void *GLXContext;
typedef Window GLXDrawable;

#endif

#if defined(__APPLE__)


#include <CoreFoundation/CoreFoundation.h>

typedef struct _CGLContextObject       *CGLContextObj;
typedef struct _CGLPixelFormatObject   *CGLPixelFormatObj;
typedef struct _CGLRendererInfoObject  *CGLRendererInfoObj;
typedef struct _CGLPBufferObject       *CGLPBufferObj;
//typedef CGLError tpUInt;

#endif





#if defined(_WIN32)
#	define TP_CALLBACK __stdcall
#else
#	define TP_CALLBACK
#endif

typedef tpFloat		GLclampf;
typedef tpUInt		GLenum;
typedef tpInt		GLint;
typedef tpFloat		GLfloat;
typedef tpDouble	GLdouble;
typedef tpUByte		GLubyte;
typedef tpUInt		GLuint;
typedef tpInt		GLsizei;
typedef void		GLvoid;
typedef tpUByte		GLboolean;
typedef tpInt		GLfixed;
typedef tpChar		GLchar;

typedef tpLong		GLintptr;
typedef tpLong		GLsizeiptr;


typedef int int32_t;

typedef int32_t EGLint;
typedef unsigned int EGLBoolean;
typedef void *EGLConfig;
typedef void *EGLContext;
typedef void *EGLDisplay;
typedef void *EGLSurface;


#if defined(__cplusplus)

#include <tp/referenced.h>
#include <tp/math.h>
#include <tp/list.h>
#include <tp/library.h>



#if 1
#define TP_REPORT_GLERROR() \
	{ int err = tpGL::glGetError();\
	if (err) \
	tpLogError("%s - OpenGL error (0x%x) %d",__FUNCTION__,err,__LINE__); };
#else
#define TP_REPORT_GLERROR()
#endif


class tpGLBackend
{

	//
	tpList<tpString> m_gl;
	tpList<tpString> m_egl;
};

class tpGL {
public:

	static void setPerspective(tpReal FOV, tpReal aspect, tpReal nearDist, tpReal farDist );


	enum {
		TP_GL_PREFETCH,
		TP_GL_FULL
	};

	enum {
		TP_GL_DEFAULT,
		TP_GL_OES1_NATIVE,
		TP_GL_OES1_EMULATED,
		TP_GL_OES2_NATIVE,
		TP_GL_OES2_EMULATED,
		TP_GL_DEFERRED
	};

	/* ARB */
	static const tpUInt ARRAY_BUFFER_ARB = 0x8892;
	static const tpUInt STATIC_DRAW_ARB = 0x88E4;


	/* Boolean values */
	enum {
		 _FALSE			= 0x0,
		 _TRUE			= 0x1
	};

	/* Data types */
	enum {
		BYTE			= 0x1400,
		UNSIGNED_BYTE	= 0x1401,
		SHORT			= 0x1402,
		UNSIGNED_SHORT	= 0x1403,
		INT				= 0x1404,
		UNSIGNED_INT	= 0x1405,
		FLOAT			= 0x1406,
		_2_BYTES		= 0x1407,
		_3_BYTES		= 0x1408,
		_4_BYTES		= 0x1409,
		DOUBLE			= 0x140A,

	};

	enum {
		UNSIGNED_BYTE_3_3_2	= 0x8032,
		UNSIGNED_SHORT_4_4_4_4	= 0x8033,
		UNSIGNED_SHORT_5_5_5_1	= 0x8034,
		UNSIGNED_INT_8_8_8_8	= 0x8035,
		UNSIGNED_INT_10_10_10_2	= 0x8036,
		RESCALE_NORMAL	= 0x803A,
		UNSIGNED_BYTE_2_3_3_REV	= 0x8362,
		UNSIGNED_SHORT_5_6_5	= 0x8363,
		UNSIGNED_SHORT_5_6_5_REV	= 0x8364,
		UNSIGNED_SHORT_4_4_4_4_REV	= 0x8365,
		UNSIGNED_SHORT_1_5_5_5_REV	= 0x8366,
		UNSIGNED_INT_8_8_8_8_REV	= 0x8367,
		UNSIGNED_INT_2_10_10_10_REV	= 0x8368
	};


	enum {
		PACK_ALIGNMENT	= 0x0D05,
		UNPACK_ALIGNMENT = 0x0CF5
	};

	/* Strings */
	static tpUInt VENDOR;
	static tpUInt RENDERER;
	static tpUInt VERSION;
	static tpUInt EXTENSIONS;

	/* Lighting */
	static tpUInt LIGHTING;
	static tpUInt LIGHT0;
	static tpUInt LIGHT1;
	static tpUInt LIGHT2;
	static tpUInt LIGHT3;
	static tpUInt LIGHT4;
	static tpUInt LIGHT5;
	static tpUInt LIGHT6;
	static tpUInt LIGHT7;
	static tpUInt SPOT_EXPONENT;
	static tpUInt SPOT_CUTOFF;
	static tpUInt CONSTANT_ATTENUATION;
	static tpUInt LINEAR_ATTENUATION;
	static tpUInt QUADRATIC_ATTENUATION;
	static tpUInt AMBIENT;
	static tpUInt DIFFUSE;
	static tpUInt SPECULAR;
	static tpUInt SHININESS;
	static tpUInt EMISSION;
	static tpUInt POSITION;
	static tpUInt SPOT_DIRECTION;
	static tpUInt AMBIENT_AND_DIFFUSE;
	static tpUInt COLOR_INDEXES;
	static tpUInt LIGHT_MODEL_TWO_SIDE;
	static tpUInt LIGHT_MODEL_LOCAL_VIEWER;
	static tpUInt LIGHT_MODEL_AMBIENT;
	static tpUInt LIGHT_MODEL_COLOR_CONTROL;
	static tpUInt FRONT_AND_BACK;
	static tpUInt SHADE_MODEL;
	static tpUInt FLAT;
	static tpUInt SMOOTH;
	static tpUInt COLOR_MATERIAL;
	static tpUInt COLOR_MATERIAL_FACE;
	static tpUInt COLOR_MATERIAL_PARAMETER;
	static tpUInt NORMALIZE;

	/* Colorspace */
	static tpUInt RGB;
	static tpUInt RGBA;
	static tpUInt BGR;
	static tpUInt BGRA;
	static tpUInt LUMINANCE;

	/* Texture */
	static tpUInt TEXTURE_ENV;
	static tpUInt TEXTURE_ENV_MODE;
	static tpUInt TEXTURE_1D;
	static tpUInt TEXTURE_2D;
	static tpUInt TEXTURE_WRAP_S;
	static tpUInt TEXTURE_WRAP_T;
	static tpUInt TEXTURE_MAG_FILTER;
	static tpUInt TEXTURE_MIN_FILTER;
	static tpUInt TEXTURE_ENV_COLOR;
	static tpUInt TEXTURE_GEN_S;
	static tpUInt TEXTURE_GEN_T;
	static tpUInt TEXTURE_GEN_MODE;
	static tpUInt TEXTURE_BORDER_COLOR;
	static tpUInt TEXTURE_WIDTH;
	static tpUInt TEXTURE_HEIGHT;
	static tpUInt TEXTURE_BORDER;
	static tpUInt TEXTURE_COMPONENTS;
	static tpUInt TEXTURE_RED_SIZE;
	static tpUInt TEXTURE_GREEN_SIZE;
	static tpUInt TEXTURE_BLUE_SIZE;
	static tpUInt TEXTURE_ALPHA_SIZE;
	static tpUInt TEXTURE_LUMINANCE_SIZE;
	static tpUInt TEXTURE_INTENSITY_SIZE;
	static tpUInt NEAREST_MIPMAP_NEAREST;
	static tpUInt NEAREST_MIPMAP_LINEAR;
	static tpUInt LINEAR_MIPMAP_NEAREST;
	static tpUInt LINEAR_MIPMAP_LINEAR;
	static tpUInt OBJECT_LINEAR;
	static tpUInt OBJECT_PLANE;
	static tpUInt EYE_LINEAR;
	static tpUInt EYE_PLANE;
	static tpUInt SPHERE_MAP;
	static tpUInt DECAL	;
	static tpUInt MODULATE;
	static tpUInt NEAREST;
	static tpUInt REPEAT;
	static tpUInt CLAMP;
	static tpUInt S;
	static tpUInt T;
	static tpUInt R;
	static tpUInt Q;
	static tpUInt TEXTURE_GEN_R;
	static tpUInt TEXTURE_GEN_Q;

	/* Fog */
	static tpUInt FOG;
	static tpUInt FOG_MODE;
	static tpUInt FOG_DENSITY;
	static tpUInt FOG_COLOR;
	static tpUInt FOG_INDEX;
	static tpUInt FOG_START;
	static tpUInt FOG_END	;
	static tpUInt LINEAR;
	static tpUInt EXP;
	static tpUInt EXP2;

	/* Logic Ops */
	static tpUInt LOGIC_OP;
	static tpUInt INDEX_LOGIC_OP;
	static tpUInt COLOR_LOGIC_OP;
	static tpUInt LOGIC_OP_MODE;
	static tpUInt CLEAR;
	static tpUInt SET;
	static tpUInt COPY;
	static tpUInt COPY_INVERTED;
	static tpUInt NOOP;
	static tpUInt INVERT;
	static tpUInt AND	;
	static tpUInt NAND;
	static tpUInt OR;
	static tpUInt NOR;
	static tpUInt XOR;
	static tpUInt EQUIV;
	static tpUInt AND_REVERSE;
	static tpUInt AND_INVERTED;
	static tpUInt OR_REVERSE;
	static tpUInt OR_INVERTED;



	/* Primitives */
	static tpUInt POINTS;
	static tpUInt LINES;
	static tpUInt LINE_LOOP;
	static tpUInt LINE_STRIP;
	static tpUInt TRIANGLES;
	static tpUInt TRIANGLE_STRIP;
	static tpUInt TRIANGLE_FAN;
	static tpUInt QUADS;
	static tpUInt QUAD_STRIP;
	static tpUInt POLYGON;

	/* Vertex Arrays */
	static tpUInt VERTEX_ARRAY;
	static tpUInt NORMAL_ARRAY;
	static tpUInt COLOR_ARRAY;
	static tpUInt INDEX_ARRAY;
	static tpUInt TEXTURE_COORD_ARRAY;
	static tpUInt EDGE_FLAG_ARRAY;
	static tpUInt VERTEX_ARRAY_SIZE;
	static tpUInt VERTEX_ARRAY_TYPE;
	static tpUInt VERTEX_ARRAY_STRIDE;
	static tpUInt NORMAL_ARRAY_TYPE;
	static tpUInt NORMAL_ARRAY_STRIDE;
	static tpUInt COLOR_ARRAY_SIZE;
	static tpUInt COLOR_ARRAY_TYPE;
	static tpUInt COLOR_ARRAY_STRIDE;
	static tpUInt INDEX_ARRAY_TYPE;
	static tpUInt INDEX_ARRAY_STRIDE;
	static tpUInt TEXTURE_COORD_ARRAY_SIZE;
	static tpUInt TEXTURE_COORD_ARRAY_TYPE;
	static tpUInt TEXTURE_COORD_ARRAY_STRIDE;
	static tpUInt EDGE_FLAG_ARRAY_STRIDE;
	static tpUInt VERTEX_ARRAY_POINTER;
	static tpUInt NORMAL_ARRAY_POINTER;
	static tpUInt COLOR_ARRAY_POINTER;
	static tpUInt INDEX_ARRAY_POINTER;
	static tpUInt TEXTURE_COORD_ARRAY_POINTER;
	static tpUInt EDGE_FLAG_ARRAY_POINTER;
	static tpUInt V2F;
	static tpUInt V3F;
	static tpUInt C4UB_V2F;
	static tpUInt C4UB_V3F;
	static tpUInt C3F_V3F;
	static tpUInt N3F_V3F;
	static tpUInt C4F_N3F_V3F;
	static tpUInt T2F_V3F;
	static tpUInt T4F_V4F;
	static tpUInt T2F_C4UB_V3F;
	static tpUInt T2F_C3F_V3F;
	static tpUInt T2F_N3F_V3F;
	static tpUInt T2F_C4F_N3F_V3F;
	static tpUInt T4F_C4F_N3F_V4F;

	/* Matrix Mode */
	static tpUInt MATRIX_MODE;
	static tpUInt MODELVIEW;
	static tpUInt PROJECTION;
	static tpUInt TEXTURE;

	/* Points */
	static tpUInt POINT_SMOOTH;
	static tpUInt POINT_SIZE;
	static tpUInt POINT_SIZE_GRANULARITY;
	static tpUInt POINT_SIZE_RANGE;

	/* Lines */
	static tpUInt LINE_SMOOTH;
	static tpUInt LINE_STIPPLE;
	static tpUInt LINE_STIPPLE_PATTERN;
	static tpUInt LINE_STIPPLE_REPEAT;
	static tpUInt LINE_WIDTH;
	static tpUInt LINE_WIDTH_GRANULARITY;
	static tpUInt LINE_WIDTH_RANGE;

	/* Polygons */
	static tpUInt POINT;
	static tpUInt LINE;
	static tpUInt FILL;
	static tpUInt CW;
	static tpUInt CCW;
	static tpUInt FRONT;
	static tpUInt BACK;
	static tpUInt POLYGON_MODE;
	static tpUInt POLYGON_SMOOTH;
	static tpUInt POLYGON_STIPPLE;
	static tpUInt EDGE_FLAG;
	static tpUInt CULL_FACE;
	static tpUInt CULL_FACE_MODE;
	static tpUInt FRONT_FACE	;
	static tpUInt POLYGON_OFFSET_FACTOR;
	static tpUInt POLYGON_OFFSET_UNITS;
	static tpUInt POLYGON_OFFSET_POINT;
	static tpUInt POLYGON_OFFSET_LINE;
	static tpUInt POLYGON_OFFSET_FILL;



	/* AlphaFunction */
	enum tpGL_ALPHA_FUNCTION {
		NEVER                          = 0x0200,
		LESS                           = 0x0201,
		EQUAL                          = 0x0202,
		LEQUAL                         = 0x0203,
		GREATER                        = 0x0204,
		NOTEQUAL                       = 0x0205,
		GEQUAL                         = 0x0206,
		ALWAYS                         = 0x0207
	};

	/* AttribMask */
	enum tpGL_ATTRIBUTE_MASK {
		CURRENT_BIT                    = 0x00000001,
		POINT_BIT                      = 0x00000002,
		LINE_BIT                       = 0x00000004,
		POLYGON_BIT                    = 0x00000008,
		POLYGON_STIPPLE_BIT            = 0x00000010,
		PIXEL_MODE_BIT                 = 0x00000020,
		LIGHTING_BIT                   = 0x00000040,
		FOG_BIT                        = 0x00000080,
		DEPTH_BUFFER_BIT               = 0x00000100,
		ACCUM_BUFFER_BIT               = 0x00000200,
		STENCIL_BUFFER_BIT             = 0x00000400,
		VIEWPORT_BIT                   = 0x00000800,
		TRANSFORM_BIT                  = 0x00001000,
		ENABLE_BIT                     = 0x00002000,
		COLOR_BUFFER_BIT               = 0x00004000,
		HINT_BIT                       = 0x00008000,
		EVAL_BIT                       = 0x00010000,
		LIST_BIT                       = 0x00020000,
		TEXTURE_BIT                    = 0x00040000,
		SCISSOR_BIT                    = 0x00080000,
		ALL_ATTRIB_BITS                = 0x000fffff
	};

	/* random other things */
	static const GLenum AUTO_NORMAL = 0x0D80;
	static const GLenum DEPTH_TEST = 0x0B71;



	static EGLint     (TP_CALLBACK * eglGetError)(void);
	static EGLBoolean (TP_CALLBACK * eglInitialize)(EGLDisplay dpy, EGLint *major, EGLint *minor);
	static EGLDisplay (TP_CALLBACK * eglGetDisplay)(NativeDisplayType native_display);
	static EGLBoolean (TP_CALLBACK * eglGetConfigs)(EGLDisplay dpy,EGLConfig* config, EGLint config_size, EGLint* num_config );
	static EGLBoolean (TP_CALLBACK * eglChooseConfig)(EGLDisplay display,EGLint const * attrib_list,EGLConfig * configs, EGLint config_size, EGLint * num_config);
	static EGLContext (TP_CALLBACK * eglCreateContext)(EGLDisplay display,EGLConfig config,EGLContext share_context,EGLint const * attrib_list);
	static EGLSurface (TP_CALLBACK * eglCreateWindowSurface)(EGLDisplay display,EGLConfig config,NativeWindowType native_window,EGLint const * attrib_list);
	static EGLBoolean (TP_CALLBACK * eglMakeCurrent)(EGLDisplay display,EGLSurface draw,EGLSurface read,EGLContext context);
	static EGLBoolean (TP_CALLBACK * eglSwapBuffers)(EGLDisplay display,EGLSurface surface);
	static const char* (TP_CALLBACK * eglQueryString)(EGLDisplay dpy,GLint);
	static EGLBoolean (TP_CALLBACK * eglQuerySurface)(const EGLDisplay display, EGLSurface surface, EGLint attribute, EGLint * value);

	static void (TP_CALLBACK *	Begin)(GLenum);
	static void (TP_CALLBACK *	End)();

	static void (TP_CALLBACK *	ClearColor)(GLclampf red, GLclampf  green, GLclampf  blue, GLclampf  alpha);
	static void (TP_CALLBACK *	Clear)(GLenum bits);
	static void (TP_CALLBACK *	Enable)(GLenum );
	static void (TP_CALLBACK *	Disable)(GLenum );
	static void (TP_CALLBACK *	Flush)(void);
	static void (TP_CALLBACK *	Finish)(void);
	static void (TP_CALLBACK *	ShadeModel)(GLenum );

	static void	(TP_CALLBACK *	MatrixMode)(GLenum);
	static void	(TP_CALLBACK *	LoadIdentity)(void);

	static void	(TP_CALLBACK *	LoadMatrixd)(const double *);
	static void	(TP_CALLBACK *	LoadMatrixf)(const float *);

	static void	(TP_CALLBACK *	MultMatrixd)(const double*);
	static void	(TP_CALLBACK *	MultMatrixf)(const float *);

	static void (TP_CALLBACK *	PushMatrix)(void);
	static void (TP_CALLBACK *	PopMatrix)(void);

	static void (TP_CALLBACK *  Materialf)(GLenum, GLenum, GLfloat );
	static void (TP_CALLBACK *  Materialfv)(GLenum, GLenum, const GLfloat *);
	static void (TP_CALLBACK *  Materiali)(GLenum, GLenum, GLint );
	static void (TP_CALLBACK *  Materialiv)(GLenum, GLenum, const GLint *);

	static void	(TP_CALLBACK *	TexCoord2d)(GLdouble,GLdouble);
	static void (TP_CALLBACK *	TexCoord2dv)(const GLdouble*);
	static void	(TP_CALLBACK *	TexCoord2f)(GLfloat,GLfloat);
	static void (TP_CALLBACK *	TexCoord2fv)(const GLfloat*);

	static tpUByte (TP_CALLBACK *	IsTexture)(GLuint);
	static void    (TP_CALLBACK *	GenTextures)(GLsizei,GLuint*);
	static void    (TP_CALLBACK *	BindTexture)(GLsizei,GLuint);
	static void    (TP_CALLBACK *	TexImage2D)   (GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*);
	static void    (TP_CALLBACK *	TexSubImage2D)(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,const GLvoid*);

	static void    (TP_CALLBACK *	TexParameterf)(GLenum,GLenum,GLfloat);
	static void    (TP_CALLBACK *	TexParameteri)(GLenum,GLenum,GLint);
	static void    (TP_CALLBACK *	TexParameterfv)(GLenum,GLenum,const GLfloat*);
	static void    (TP_CALLBACK *	TexParameteriv)(GLenum,GLenum,const GLint*);

	static const GLubyte* (TP_CALLBACK * GetString)(GLenum);

	static void    (TP_CALLBACK *	LightModelf)(GLenum,GLfloat);
	static void    (TP_CALLBACK *	LightModelfv)(GLenum,const GLfloat*);
	static void    (TP_CALLBACK *	LightModeli)(GLenum,GLint);
	static void    (TP_CALLBACK *	LightModeliv)(GLenum,const GLint*);

	static void    (TP_CALLBACK *	Lightf)(GLenum,GLenum,GLfloat);
	static void    (TP_CALLBACK *	Lightfv)(GLenum,GLenum,const GLfloat*);
	static void    (TP_CALLBACK *	Lighti)(GLenum,GLenum,GLint);
	static void    (TP_CALLBACK *	Lightiv)(GLenum,GLenum,const GLint*);

	static void    (TP_CALLBACK *	Normal3dv)(const GLdouble *);
	static void    (TP_CALLBACK *	Vertex3dv)(const GLdouble *);


	/* Viewport / Projection */
	static void	   (TP_CALLBACK *	Frustum)(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble);
	static void	   (TP_CALLBACK *	Frustumf)(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
	static void	   (TP_CALLBACK *	Frustumx)(GLfixed,GLfixed,GLfixed,GLfixed,GLfixed,GLfixed);
	static void    (TP_CALLBACK *	Viewport)(GLint,GLint,GLsizei,GLsizei);

	/* OpenGL 1.1 or later */
	static void	   (TP_CALLBACK *  EnableClientState)(GLenum);
	static void	   (TP_CALLBACK *  DisableClientState)(GLenum);

	static void	   (TP_CALLBACK *  NormalPointer)(GLenum,GLsizei,const GLvoid*);
	static void	   (TP_CALLBACK *  TexCoordPointer)(GLint,GLenum,GLsizei,const GLvoid*);
	static void	   (TP_CALLBACK *  VertexPointer)(GLint,GLenum,GLsizei,const GLvoid*);
	static void	   (TP_CALLBACK *  ColorPointer)(GLint,GLenum,GLsizei,const GLvoid*);
	static void	   (TP_CALLBACK *  DrawElements)(GLenum,GLsizei,GLenum,const GLvoid*);
	static void	   (TP_CALLBACK *  DrawArrays)(GLenum,GLint,GLsizei);

	static void	   (TP_CALLBACK *  PixelStorei )(GLenum,GLint);

	static void (TP_CALLBACK * BindBufferARB) (GLenum target, GLuint buffer);
	static void (TP_CALLBACK * DeleteBufferARB) (GLsizei n, const GLuint *buffers);
	static void (TP_CALLBACK * GenBufferARB) (GLsizei n, GLuint *buffers);
	static void (TP_CALLBACK * DataBufferARB) (GLenum target, int size, const GLvoid *data, GLenum usage);


	//////////////////////////////////////////////////////////////////////////
	// OpenGL 3.x and OpenGL ES 2.0 profile
	//////////////////////////////////////////////////////////////////////////
#define TP_DEF_GLFUNC(name) \
	static name##Func name;

#define TP_DEC_GLFUNC(name,ret)\
	typedef ret			(TP_CALLBACK *name##Func)

	// Generic
	TP_DEC_GLFUNC(glGetError,GLenum)(void);
	TP_DEC_GLFUNC(glGetBooleanv,void)(GLenum,GLboolean*);
	TP_DEC_GLFUNC(glGetFloatv,void)(GLenum,GLfloat*);
	TP_DEC_GLFUNC(glGetIntegerv,void)(GLenum,GLint*);

	TP_DEF_GLFUNC(glGetError);
	TP_DEF_GLFUNC(glGetBooleanv)
	TP_DEF_GLFUNC(glGetFloatv)
	TP_DEF_GLFUNC(glGetIntegerv)

	TP_DEC_GLFUNC(glActiveTexture,void)(GLenum);
	TP_DEF_GLFUNC(glActiveTexture);



	// Uniforms
	TP_DEC_GLFUNC(glGetUniformLocation,GLint)(GLuint,const GLchar*);
	TP_DEC_GLFUNC(glGetAttribLocation,GLint)(GLuint,const GLchar*);

	TP_DEC_GLFUNC(glUniform1f,void)(GLint,GLfloat);
	TP_DEC_GLFUNC(glUniform2f,void)(GLint,GLfloat,GLfloat);
	TP_DEC_GLFUNC(glUniform3f,void)(GLint,GLfloat,GLfloat,GLfloat);
	TP_DEC_GLFUNC(glUniform4f,void)(GLint,GLfloat,GLfloat,GLfloat,GLfloat);
	TP_DEC_GLFUNC(glUniform1i,void)(GLint,GLint);
	TP_DEC_GLFUNC(glUniform2i,void)(GLint,GLint,GLint);
	TP_DEC_GLFUNC(glUniform3i,void)(GLint,GLint,GLint,GLint);
	TP_DEC_GLFUNC(glUniform4i,void)(GLint,GLint,GLint,GLint,GLint);

	TP_DEC_GLFUNC(glUniform1fv,void)(GLint,GLsizei, const GLfloat *);
	TP_DEC_GLFUNC(glUniform2fv,void)(GLint,GLsizei, const GLfloat *);
	TP_DEC_GLFUNC(glUniform3fv,void)(GLint,GLsizei, const GLfloat *);
	TP_DEC_GLFUNC(glUniform4fv,void)(GLint,GLsizei, const GLfloat *);
	TP_DEC_GLFUNC(glUniform1iv,void)(GLint,GLsizei, const GLint *);
	TP_DEC_GLFUNC(glUniform2iv,void)(GLint,GLsizei, const GLint *);
	TP_DEC_GLFUNC(glUniform3iv,void)(GLint,GLsizei, const GLint *);
	TP_DEC_GLFUNC(glUniform4iv,void)(GLint,GLsizei, const GLint *);

	TP_DEC_GLFUNC(glUniformMatrix2fv,void)(GLint,GLsizei,GLboolean,const GLfloat*);
	TP_DEC_GLFUNC(glUniformMatrix3fv,void)(GLint,GLsizei,GLboolean,const GLfloat*);
	TP_DEC_GLFUNC(glUniformMatrix4fv,void)(GLint,GLsizei,GLboolean,const GLfloat*);

	TP_DEF_GLFUNC(glGetUniformLocation)
	TP_DEF_GLFUNC(glGetAttribLocation)
	TP_DEF_GLFUNC(glUniform1f)
	TP_DEF_GLFUNC(glUniform2f)
	TP_DEF_GLFUNC(glUniform3f)
	TP_DEF_GLFUNC(glUniform4f)
	TP_DEF_GLFUNC(glUniform1i)
	TP_DEF_GLFUNC(glUniform2i)
	TP_DEF_GLFUNC(glUniform3i)
	TP_DEF_GLFUNC(glUniform4i)
	TP_DEF_GLFUNC(glUniform1fv)
	TP_DEF_GLFUNC(glUniform2fv)
	TP_DEF_GLFUNC(glUniform3fv)
	TP_DEF_GLFUNC(glUniform4fv)
	TP_DEF_GLFUNC(glUniform1iv)
	TP_DEF_GLFUNC(glUniform2iv)
	TP_DEF_GLFUNC(glUniform3iv)
	TP_DEF_GLFUNC(glUniform4iv)
	TP_DEF_GLFUNC(glUniformMatrix2fv)
	TP_DEF_GLFUNC(glUniformMatrix3fv)
	TP_DEF_GLFUNC(glUniformMatrix4fv)

	// Shader
	TP_DEC_GLFUNC(glCreateShader,GLuint)(GLenum);
	TP_DEC_GLFUNC(glShaderSource,void)  (GLuint shader,GLsizei count, const GLchar** string, const GLint * length);
	TP_DEC_GLFUNC(glCompileShader,void) (GLuint);
	TP_DEC_GLFUNC(glGetShaderiv,void) (GLuint shader,GLenum pname, GLint *params);
	TP_DEC_GLFUNC(glGetShaderInfoLog,void)(GLuint shader, GLsizei maxLength, GLsizei * length, GLchar* infoLog);
	TP_DEC_GLFUNC(glIsShader,GLboolean)(GLuint);

	TP_DEC_GLFUNC(glDeleteShader,GLuint)(GLuint);
	TP_DEC_GLFUNC(glShaderBinary,void)(GLsizei n,const GLuint *shaders, GLenum binaryformat,const void *binary, GLsizei length);

	TP_DEF_GLFUNC(glCreateShader)
	TP_DEF_GLFUNC(glShaderSource)
	TP_DEF_GLFUNC(glCompileShader)
	TP_DEF_GLFUNC(glGetShaderiv)
	TP_DEF_GLFUNC(glGetShaderInfoLog)
	TP_DEF_GLFUNC(glIsShader)
	TP_DEF_GLFUNC(glDeleteShader)
	TP_DEF_GLFUNC(glShaderBinary)


	// Program
	TP_DEC_GLFUNC(glCreateProgram,GLuint)();
	TP_DEC_GLFUNC(glAttachShader,void)(GLuint program, GLuint shader);
	TP_DEC_GLFUNC(glLinkProgram,void)(GLuint);
	TP_DEC_GLFUNC(glBindAttribLocation,void)(GLuint program,GLuint index, const GLchar* name);
	TP_DEC_GLFUNC(glGetProgramiv,void)(GLuint program, GLenum pname, GLint *params);
	TP_DEC_GLFUNC(glGetProgramInfoLog,void)(GLuint program, GLsizei maxLength, GLsizei * , GLchar* infoLog);
	TP_DEC_GLFUNC(glUseProgram,GLuint)(GLuint);
	TP_DEC_GLFUNC(glDeleteProgram,GLuint)(GLuint);
	TP_DEC_GLFUNC(glIsProgram,GLboolean)(GLuint);


	TP_DEF_GLFUNC(glCreateProgram);
	TP_DEF_GLFUNC(glAttachShader);
	TP_DEF_GLFUNC(glLinkProgram);
	TP_DEF_GLFUNC(glBindAttribLocation);
	TP_DEF_GLFUNC(glGetProgramiv);
	TP_DEF_GLFUNC(glGetProgramInfoLog);
	TP_DEF_GLFUNC(glUseProgram);
	TP_DEF_GLFUNC(glDeleteProgram);
	TP_DEF_GLFUNC(glIsProgram)

	// Vertices
	TP_DEC_GLFUNC(glVertexAttribPointer,GLuint)( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);
	TP_DEC_GLFUNC(glEnableVertexAttribArray,void)( GLuint index );
	TP_DEC_GLFUNC(glDisableVertexAttribArray,void)( GLuint index );

	TP_DEF_GLFUNC(glVertexAttribPointer);
	TP_DEF_GLFUNC(glEnableVertexAttribArray);
	TP_DEF_GLFUNC(glDisableVertexAttribArray);


	// ES functions
	TP_DEC_GLFUNC(glFrontFace,void)(GLenum);
	TP_DEF_GLFUNC(glFrontFace);


	// VBO/PBO support
	TP_DEC_GLFUNC(glGenBuffers,void)(GLsizei n, GLuint* buffers);
	TP_DEC_GLFUNC(glBindBuffer,void)(GLenum target, GLuint buffer);
	TP_DEC_GLFUNC(glDeleteBuffers,void)(GLsizei n, const GLuint* buffers);
	TP_DEC_GLFUNC(glIsBuffer,GLboolean)(GLuint buffer);
	TP_DEC_GLFUNC(glBufferData,void)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);

	TP_DEF_GLFUNC(glGenBuffers);
	TP_DEF_GLFUNC(glBindBuffer);
	TP_DEF_GLFUNC(glDeleteBuffers);
	TP_DEF_GLFUNC(glIsBuffer);
	TP_DEF_GLFUNC(glBufferData);


	// z-Buffer related
	TP_DEC_GLFUNC(glDepthFunc,void)(GLenum);
	TP_DEC_GLFUNC(glDepthRange,void)(GLclampf n,GLclampf f);

	TP_DEF_GLFUNC(glDepthFunc);
	TP_DEF_GLFUNC(glDepthRange);




	TP_DEC_GLFUNC(tpGetProcAddress,void*)(const GLchar* name);
	TP_DEF_GLFUNC(tpGetProcAddress);


	static bool hasVBO();

#if defined(__APPLE__)

	//static CGLError (TP_CALLBACK * CGLCreateContext)(CGLPixelFormatObj,CGLContextObj,CGLContextObj*);
	//static const char* (TP_CALLBACK * CGLErrorString)(CGLError);

#endif

#if defined(WIN32)
//	static HGLRC   (TP_CALLBACK *	wglCreateContext)(HDC);
	static HGLRC   (TP_CALLBACK *	wglCreateContext)(HDC);
	static BOOL	   (TP_CALLBACK *	wglDeleteContext)(HGLRC);
	static BOOL	   (TP_CALLBACK *	MakeCurrent)(HDC,HGLRC);
	static BOOL	   (TP_CALLBACK *	SwapBuffers)(HDC);
#endif

#if defined(TPGL_USE_X11)
	static XVisualInfo* (TP_CALLBACK* glXQueryExtension)(Display *dpy, int screen, int *attribList );
	static XVisualInfo* (TP_CALLBACK* glXChooseVisual)( Display *dpy, int screen, int *attribList );
	static Bool         (TP_CALLBACK* glXQueryVersion)( Display *dpy, int *major, int *minor );
	static GLXContext   (TP_CALLBACK* glXCreateContext)( Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct );
	static void         (TP_CALLBACK* glXDestroyContext)( Display *dpy, GLXContext ctx );
	static Bool         (TP_CALLBACK* glXMakeCurrent)( Display *dpy, GLXDrawable drawable, GLXContext ctx );
	static void         (TP_CALLBACK* glXSwapBuffers)( Display *dpy, GLXDrawable drawable );
#endif

	//static tpString getVendor();
	//static tpString getVersion();
	//static tpString getRenderer();
	//static tpString getExtensions();

	bool load ( tpUInt stage = TP_GL_PREFETCH );
	void release();

	bool hasEGL();
	bool isValid() const;

	bool hasShaderSupport();
	bool hasShaderCompiler();

	bool hasShaderBinarySupport();

	static tpGL& get();

protected:

	tpGL(unsigned int mode = TP_GL_DEFAULT);
	virtual ~tpGL();

	static tpRefPtr<tpLibrary> mg_egl;
	static tpRefPtr<tpLibrary> mg_ogl;

	void loadLibraries();

	void loadGL();

	void loadOGL();
	void loadEGL();

	void* getAddress( const tpString& name, const tpString& altname = "");
};


struct tpGLRenderSurfaceContext {

	void *native_display;
	void *native_window;
	void *native_context;
	void *native_screen;

	void *egl_display;
	void *egl_surface;
	void *egl_context;



};


#endif

#endif

