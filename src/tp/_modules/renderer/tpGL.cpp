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
#include "tpGL.h"
#include <tp/library.h>
#include <tp/log.h>

tpRefPtr<tpLibrary> tpGL::mg_egl = 0;
tpRefPtr<tpLibrary> tpGL::mg_ogl = 0;

/*static*/ void (TP_CALLBACK * tpGL::Begin)(GLenum)= 0;
/*static*/ void (TP_CALLBACK * tpGL::End)()= 0;

/*static*/ void (TP_CALLBACK * tpGL::ClearColor)(GLclampf red, GLclampf  green, GLclampf  blue, GLclampf  alpha)= 0;
/*static*/ void (TP_CALLBACK * tpGL::Clear)(GLenum bits)= 0;
/*static*/ void (TP_CALLBACK * tpGL::Enable)(GLenum )= 0;
/*static*/ void (TP_CALLBACK * tpGL::Disable)(GLenum )= 0;
/*static*/ void (TP_CALLBACK * tpGL::Flush)(void)= 0;
/*static*/ void (TP_CALLBACK * tpGL::Finish)(void)= 0;
/*static*/ void (TP_CALLBACK * tpGL::ShadeModel)(GLenum )= 0;

/*static*/ void(TP_CALLBACK * tpGL::MatrixMode)(GLenum)= 0;
/*static*/ void(TP_CALLBACK * tpGL::LoadIdentity)(void)= 0;

/*static*/ void(TP_CALLBACK * tpGL::LoadMatrixd)(const double *)= 0;
/*static*/ void(TP_CALLBACK * tpGL::LoadMatrixf)(const float *)= 0;

/*static*/ void(TP_CALLBACK * tpGL::MultMatrixd)(const double*)= 0;
/*static*/ void(TP_CALLBACK * tpGL::MultMatrixf)(const float *)= 0;

/*static*/ void (TP_CALLBACK * tpGL::PushMatrix)(void)= 0;
/*static*/ void (TP_CALLBACK * tpGL::PopMatrix)(void)= 0;  

/*static*/ void (TP_CALLBACK * tpGL::  Materialf)(GLenum, GLenum, GLfloat )= 0;
/*static*/ void (TP_CALLBACK * tpGL::  Materialfv)(GLenum, GLenum, const GLfloat *)= 0;
/*static*/ void (TP_CALLBACK * tpGL::  Materiali)(GLenum, GLenum, GLint )= 0;
/*static*/ void (TP_CALLBACK * tpGL::  Materialiv)(GLenum, GLenum, const GLint *)= 0;

/*static*/ void(TP_CALLBACK * tpGL::TexCoord2d)(GLdouble,GLdouble)= 0;
/*static*/ void (TP_CALLBACK * tpGL::TexCoord2dv)(const GLdouble*) = 0;
/*static*/ void(TP_CALLBACK * tpGL::TexCoord2f)(GLfloat,GLfloat)= 0;
/*static*/ void (TP_CALLBACK * tpGL::TexCoord2fv)(const GLfloat*) = 0;

/*static*/ tpUByte (TP_CALLBACK * tpGL::IsTexture)(GLuint)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::GenTextures)(GLsizei,GLuint*)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::BindTexture)(GLsizei,GLuint)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::TexImage2D)   (GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::TexSubImage2D)(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,const GLvoid*)= 0;

/*static*/ void    (TP_CALLBACK * tpGL::TexParameterf)(GLenum,GLenum,GLfloat)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::TexParameteri)(GLenum,GLenum,GLint)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::TexParameterfv)(GLenum,GLenum,const GLfloat*)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::TexParameteriv)(GLenum,GLenum,const GLint*)= 0;

/*static*/ const GLubyte* (TP_CALLBACK * tpGL:: GetString)(GLenum) = 0;

/*static*/ void    (TP_CALLBACK * tpGL::LightModelf)(GLenum,GLfloat)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::LightModelfv)(GLenum,const GLfloat*)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::LightModeli)(GLenum,GLint)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::LightModeliv)(GLenum,const GLint*)= 0;

/*static*/ void    (TP_CALLBACK * tpGL::Lightf)(GLenum,GLenum,GLfloat)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::Lightfv)(GLenum,GLenum,const GLfloat*)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::Lighti)(GLenum,GLenum,GLint)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::Lightiv)(GLenum,GLenum,const GLint*)= 0;

/*static*/ void    (TP_CALLBACK * tpGL::Normal3dv)(const GLdouble *)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::Vertex3dv)(const GLdouble *)= 0;

/*static*/ void    (TP_CALLBACK * tpGL::Viewport)(GLint,GLint,GLsizei,GLsizei)= 0;
/*static*/ void    (TP_CALLBACK * tpGL::Frustum)(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble) = 0;
/*static*/ void    (TP_CALLBACK * tpGL::Frustumf)(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat) = 0;
/*static*/ void    (TP_CALLBACK * tpGL::Frustumx)(GLfixed,GLfixed,GLfixed,GLfixed,GLfixed,GLfixed) = 0;

/*static*/ void	   (TP_CALLBACK * tpGL::EnableClientState)(GLenum) = 0;
/*static*/ void	   (TP_CALLBACK * tpGL::DisableClientState)(GLenum) = 0;

/*static*/ void	   (TP_CALLBACK * tpGL::NormalPointer)(GLenum,GLsizei,const GLvoid*) = 0;
/*static*/ void	   (TP_CALLBACK * tpGL::TexCoordPointer)(GLint,GLenum,GLsizei,const GLvoid*) = 0;
/*static*/ void	   (TP_CALLBACK * tpGL::VertexPointer)(GLint,GLenum,GLsizei,const GLvoid*) = 0;
/*static*/ void	   (TP_CALLBACK * tpGL::DrawElements)(GLenum,GLsizei,GLenum,const GLvoid*) = 0;
/*static*/ void	   (TP_CALLBACK * tpGL::DrawArrays)(GLenum,GLint,GLsizei) = 0;

/*static*/ void	   (TP_CALLBACK * tpGL::PixelStorei )(GLenum,GLint) = 0;

#if defined(WIN32)
/*static*/ HGLRC   (TP_CALLBACK * tpGL::wglCreateContext)(HDC) = 0;
/*static*/ BOOL	   (TP_CALLBACK * tpGL::wglDeleteContext)(HGLRC) = 0;
/*static*/ BOOL	   (TP_CALLBACK * tpGL::MakeCurrent)(HDC,HGLRC) = 0;
/*static*/ BOOL	   (TP_CALLBACK * tpGL::SwapBuffers)(HDC) = 0;
#endif


#if defined(__unix) || defined(__APPLE__)
XVisualInfo* (TP_CALLBACK* tpGL::glXQueryExtension)(Display *dpy, int screen, int *attribList ) = 0;
XVisualInfo* (TP_CALLBACK* tpGL::glXChooseVisual)( Display *dpy, int screen, int *attribList )  = 0;
Bool         (TP_CALLBACK* tpGL::glXQueryVersion)( Display *dpy, int *major, int *minor )  = 0;
GLXContext   (TP_CALLBACK* tpGL::glXCreateContext)( Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct )  = 0;
void         (TP_CALLBACK* tpGL::glXDestroyContext)( Display *dpy, GLXContext ctx )  = 0;
Bool         (TP_CALLBACK* tpGL::glXMakeCurrent)( Display *dpy, GLXDrawable drawable, GLXContext ctx )  = 0;
void         (TP_CALLBACK* tpGL::glXSwapBuffers)( Display *dpy, GLXDrawable drawable ) = 0;
#endif


/*static*/ const char* (TP_CALLBACK * tpGL::eglQueryString)(EGLDisplay,GLint) = 0;
/*static*/ EGLBoolean (TP_CALLBACK * tpGL::eglQuerySurface)(const EGLDisplay display, EGLSurface surface, EGLint attribute, EGLint * value) = 0;
/*static*/ EGLint     (TP_CALLBACK * tpGL::eglGetError)(void) = 0;
/*static*/ EGLBoolean (TP_CALLBACK * tpGL::eglInitialize)(EGLDisplay dpy, EGLint *major, EGLint *minor) = 0;
/*static*/ EGLDisplay (TP_CALLBACK * tpGL::eglGetDisplay)(NativeDisplayType native_display) = 0;
/*static*/ EGLBoolean (TP_CALLBACK * tpGL::eglGetConfigs)(EGLDisplay dpy,EGLConfig* config, EGLint config_size, EGLint* num_config ) = 0;
/*static*/ EGLBoolean (TP_CALLBACK * tpGL::eglChooseConfig)(EGLDisplay display,EGLint const * attrib_list,EGLConfig * configs, EGLint config_size, EGLint * num_config) = 0;
/*static*/ EGLContext (TP_CALLBACK * tpGL::eglCreateContext)(EGLDisplay display,EGLConfig config,EGLContext share_context,EGLint const * attrib_list) = 0;
/*static*/ EGLSurface (TP_CALLBACK * tpGL::eglCreateWindowSurface)(EGLDisplay display,EGLConfig config,NativeWindowType native_window,EGLint const * attrib_list) = 0;
/*static*/ EGLBoolean (TP_CALLBACK * tpGL::eglMakeCurrent)(EGLDisplay display,EGLSurface draw,EGLSurface read,EGLContext context) = 0;
/*static*/ EGLBoolean (TP_CALLBACK * tpGL::eglSwapBuffers)(EGLDisplay display,EGLSurface surface) = 0;



// Shader
/* static */ GLuint (TP_CALLBACK *tpGL::glCreateShader) (GLenum shaderType) = 0;
/* static */ void   (TP_CALLBACK *tpGL::glShaderSource) (GLuint shader,GLsizei count, const GLchar** string, const GLint * 	length) = 0;
/* static */ void   (TP_CALLBACK *tpGL::glCompileShader) (GLuint) = 0;
/* static */ void   (TP_CALLBACK *tpGL::glGetShaderiv) (GLuint shader,GLenum pname, GLint *params) = 0;
/* static */ void   (TP_CALLBACK *tpGL::glGetShaderInfoLog) (GLuint shader, GLsizei maxLength, GLsizei * length, GLchar* infoLog) = 0;
/* static */ GLuint (TP_CALLBACK *tpGL::glDeleteShader) (GLuint) = 0;
/* static */ GLboolean (TP_CALLBACK *tpGL::glIsShader) (GLuint) = 0;

// Program
/* static */ GLuint (TP_CALLBACK *tpGL::glCreateProgram) () = 0;
/* static */ void   (TP_CALLBACK *tpGL::glAttachShader) (GLuint program, GLuint shader) = 0;
/* static */ void   (TP_CALLBACK *tpGL::glLinkProgram) (GLuint) = 0;
/* static */ void   (TP_CALLBACK *tpGL::glBindAttribLocation)(GLuint program,GLuint index, const GLchar* name) = 0;
/* static */ void   (TP_CALLBACK *tpGL::glGetProgramiv)(GLuint program, GLenum pname, GLint *params) = 0;
/* static */ void   (TP_CALLBACK *tpGL::glGetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei * , GLchar* infoLog) = 0;
/* static */ GLuint (TP_CALLBACK *tpGL::glUseProgram)(GLuint) = 0;
/* static */ GLuint (TP_CALLBACK *tpGL::glDeleteProgram)(GLuint) = 0;
/* static */ GLboolean (TP_CALLBACK *tpGL::glIsProgram) (GLuint) = 0;

// Vertices
/* static */ GLuint (TP_CALLBACK *tpGL::glVertexAttribPointer)(	GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer) = 0;
/* static */ void   (TP_CALLBACK* tpGL::glEnableVertexAttribArray)( GLuint index ) = 0;
/* static */ void   (TP_CALLBACK* tpGL::glDisableVertexAttribArray)( GLuint index ) = 0;

#define TP_DEF_GLFUNC_INIT(name) \
	tpGL::name##Func tpGL::name = reinterpret_cast<tpGL::name##Func>(0); 

TP_DEF_GLFUNC_INIT(glGetError)
TP_DEF_GLFUNC_INIT(glGetBooleanv)
TP_DEF_GLFUNC_INIT(glGetFloatv)
TP_DEF_GLFUNC_INIT(glGetIntegerv)

TP_DEF_GLFUNC_INIT(glGetUniformLocation)
TP_DEF_GLFUNC_INIT(glGetAttribLocation)
TP_DEF_GLFUNC_INIT(glUniform1f)
TP_DEF_GLFUNC_INIT(glUniform2f)
TP_DEF_GLFUNC_INIT(glUniform3f)
TP_DEF_GLFUNC_INIT(glUniform4f)
TP_DEF_GLFUNC_INIT(glUniform1i)
TP_DEF_GLFUNC_INIT(glUniform2i)
TP_DEF_GLFUNC_INIT(glUniform3i)
TP_DEF_GLFUNC_INIT(glUniform4i)
TP_DEF_GLFUNC_INIT(glUniform1fv)
TP_DEF_GLFUNC_INIT(glUniform2fv)
TP_DEF_GLFUNC_INIT(glUniform3fv)
TP_DEF_GLFUNC_INIT(glUniform4fv)
TP_DEF_GLFUNC_INIT(glUniform1iv)
TP_DEF_GLFUNC_INIT(glUniform2iv)
TP_DEF_GLFUNC_INIT(glUniform3iv)
TP_DEF_GLFUNC_INIT(glUniform4iv)

TP_DEF_GLFUNC_INIT(glShaderBinary)

TP_DEF_GLFUNC_INIT(glUniformMatrix2fv)
TP_DEF_GLFUNC_INIT(glUniformMatrix3fv)
TP_DEF_GLFUNC_INIT(glUniformMatrix4fv)

TP_DEF_GLFUNC_INIT(glFrontFace)

TP_DEF_GLFUNC_INIT(glGenBuffers);
TP_DEF_GLFUNC_INIT(glBindBuffer);
TP_DEF_GLFUNC_INIT(glDeleteBuffers);
TP_DEF_GLFUNC_INIT(glIsBuffer);
TP_DEF_GLFUNC_INIT(glBufferData);

TP_DEF_GLFUNC_INIT(glDepthFunc);
TP_DEF_GLFUNC_INIT(glDepthRange);

TP_DEF_GLFUNC_INIT(glActiveTexture);



TP_DEF_GLFUNC_INIT(tpGetProcAddress)


/* static */ tpList<tpString> gs_gllibs = tpList<tpString>();


/* StringName */
tpUInt tpGL::VENDOR						= 0x1F00;
tpUInt tpGL::RENDERER                   = 0x1F01;
tpUInt tpGL::VERSION                    = 0x1F02;
tpUInt tpGL::EXTENSIONS                 = 0x1F03;

/* Lighting */
tpUInt tpGL::LIGHTING				= 0x0B50;
tpUInt tpGL::LIGHT0				= 0x4000;
tpUInt tpGL::LIGHT1				= 0x4001;
tpUInt tpGL::LIGHT2				= 0x4002;
tpUInt tpGL::LIGHT3				= 0x4003;
tpUInt tpGL::LIGHT4				= 0x4004;
tpUInt tpGL::LIGHT5				= 0x4005;
tpUInt tpGL::LIGHT6				= 0x4006;
tpUInt tpGL::LIGHT7				= 0x4007;
tpUInt tpGL::SPOT_EXPONENT			= 0x1205;
tpUInt tpGL::SPOT_CUTOFF				= 0x1206;
tpUInt tpGL::CONSTANT_ATTENUATION			= 0x1207;
tpUInt tpGL::LINEAR_ATTENUATION			= 0x1208;
tpUInt tpGL::QUADRATIC_ATTENUATION		= 0x1209;
tpUInt tpGL::AMBIENT				= 0x1200;
tpUInt tpGL::DIFFUSE				= 0x1201;
tpUInt tpGL::SPECULAR				= 0x1202;
tpUInt tpGL::SHININESS				= 0x1601;
tpUInt tpGL::EMISSION				= 0x1600;
tpUInt tpGL::POSITION				= 0x1203;
tpUInt tpGL::SPOT_DIRECTION			= 0x1204;
tpUInt tpGL::AMBIENT_AND_DIFFUSE			= 0x1602;
tpUInt tpGL::COLOR_INDEXES			= 0x1603;
tpUInt tpGL::LIGHT_MODEL_TWO_SIDE			= 0x0B52;
tpUInt tpGL::LIGHT_MODEL_LOCAL_VIEWER		= 0x0B51;
tpUInt tpGL::LIGHT_MODEL_AMBIENT			= 0x0B53;
tpUInt tpGL::FRONT_AND_BACK			= 0x0408;
tpUInt tpGL::SHADE_MODEL				= 0x0B54;
tpUInt tpGL::FLAT					= 0x1D00;
tpUInt tpGL::SMOOTH				= 0x1D01;
tpUInt tpGL::COLOR_MATERIAL			= 0x0B57;
tpUInt tpGL::COLOR_MATERIAL_FACE			= 0x0B55;
tpUInt tpGL::COLOR_MATERIAL_PARAMETER		= 0x0B56;
tpUInt tpGL::NORMALIZE				= 0x0BA1;


/* Colorspace */
tpUInt tpGL::RGB = 0x1907;
tpUInt tpGL::RGBA = 0x1908;
tpUInt tpGL::LUMINANCE = 0x1909;
tpUInt tpGL::BGR = 0x80E0;
tpUInt tpGL::BGRA = 0x80E1;


                    
 /* Texture mapping */
tpUInt tpGL::TEXTURE_ENV				= 0x2300;
tpUInt tpGL::TEXTURE_ENV_MODE			= 0x2200;
tpUInt tpGL::TEXTURE_1D				= 0x0DE0;
tpUInt tpGL::TEXTURE_2D				= 0x0DE1; //0x0DE1
tpUInt tpGL::TEXTURE_WRAP_S			= 0x2802;
tpUInt tpGL::TEXTURE_WRAP_T			= 0x2803;
tpUInt tpGL::TEXTURE_MAG_FILTER			= 0x2800;
tpUInt tpGL::TEXTURE_MIN_FILTER			= 0x2801;
tpUInt tpGL::TEXTURE_ENV_COLOR			= 0x2201;
tpUInt tpGL::TEXTURE_GEN_S			= 0x0C60;
tpUInt tpGL::TEXTURE_GEN_T			= 0x0C61;
tpUInt tpGL::TEXTURE_GEN_MODE			= 0x2500;
tpUInt tpGL::TEXTURE_BORDER_COLOR			= 0x1004;
tpUInt tpGL::TEXTURE_WIDTH			= 0x1000;
tpUInt tpGL::TEXTURE_HEIGHT			= 0x1001;
tpUInt tpGL::TEXTURE_BORDER			= 0x1005;
tpUInt tpGL::TEXTURE_COMPONENTS			= 0x1003;
tpUInt tpGL::TEXTURE_RED_SIZE			= 0x805C;
tpUInt tpGL::TEXTURE_GREEN_SIZE			= 0x805D;
tpUInt tpGL::TEXTURE_BLUE_SIZE			= 0x805E;
tpUInt tpGL::TEXTURE_ALPHA_SIZE			= 0x805F;
tpUInt tpGL::TEXTURE_LUMINANCE_SIZE		= 0x8060;
tpUInt tpGL::TEXTURE_INTENSITY_SIZE		= 0x8061;
tpUInt tpGL::NEAREST_MIPMAP_NEAREST		= 0x2700;
tpUInt tpGL::NEAREST_MIPMAP_LINEAR		= 0x2702;
tpUInt tpGL::LINEAR_MIPMAP_NEAREST		= 0x2701;
tpUInt tpGL::LINEAR_MIPMAP_LINEAR			= 0x2703;
tpUInt tpGL::OBJECT_LINEAR			= 0x2401;
tpUInt tpGL::OBJECT_PLANE				= 0x2501;
tpUInt tpGL::EYE_LINEAR				= 0x2400;
tpUInt tpGL::EYE_PLANE				= 0x2502;
tpUInt tpGL::SPHERE_MAP				= 0x2402;
tpUInt tpGL::DECAL				= 0x2101;
tpUInt tpGL::MODULATE				= 0x2100;
tpUInt tpGL::NEAREST				= 0x2600;
tpUInt tpGL::REPEAT				= 0x2901;
tpUInt tpGL::CLAMP				= 0x2900;
tpUInt tpGL::S					= 0x2000;
tpUInt tpGL::T					= 0x2001;
tpUInt tpGL::R					= 0x2002;
tpUInt tpGL::Q					= 0x2003;
tpUInt tpGL::TEXTURE_GEN_R			= 0x0C62;
tpUInt tpGL::TEXTURE_GEN_Q			= 0x0C63;


/* Primitives */
 tpUInt tpGL::POINTS				= 0x0000;
 tpUInt tpGL::LINES					= 0x0001;
 tpUInt tpGL::LINE_LOOP				= 0x0002;
 tpUInt tpGL::LINE_STRIP			= 0x0003;
 tpUInt tpGL::TRIANGLES				= 0x0004;
 tpUInt tpGL::TRIANGLE_STRIP		= 0x0005;
 tpUInt tpGL::TRIANGLE_FAN			= 0x0006;
 tpUInt tpGL::QUADS					= 0x0007;
 tpUInt tpGL::QUAD_STRIP			= 0x0008;
 tpUInt tpGL::POLYGON				= 0x0009;

/* Vertex Arrays */
 tpUInt tpGL::VERTEX_ARRAY				 = 0x8074;
 tpUInt tpGL::NORMAL_ARRAY				 = 0x8075;
 tpUInt tpGL::COLOR_ARRAY				 = 0x8076;
 tpUInt tpGL::INDEX_ARRAY				 = 0x8077;
 tpUInt tpGL::TEXTURE_COORD_ARRAY			 = 0x8078;
 tpUInt tpGL::EDGE_FLAG_ARRAY			 = 0x8079;
 tpUInt tpGL::VERTEX_ARRAY_SIZE			 = 0x807A;
 tpUInt tpGL::VERTEX_ARRAY_TYPE			 = 0x807B;
 tpUInt tpGL::VERTEX_ARRAY_STRIDE			 = 0x807C;
 tpUInt tpGL::NORMAL_ARRAY_TYPE			 = 0x807E;
 tpUInt tpGL::NORMAL_ARRAY_STRIDE			 = 0x807F;
 tpUInt tpGL::COLOR_ARRAY_SIZE			 = 0x8081;
 tpUInt tpGL::COLOR_ARRAY_TYPE			 = 0x8082;
 tpUInt tpGL::COLOR_ARRAY_STRIDE			 = 0x8083;
 tpUInt tpGL::INDEX_ARRAY_TYPE			 = 0x8085;
 tpUInt tpGL::INDEX_ARRAY_STRIDE			 = 0x8086;
 tpUInt tpGL::TEXTURE_COORD_ARRAY_SIZE		 = 0x8088;
 tpUInt tpGL::TEXTURE_COORD_ARRAY_TYPE		 = 0x8089;
 tpUInt tpGL::TEXTURE_COORD_ARRAY_STRIDE		 = 0x808A;
 tpUInt tpGL::EDGE_FLAG_ARRAY_STRIDE		 = 0x808C;
 tpUInt tpGL::VERTEX_ARRAY_POINTER			 = 0x808E;
 tpUInt tpGL::NORMAL_ARRAY_POINTER			 = 0x808F;
 tpUInt tpGL::COLOR_ARRAY_POINTER			 = 0x8090;
 tpUInt tpGL::INDEX_ARRAY_POINTER			 = 0x8091;
 tpUInt tpGL::TEXTURE_COORD_ARRAY_POINTER		 = 0x8092;
 tpUInt tpGL::EDGE_FLAG_ARRAY_POINTER		 = 0x8093;
 tpUInt tpGL::V2F					 = 0x2A20;
 tpUInt tpGL::V3F					 = 0x2A21;
 tpUInt tpGL::C4UB_V2F				 = 0x2A22;
 tpUInt tpGL::C4UB_V3F				 = 0x2A23;
 tpUInt tpGL::C3F_V3F				 = 0x2A24;
 tpUInt tpGL::N3F_V3F				 = 0x2A25;
 tpUInt tpGL::C4F_N3F_V3F				 = 0x2A26;
 tpUInt tpGL::T2F_V3F				 = 0x2A27;
 tpUInt tpGL::T4F_V4F				 = 0x2A28;
 tpUInt tpGL::T2F_C4UB_V3F				 = 0x2A29;
 tpUInt tpGL::T2F_C3F_V3F				 = 0x2A2A;
 tpUInt tpGL::T2F_N3F_V3F				 = 0x2A2B;
 tpUInt tpGL::T2F_C4F_N3F_V3F			 = 0x2A2C;
 tpUInt tpGL::T4F_C4F_N3F_V4F			 = 0x2A2D;

/* Matrix Mode */
 tpUInt tpGL::MATRIX_MODE				 = 0x0BA0;
 tpUInt tpGL::MODELVIEW				 = 0x1700;
 tpUInt tpGL::PROJECTION				 = 0x1701;
 tpUInt tpGL::TEXTURE			=	0x1702;

/* Points */
 tpUInt tpGL::POINT_SMOOTH				 = 0x0B10;
 tpUInt tpGL::POINT_SIZE				 = 0x0B11;
 tpUInt tpGL::POINT_SIZE_GRANULARITY 		 = 0x0B13;
 tpUInt tpGL::POINT_SIZE_RANGE			 = 0x0B12;

/* Lines */
 tpUInt tpGL::LINE_SMOOTH				 = 0x0B20;
 tpUInt tpGL::LINE_STIPPLE				 = 0x0B24;
 tpUInt tpGL::LINE_STIPPLE_PATTERN			 = 0x0B25;
 tpUInt tpGL::LINE_STIPPLE_REPEAT			 = 0x0B26;
 tpUInt tpGL::LINE_WIDTH				 = 0x0B21;
 tpUInt tpGL::LINE_WIDTH_GRANULARITY		 = 0x0B23;
 tpUInt tpGL::LINE_WIDTH_RANGE			 = 0x0B22;

/* Polygons */
 tpUInt tpGL::POINT				 = 0x1B00;
 tpUInt tpGL::LINE					 = 0x1B01;
 tpUInt tpGL::FILL					 = 0x1B02;
 tpUInt tpGL::CW					 = 0x0900;
 tpUInt tpGL::CCW					 = 0x0901;
 tpUInt tpGL::FRONT				 = 0x0404;
 tpUInt tpGL::BACK					 = 0x0405;
 tpUInt tpGL::POLYGON_MODE				 = 0x0B40;
 tpUInt tpGL::POLYGON_SMOOTH			 = 0x0B41;
 tpUInt tpGL::POLYGON_STIPPLE			 = 0x0B42;
 tpUInt tpGL::EDGE_FLAG				 = 0x0B43;
 tpUInt tpGL::CULL_FACE				 = 0x0B44;
 tpUInt tpGL::CULL_FACE_MODE			 = 0x0B45;
 tpUInt tpGL::FRONT_FACE				 = 0x0B46;
 tpUInt tpGL::POLYGON_OFFSET_FACTOR		 = 0x8038;
 tpUInt tpGL::POLYGON_OFFSET_UNITS			 = 0x2A00;
 tpUInt tpGL::POLYGON_OFFSET_POINT			 = 0x2A01;
 tpUInt tpGL::POLYGON_OFFSET_LINE			 = 0x2A02;
 tpUInt tpGL::POLYGON_OFFSET_FILL		=	0x8037;


/* Fog */;
 tpUInt tpGL::FOG					 = 0x0B60;
 tpUInt tpGL::FOG_MODE				 = 0x0B65;
 tpUInt tpGL::FOG_DENSITY			 = 0x0B62;
 tpUInt tpGL::FOG_COLOR				 = 0x0B66;
 tpUInt tpGL::FOG_INDEX				 = 0x0B61;
 tpUInt tpGL::FOG_START				 = 0x0B63;
 tpUInt tpGL::FOG_END				 = 0x0B64;
 tpUInt tpGL::LINEAR				 = 0x2601;
 tpUInt tpGL::EXP					 = 0x0800;
 tpUInt tpGL::EXP2					 = 0x0801;

/* Logic Ops */;
 tpUInt tpGL::LOGIC_OP				 = 0x0BF1;
 tpUInt tpGL::INDEX_LOGIC_OP			 = 0x0BF1;
 tpUInt tpGL::COLOR_LOGIC_OP			 = 0x0BF2;
 tpUInt tpGL::LOGIC_OP_MODE			 = 0x0BF0;
 tpUInt tpGL::CLEAR				 = 0x1500;
 tpUInt tpGL::SET					 = 0x150F;
 tpUInt tpGL::COPY					 = 0x1503;
 tpUInt tpGL::COPY_INVERTED			 = 0x150C;
 tpUInt tpGL::NOOP					 = 0x1505;
 tpUInt tpGL::INVERT				 = 0x150A;
 tpUInt tpGL::AND					 = 0x1501;
 tpUInt tpGL::NAND					 = 0x150E;
 tpUInt tpGL::OR					 = 0x1507;
 tpUInt tpGL::NOR					 = 0x1508;
 tpUInt tpGL::XOR					 = 0x1506;
 tpUInt tpGL::EQUIV				 = 0x1509;
 tpUInt tpGL::AND_REVERSE				 = 0x1502;
 tpUInt tpGL::AND_INVERTED				 = 0x1504;
 tpUInt tpGL::OR_REVERSE				 = 0x150B;
 tpUInt tpGL::OR_INVERTED				 = 0x150D ;



tpGL::tpGL(unsigned int mode) 
{
}


tpVoid* tpGL::getAddress( const tpString& name, const tpString& altname /*= ""*/)
{
	tpVoid* result(0L);

	if (tpGL::tpGetProcAddress) 
	{
		result = tpGL::tpGetProcAddress(name.c_str());
		tpLogNotify("%s - %s = 0x%x",__FUNCTION__,name.c_str(),(tpLong)result);
	}
	
	if (!result) {
		result = mg_ogl->getAddress(name);
	}
	
	if (!result) {
		result = mg_egl->getAddress(name);
	}

	if (!result && (!altname.isEmpty()) )
	{
		result = getAddress(altname,"");
	}

	return result;
}

void tpGL::loadLibraries()
{
	mg_egl = new tpLibrary();
	mg_ogl = new tpLibrary();

	tpLogNotify("Twisted Pairs OpenGL runtime linker");

	// first try to load EGL
//	if (!tpGL::mg_egl->open(tpString(L"libEGL")))
//	if (!tpGL::mg_egl->open(tpString(L"libGLES_CM")))
	if (!tpGL::mg_egl->open("libEGL"))
	if (!tpGL::mg_egl->open("libGLES_CM"))
//	if (!tpGL::mg_egl->open(tpString(L"/usr/lib/mesa/libGL.so.1")))
	{		
		tpLogNotify("%s - no EGL",__FUNCTION__);
	}

	if (tpGL::mg_egl->isValid())
	{
		tpGL::tpGetProcAddress = reinterpret_cast<tpGL::tpGetProcAddressFunc>(tpGL::mg_egl->getAddress("eglGetProcAddress"));

		if (0 == tpGL::tpGetProcAddress)
		{
			tpGL::mg_egl->close();
		}

	}
	//if (!tpGL::mg_ogl->open(tpString(L"libGLESv2")))
	//if (!tpGL::mg_ogl->open(tpString(L"libGLES_CM")))
	//if (!tpGL::mg_ogl->open(tpString(L"libGLESv1_CM")))
//	if (!tpGL::mg_ogl->open(tpString(L"libGLES_CL")))
	if (!tpGL::mg_ogl->open(tpString("OpenGL32")))
	if (!tpGL::mg_ogl->open(tpString("/System/Library/Frameworks/OpenGL.framework/OpenGL")))
	if (!tpGL::mg_ogl->open(tpString("/usr/lib/libGL.so.1")))
	if (!tpGL::mg_ogl->open(tpString("/usr/lib/mesa/libGL.so.1")))
	//if (!tpGL::mg_ogl->open(tpString("/usr/X11/lib/libGL.dylib")))
	{
		tpLogError("%s - no OpenGL backend",__FUNCTION__);
	} else {
		tpLogNotify("%s - OpenGL backend from %s",__FUNCTION__,mg_ogl->getModuleName().c_str());
	}
	
	
#if defined(__APPLE__)
	if (tpGL::mg_ogl->getAddress("CGLSetSurface")) tpGL::get().load(tpGL::TP_GL_FULL);
#endif


#if defined(_WIN32)
	/* Windows GL */
	tpGL::wglCreateContext = reinterpret_cast<HGLRC(TP_CALLBACK *)(HDC)>(tpGL::mg_ogl->getAddress("wglCreateContext"));
	tpGL::wglDeleteContext = reinterpret_cast<BOOL(TP_CALLBACK *)(HGLRC)>(tpGL::mg_ogl->getAddress("wglDeleteContext"));
	tpGL::MakeCurrent = reinterpret_cast<BOOL(TP_CALLBACK *)(HDC,HGLRC)>(tpGL::mg_ogl->getAddress("wglMakeCurrent"));	
	tpGL::SwapBuffers = reinterpret_cast<BOOL(TP_CALLBACK *)(HDC)>(tpGL::mg_ogl->getAddress("wglSwapBuffers"));
#endif

#if defined(__unix) || defined(__APPLE__)
	/* GLX variant */
	//static XVisualInfo* (TP_CALLBACK* glXQueryExtension)(Display *dpy, int screen, int *attribList );
	//static XVisualInfo* (TP_CALLBACK* glXChooseVisual)( Display *dpy, int screen, int *attribList );
	//static Bool         (TP_CALLBACK* glXQueryVersion)( Display *dpy, int *major, int *minor );
	//static GLXContext   (TP_CALLBACK* glXCreateContext)( Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct );
	//static void         (TP_CALLBACK* glXDestroyContext)( Display *dpy, GLXContext ctx );
	//static Bool         (TP_CALLBACK* glXMakeCurrent)( Display *dpy, GLXDrawable drawable, GLXContext ctx );

	
	tpGL::glXQueryExtension = reinterpret_cast<XVisualInfo* (TP_CALLBACK*)(Display*, int , int * )>(tpGL::mg_ogl->getAddress("glXQueryExtension"));
	tpGL::glXChooseVisual = reinterpret_cast<XVisualInfo* (TP_CALLBACK*)(Display*, int , int * )>(tpGL::mg_ogl->getAddress("glXChooseVisual"));
	tpGL::glXQueryVersion = reinterpret_cast<Bool (TP_CALLBACK*)(Display*, int*  , int * )>(tpGL::mg_ogl->getAddress("glXQueryVersion"));
	tpGL::glXCreateContext = reinterpret_cast<GLXContext (TP_CALLBACK*)( Display*, XVisualInfo*, GLXContext, Bool )>(tpGL::mg_ogl->getAddress("glXCreateContext"));
	tpGL::glXDestroyContext = reinterpret_cast<void (TP_CALLBACK*)( Display*, GLXContext )>(tpGL::mg_ogl->getAddress("glXDestroyContext"));
	tpGL::glXMakeCurrent = reinterpret_cast<Bool (TP_CALLBACK*)( Display*, GLXDrawable, GLXContext )>(tpGL::mg_ogl->getAddress("glXMakeCurrent"));
	tpGL::glXSwapBuffers  = reinterpret_cast< void (TP_CALLBACK*)( Display *, GLXDrawable )>(tpGL::mg_ogl->getAddress("glXSwapBuffers"));

#endif

#if defined(_WIN32) && !defined(_WIN32_WCE) 
	tpGL::tpGetProcAddress = reinterpret_cast<tpGetProcAddressFunc>(tpGL::mg_ogl->getAddress("wglGetProcAddress"));
#endif

	tpGL::GetString = reinterpret_cast<const GLubyte* (TP_CALLBACK *)(GLenum)>(tpGL::mg_ogl->getAddress("glGetString"));
	if (0 == tpGL::GetString)
	{
		tpGL::GetString = reinterpret_cast<const GLubyte* (TP_CALLBACK *)(GLenum)>(tpGL::mg_egl->getAddress("glGetString"));
	}
}

void tpGL::loadGL() {

	loadEGL();
	loadOGL();

}

/*

bool tpGL::hasVBO() {
	return IsExtensionSupported( "GL_ARB_vertex_buffer_object" );
}
 */

tpGL::~tpGL()
{
	tpGL::get().release();
}



/*

bool tpIsExtensionSupported( char* szTargetExtension )
{
	const unsigned char *pszExtensions = NULL;
	const unsigned char *pszStart;
	unsigned char *pszWhere, *pszTerminator;
	
	// Extension names should not have spaces
	pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
	if( pszWhere || *szTargetExtension == '\0' )
		return false;
	
	// Get Extensions String
	pszExtensions = tpGL::GetString( tpGL::EXTENSIONS );
	
	// Search The Extensions String For An Exact Copy
	pszStart = pszExtensions;
	for(;;)
	{
		pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
		if( !pszWhere )
			break;
		pszTerminator = pszWhere + strlen( szTargetExtension );
		if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
			if( *pszTerminator == ' ' || *pszTerminator == '\0' )
				return true;
		pszStart = pszTerminator;
	}
	return false;
}

class tpGLContextObject {
	
	void BuildVBOs() {
		
		// Generate And Bind The Vertex Buffer
		glGenBuffersARB( 1, &m_nVBOVertices );					// Get A Valid Name
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOVertices );			// Bind The Buffer
		// Load The Data
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_nVertexCount*3*sizeof(float), m_pVertices, GL_STATIC_DRAW_ARB );
		
		// Generate And Bind The Texture Coordinate Buffer
		glGenBuffersARB( 1, &m_nVBOTexCoords );					// Get A Valid Name
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_nVBOTexCoords );		// Bind The Buffer
		// Load The Data
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, m_nVertexCount*2*sizeof(float), m_pTexCoords, GL_STATIC_DRAW_ARB );
		
		// Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card
		delete [] m_pVertices; m_pVertices = NULL;
		delete [] m_pTexCoords; m_pTexCoords = NULL;
	}
	
	
}
*/



tpVoid tpGL::setPerspective( tpReal FOV, tpReal aspect, tpReal nearDist, tpReal farDist ) 
{
	
	tpReal w,h;
	w = 2 * nearDist * tpReal(tan(FOV / 2));
	h = w / aspect;
	
	if (tpGL::Frustum) {
		tpGL::Frustum(w,-w,h,-h,nearDist,farDist);
	}
	else if (tpGL::Frustumf) 
	{
		tpGL::Frustumf(w,-w,h,-h,nearDist,farDist);
	}

}

bool tpGL::hasEGL()
{
	return (mg_egl->isValid());
}

void tpGL::loadEGL()
{
	//tpGL::tpGetProcAddress = reinterpret_cast<tpGL::tpGetProcAddressFunc>(tpGL::mg_egl->getAddress("eglGetProcAddress"));
	tpGL::eglQueryString = reinterpret_cast<const GLchar*(TP_CALLBACK *)(EGLDisplay,GLint)>(tpGL::mg_egl->getAddress("eglQueryString"));
	tpGL::eglQuerySurface = reinterpret_cast<EGLBoolean (TP_CALLBACK *)(const EGLDisplay display, EGLSurface surface, EGLint attribute, EGLint * value)>(tpGL::mg_egl->getAddress("eglQuerySurface"));

	tpGL::eglGetError = reinterpret_cast<EGLint(TP_CALLBACK *)(void)>(tpGL::mg_egl->getAddress("eglGetError"));
	tpGL::eglInitialize = reinterpret_cast<EGLBoolean (TP_CALLBACK *)(EGLDisplay dpy, EGLint *major, EGLint *minor)>(tpGL::mg_egl->getAddress("eglInitialize"));
	tpGL::eglGetDisplay = reinterpret_cast<EGLDisplay(TP_CALLBACK *)(NativeDisplayType native_display)>(tpGL::mg_egl->getAddress("eglGetDisplay"));
	tpGL::eglGetConfigs = reinterpret_cast<EGLBoolean(TP_CALLBACK *)(EGLDisplay dpy,EGLConfig* config, EGLint config_size, EGLint* num_config )>(tpGL::mg_egl->getAddress("eglGetConfigs"));
	tpGL::eglChooseConfig = reinterpret_cast<EGLBoolean(TP_CALLBACK *)(EGLDisplay display,EGLint const * attrib_list,EGLConfig * configs, EGLint config_size, EGLint * num_config)>(tpGL::mg_egl->getAddress("eglChooseConfig"));
	tpGL::eglCreateContext = reinterpret_cast<EGLContext(TP_CALLBACK *)(EGLDisplay display,EGLConfig config,EGLContext share_context,EGLint const * attrib_list)>(tpGL::mg_egl->getAddress("eglCreateContext"));
	tpGL::eglCreateWindowSurface = reinterpret_cast<EGLSurface(TP_CALLBACK *)(EGLDisplay display,EGLConfig config,NativeWindowType native_window,EGLint const * attrib_list)>(tpGL::mg_egl->getAddress("eglCreateWindowSurface"));

	tpGL::eglMakeCurrent = reinterpret_cast<EGLBoolean(TP_CALLBACK *)(EGLDisplay display,EGLSurface draw,EGLSurface read,EGLContext context)>(tpGL::mg_egl->getAddress("eglMakeCurrent"));
	tpGL::eglSwapBuffers = reinterpret_cast<EGLBoolean(TP_CALLBACK *)(EGLDisplay display,EGLSurface surface)>(tpGL::mg_egl->getAddress("eglSwapBuffers"));
}

void tpGL::loadOGL()
{
#define TP_FETCH_ADDR(glFunc) \
	tpGL::glFunc = reinterpret_cast<tpGL::glFunc##Func>(getAddress(#glFunc));

#define TP_FETCH_ADDR2(glFunc,glFuncAltName) \
	tpGL::glFunc = reinterpret_cast<tpGL::glFunc##Func>(getAddress(#glFunc,#glFuncAltName));


	if (tpGL::mg_ogl->isValid()) 
	{

		tpGL::Begin = reinterpret_cast<void(TP_CALLBACK *)(GLenum)>(tpGL::mg_ogl->getAddress("glBegin"));	
		tpGL::End = reinterpret_cast<void(TP_CALLBACK *)(void)>(tpGL::mg_ogl->getAddress("glEnd"));

		tpGL::ClearColor = reinterpret_cast<void(TP_CALLBACK *)(GLclampf, GLclampf, GLclampf, GLclampf)>(tpGL::mg_ogl->getAddress("glClearColor"));
		tpGL::Clear = reinterpret_cast<void(TP_CALLBACK *)(GLenum)>(tpGL::mg_ogl->getAddress("glClear"));

		tpGL::Enable = reinterpret_cast<void(TP_CALLBACK *)(GLenum)>(tpGL::mg_ogl->getAddress("glEnable"));
		tpGL::Disable = reinterpret_cast<void(TP_CALLBACK *)(GLenum)>(tpGL::mg_ogl->getAddress("glDisable"));
		tpGL::Flush = reinterpret_cast<void(TP_CALLBACK *)(void)>(tpGL::mg_ogl->getAddress("glFlush"));
		tpGL::Finish = reinterpret_cast<void(TP_CALLBACK *)(void)>(tpGL::mg_ogl->getAddress("glFinish"));
		tpGL::ShadeModel = reinterpret_cast<void(TP_CALLBACK *)(GLenum)>(tpGL::mg_ogl->getAddress("glShadeModel"));

		tpGL::MatrixMode = reinterpret_cast<void(TP_CALLBACK *)(GLenum)>(tpGL::mg_ogl->getAddress("glMatrixMode"));
		tpGL::LoadIdentity = reinterpret_cast<void(TP_CALLBACK *)(void)>(tpGL::mg_ogl->getAddress("glLoadIdentity"));

		tpGL::LoadMatrixd = reinterpret_cast<void(TP_CALLBACK *)(const GLdouble*)>(tpGL::mg_ogl->getAddress("glLoadMatrixd"));
		tpGL::LoadMatrixf = reinterpret_cast<void(TP_CALLBACK *)(const GLfloat*)>(tpGL::mg_ogl->getAddress("glLoadMatrixf"));

		tpGL::MultMatrixd = reinterpret_cast<void(TP_CALLBACK *)(const GLdouble*)>(tpGL::mg_ogl->getAddress("glMultMatrixd"));
		tpGL::MultMatrixf = reinterpret_cast<void(TP_CALLBACK *)(const GLfloat*)>(tpGL::mg_ogl->getAddress("glMultMatrixf"));

		tpGL::PushMatrix = reinterpret_cast<void(TP_CALLBACK *)(void)>(tpGL::mg_ogl->getAddress("glPushMatrix"));
		tpGL::PopMatrix = reinterpret_cast<void(TP_CALLBACK *)(void)>(tpGL::mg_ogl->getAddress("glPopMatrix"));

		tpGL::Materialf = reinterpret_cast<void(TP_CALLBACK *)(GLenum, GLenum, GLfloat)>(tpGL::mg_ogl->getAddress("glMaterialf"));
		tpGL::Materialfv = reinterpret_cast<void(TP_CALLBACK *)(GLenum, GLenum, const GLfloat*)>(tpGL::mg_ogl->getAddress("glMaterialfv"));
		tpGL::Materiali = reinterpret_cast<void(TP_CALLBACK *)(GLenum, GLenum, GLint)>(tpGL::mg_ogl->getAddress("glMateriali"));
		tpGL::Materialiv = reinterpret_cast<void(TP_CALLBACK *)(GLenum, GLenum, const GLint*)>(tpGL::mg_ogl->getAddress("glMaterialiv"));

		tpGL::TexCoord2d  = reinterpret_cast<void(TP_CALLBACK *)(GLdouble, GLdouble)>(tpGL::mg_ogl->getAddress("glTexCoord2d"));
		tpGL::TexCoord2dv = reinterpret_cast<void(TP_CALLBACK *)(const GLdouble*)>(tpGL::mg_ogl->getAddress("glTexCoord2dv"));
		tpGL::TexCoord2f  = reinterpret_cast<void(TP_CALLBACK *)(GLfloat, GLfloat)>(tpGL::mg_ogl->getAddress("glTexCoord2f"));
		tpGL::TexCoord2fv = reinterpret_cast<void(TP_CALLBACK *)(const GLfloat*)>(tpGL::mg_ogl->getAddress("glTexCoord2fv"));

		tpGL::IsTexture = reinterpret_cast<GLboolean(TP_CALLBACK *)(GLuint)>(tpGL::mg_ogl->getAddress("glIsTexture"));
		tpGL::GenTextures = reinterpret_cast<void(TP_CALLBACK *)(GLsizei,GLuint*)>(tpGL::mg_ogl->getAddress("glGenTextures"));
		tpGL::BindTexture = reinterpret_cast<void(TP_CALLBACK *)(GLsizei,GLuint)>(tpGL::mg_ogl->getAddress("glBindTexture"));
		tpGL::TexImage2D = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*)>(tpGL::mg_ogl->getAddress("glTexImage2D"));
		tpGL::TexSubImage2D = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,const GLvoid*)>(tpGL::mg_ogl->getAddress("glTexSubImage2D"));

		tpGL::TexParameterf = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLenum,GLfloat)>(tpGL::mg_ogl->getAddress("glTexParameterf"));
		tpGL::TexParameteri = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLenum,GLint)>(tpGL::mg_ogl->getAddress("glTexParameteri"));
		tpGL::TexParameterfv = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLenum,const GLfloat*)>(tpGL::mg_ogl->getAddress("glTexParameterfv"));
		tpGL::TexParameteriv = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLenum,const GLint*)>(tpGL::mg_ogl->getAddress("glTexParameteriv"));

		tpGL::LightModelf = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLfloat)>(tpGL::mg_ogl->getAddress("glLightModelf"));
		tpGL::LightModelfv = reinterpret_cast<void(TP_CALLBACK *)(GLenum,const GLfloat*)>(tpGL::mg_ogl->getAddress("glLightModelfv"));
		tpGL::LightModeli = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLint)>(tpGL::mg_ogl->getAddress("glLightModeli"));
		tpGL::LightModeliv = reinterpret_cast<void(TP_CALLBACK *)(GLenum,const GLint*)>(tpGL::mg_ogl->getAddress("glLightModeliv"));

		tpGL::Lightf = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLenum,GLfloat)>(tpGL::mg_ogl->getAddress("glLightf"));
		tpGL::Lightfv = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLenum,const GLfloat*)>(tpGL::mg_ogl->getAddress("glLightfv"));
		tpGL::Lighti = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLenum,GLint)>(tpGL::mg_ogl->getAddress("glLighti"));
		tpGL::Lightiv = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLenum,const GLint*)>(tpGL::mg_ogl->getAddress("glLightiv"));

		/* Normals */
		tpGL::Normal3dv = reinterpret_cast<void(TP_CALLBACK *)(const GLdouble *)>(tpGL::mg_ogl->getAddress("glNormal3dv"));

		/* Vertices */
		tpGL::Vertex3dv = reinterpret_cast<void(TP_CALLBACK *)(const GLdouble *)>(tpGL::mg_ogl->getAddress("glVertex3dv"));

		/* OpenGL 1.1 and OpenGL ES */
		tpGL::EnableClientState = reinterpret_cast<void(TP_CALLBACK *)(GLenum)>(tpGL::mg_ogl->getAddress("glEnableClientState"));
		tpGL::DisableClientState = reinterpret_cast<void(TP_CALLBACK *)(GLenum)>(tpGL::mg_ogl->getAddress("glDisableClientState"));
		tpGL::NormalPointer = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLsizei,const GLvoid*)>(tpGL::mg_ogl->getAddress("glNormalPointer"));
		tpGL::TexCoordPointer = reinterpret_cast<void(TP_CALLBACK *)(GLint,GLenum,GLsizei,const GLvoid*)>(tpGL::mg_ogl->getAddress("glTexCoordPointer"));
		tpGL::VertexPointer = reinterpret_cast<void(TP_CALLBACK *)(GLint,GLenum,GLsizei,const GLvoid*)>(tpGL::mg_ogl->getAddress("glVertexPointer"));
		tpGL::DrawElements = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLsizei,GLenum,const GLvoid*)>(tpGL::mg_ogl->getAddress("glDrawElements"));
		tpGL::DrawArrays = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLint,GLsizei)>(tpGL::mg_ogl->getAddress("glDrawArrays"));



		tpGL::PixelStorei = reinterpret_cast<void(TP_CALLBACK *)(GLenum,GLint)>(tpGL::mg_ogl->getAddress("glPixelStorei"));


		// general 
		TP_FETCH_ADDR(glGetError)
		TP_FETCH_ADDR(glGetBooleanv)
		TP_FETCH_ADDR(glGetIntegerv)
		TP_FETCH_ADDR(glGetFloatv)

		TP_FETCH_ADDR(glActiveTexture);

		// Shader
		TP_FETCH_ADDR2(glCreateShader,glCreateShaderObjectARB)
		TP_FETCH_ADDR2(glShaderSource,glShaderSourceARB)
		TP_FETCH_ADDR2(glCompileShader,glCompileShaderARB)
		TP_FETCH_ADDR2(glGetShaderiv,glCompileShaderARB)
		TP_FETCH_ADDR(glGetShaderInfoLog)
		TP_FETCH_ADDR(glDeleteShader)
		TP_FETCH_ADDR(glIsShader)
		TP_FETCH_ADDR(glShaderBinary)

		// Program
		TP_FETCH_ADDR(glCreateProgram)
		TP_FETCH_ADDR(glAttachShader)
		TP_FETCH_ADDR(glLinkProgram)
		TP_FETCH_ADDR(glBindAttribLocation)
		TP_FETCH_ADDR(glGetProgramiv)
		TP_FETCH_ADDR(glGetProgramInfoLog)
		TP_FETCH_ADDR(glUseProgram)
		TP_FETCH_ADDR(glDeleteProgram)
		TP_FETCH_ADDR2(glIsProgram,glIsProgramARB)

		// Vertex
		TP_FETCH_ADDR(glVertexAttribPointer)
		TP_FETCH_ADDR(glEnableVertexAttribArray)
		TP_FETCH_ADDR(glDisableVertexAttribArray)

		TP_FETCH_ADDR(glGetUniformLocation)	
		TP_FETCH_ADDR(glGetAttribLocation)	
		TP_FETCH_ADDR(glUniform1f)
		TP_FETCH_ADDR(glUniform2f)
		TP_FETCH_ADDR(glUniform3f)
		TP_FETCH_ADDR(glUniform4f)
		TP_FETCH_ADDR(glUniform1i)
		TP_FETCH_ADDR(glUniform2i)
		TP_FETCH_ADDR(glUniform3i)
		TP_FETCH_ADDR(glUniform4i)
		TP_FETCH_ADDR(glUniform1fv)
		TP_FETCH_ADDR(glUniform2fv)
		TP_FETCH_ADDR(glUniform3fv)
		TP_FETCH_ADDR(glUniform4fv)
		TP_FETCH_ADDR(glUniform1iv)
		TP_FETCH_ADDR(glUniform2iv)
		TP_FETCH_ADDR(glUniform3iv)
		TP_FETCH_ADDR(glUniform4iv)

		TP_FETCH_ADDR(glUniformMatrix2fv)
		TP_FETCH_ADDR(glUniformMatrix3fv)
		TP_FETCH_ADDR(glUniformMatrix4fv)

		TP_FETCH_ADDR(glFrontFace);
		
		TP_FETCH_ADDR2(glGenBuffers,glGenBuffersARB);
		TP_FETCH_ADDR2(glBindBuffer,glBindBufferARB);
		TP_FETCH_ADDR2(glDeleteBuffers,glDeleteBuffersARB);
		TP_FETCH_ADDR2(glIsBuffer,glIsBufferARB);
		TP_FETCH_ADDR2(glBufferData,glBufferDataARB);

		TP_FETCH_ADDR(glDepthFunc);
		TP_FETCH_ADDR(glDepthRange);


		/* Windowing */
		tpGL::Viewport = reinterpret_cast<void(TP_CALLBACK *)(GLint,GLint,GLsizei,GLsizei)>(tpGL::mg_ogl->getAddress("glViewport"));
		tpGL::Frustum = reinterpret_cast<void(TP_CALLBACK *)(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble)>(tpGL::mg_ogl->getAddress("glFrustum"));
		tpGL::Frustumf = reinterpret_cast<void(TP_CALLBACK *)(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat)>(tpGL::mg_ogl->getAddress("glFrustumf"));
		tpGL::Frustumx = reinterpret_cast<void(TP_CALLBACK *)(GLfixed,GLfixed,GLfixed,GLfixed,GLfixed,GLfixed)>(tpGL::mg_ogl->getAddress("glFrustumx"));
	}

#undef TP_FETCH_ADDR // 

}

bool tpGL::load( tpUInt stage /*= TP_GL_PREFETCH */ )
{
	switch (stage)
	{
		case TP_GL_PREFETCH:
			loadLibraries();
			if (hasEGL()) loadEGL();
			break;
		case TP_GL_FULL:
			loadOGL();
			break;
		default:
			break;
	}

	return true;
}

tpGL& tpGL::get()
{
	static tpGL gl;
	return gl;
}

tpVoid tpGL::release()
{
	tpGL::mg_egl = 0;
	tpGL::mg_ogl = 0;
}

bool tpGL::isValid() const
{
	return (0 != tpGL::GetString );
}

bool tpGL::hasShaderSupport()
{
	/* pretty random */
	return (0 != tpGL::glAttachShader);
}

bool tpGL::hasShaderBinarySupport()
{
	GLint numBinaryFormats;
	if (tpGL::glGetIntegerv) 
	{
		//tpGL::glGetIntegerv(
	}

	return numBinaryFormats > 0;
}

bool tpGL::hasShaderCompiler()
{
	/* this needs to be rewritten:
	 * - handle properly OpenGL 2.0 and higher (no GL_SHADER_COMPILER)
	 * - handle properly OpenGL 1.x and OpenGL ES 1.x 
	 * - handle emulated OpenGL ES 2.0 (AMD vs. Mali)
	 */
	GLboolean hascompiler(tpGL::_FALSE);

	if (tpGL::hasEGL())
	{
		if (tpGL::glGetIntegerv) 
		{
			#define GL_SHADER_COMPILER 0x8DFA
			tpGL::glGetBooleanv(GL_SHADER_COMPILER,&hascompiler);
			tpLogNotify("%s - %d (actual value from driver)",__FUNCTION__,hascompiler);
		}
	} else {
		hascompiler = tpGL::_TRUE;
		tpLogNotify("%s - %d (prefetch value)",__FUNCTION__,hascompiler);
	}

	return tpGL::_TRUE == hascompiler; 
}
