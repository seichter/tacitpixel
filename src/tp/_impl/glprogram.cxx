/*
 * Copyright (C) 1999-2013 Hartmut Seichter
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

#include "glprogram.h"

#include "tp/host.h"
#include "tp/rtl.h"

#if defined(_WIN32)
#	define __TP_CALL __stdcall
#else
#	define __TP_CALL
#endif



#if defined(TP_OS_OSX)
	#include <OpenGL/OpenGL.h>
	#include <OpenGL/gl3.h>
	#include <OpenGL/gl3ext.h>
#elif defined(TP_OS_LINUX)
	#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
	#include <GL/glext.h>
#elif defined(TP_OS_WIN32)
	#include <GL/gl.h>
//    #include <GL/glext.h>
#endif

#define GL_COMPILE_STATUS 0x8B81
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31

#define GL_NUM_SHADER_BINARY_FORMATS 0x8DF9
#define GL_SHADER_BINARY_FORMATS 0x8DF8

#define GL_INFO_LOG_LENGTH 0x8B84

typedef  char GLchar;




class TP_API tpGL2 : public tpRuntimeLoader {
public:

	typedef GLuint (__TP_CALL *CreateShaderT)(GLenum shaderType);
	typedef void (__TP_CALL *DeleteShaderT)(GLenum shaderType);
	typedef void (__TP_CALL *DeleteProgramT)(GLenum program);

	typedef GLboolean (__TP_CALL *IsShaderT)(GLuint shaderHandle);
	typedef GLboolean (__TP_CALL *IsProgramT)(GLuint shaderHandle);

	typedef void (__TP_CALL *AttachShaderT)(GLuint program,GLuint shader);
	typedef void (__TP_CALL *DetachShaderT)(GLuint program,GLuint shader);

	typedef void (__TP_CALL *CompileShaderT)(GLuint shader);
	typedef void (__TP_CALL *LinkProgramT)(GLuint program);


	typedef void (__TP_CALL *UseProgramT)(GLuint shader);

	typedef void (__TP_CALL *GetShaderivT)(GLuint shader, GLenum pname, GLint* params);
	typedef void (__TP_CALL *GetProgramivT)(GLuint program, GLenum pname, GLint* params);


	typedef void (__TP_CALL *GetShaderInfoLogT)(GLuint shader, GLsizei maxLen, GLsizei* len, GLchar* log);
	typedef void (__TP_CALL *GetProgramInfoLogT)(GLuint program, GLsizei maxLen, GLsizei* len, GLchar* log);

	typedef void (__TP_CALL *ShaderSourceT)(GLuint shader, GLsizei count, const GLchar** str, const GLint* len);

	typedef GLint (__TP_CALL *GetAttribLocationT)(GLuint program, const GLchar* name);
	typedef GLint (__TP_CALL *GetUniformLocationT)(GLuint program, const GLchar *name);

	typedef void (__TP_CALL *BindAttribLocationT)(GLuint program, GLuint index, const GLchar* name);




	tpFunctoidImpl<CreateShaderT> CreateShader;
	tpFunctoidImpl<DeleteShaderT> DeleteShader;
	tpFunctoidImpl<DeleteProgramT> DeleteProgram;

	tpFunctoidImpl<IsShaderT> IsShader;
	tpFunctoidImpl<IsProgramT> IsProgram;

	tpFunctoidImpl<AttachShaderT> AttachShader;
	tpFunctoidImpl<DetachShaderT> DetachShader;
	tpFunctoidImpl<CompileShaderT> CompileShader;
	tpFunctoidImpl<LinkProgramT> LinkProgram;

	tpFunctoidImpl<UseProgramT> UseProgram;

	tpFunctoidImpl<GetShaderivT> GetShaderiv;
	tpFunctoidImpl<GetProgramivT> GetProgramiv;

	tpFunctoidImpl<GetShaderInfoLogT> GetShaderInfoLog;
	tpFunctoidImpl<GetProgramInfoLogT> GetProgramInfoLog;

	tpFunctoidImpl<ShaderSourceT> ShaderSource;


	tpFunctoidImpl<GetAttribLocationT> GetAttribLocation;
	tpFunctoidImpl<GetUniformLocationT> GetUniformLocation;
	tpFunctoidImpl<BindAttribLocationT> BindAttribLocation;



	static tpGL2* get(bool destroy = false) {
		static tpRefPtr<tpGL2> mGL2;
		if (!mGL2.isValid()) mGL2 = new tpGL2;
		if (destroy) mGL2 = 0L;
		return mGL2.get();
	}

	static tpGL2& a() { return *get(); }
};


//#define USE_DIRECT_LINKING

#if defined(USE_DIRECT_LINKING)
	#define glCall(call,...) gl##call()
#else
	tpGL2 gl2;
	#define glCall(call,...) gl##call()
#endif



tpGLProgram::tpGLProgram() : mVertexShader(0),
	mFragmentShader(0),
	mProgram(0),
	mReady(false)
{
	mVertexShader = gl2.a().CreateShader.f(GL_VERTEX_SHADER);
	mFragmentShader = gl2.a().CreateShader.f(GL_FRAGMENT_SHADER);
}


tpGLProgram::~tpGLProgram()
{
	destroy();
}

tpVoid tpGLProgram::use() const
{
	if (mReady && gl2.a().IsProgram.f(mProgram))
	{
		gl2.a().UseProgram.f(mProgram);
	}

}

tpVoid tpGLProgram::link()
{
#if defined(USE_DIRECT_LINKING)
	if (!isSupported())
	{
		tpLogError("%s - missing GLSL support",__FUNCTION__);
		return;
	}

	mProgram = glCreateProgram();
	glAttachShader(mProgram,mVertexShader);
	glAttachShader(mProgram,mFragmentShader);
	glLinkProgram(mProgram);

	tpLogMessage("%s - %d (%d)",__FUNCTION__,mProgram,glIsProgram(mProgram));

	mReady = true;

	this->log(mProgram);
#endif
}

void tpGLProgram::log( tpUInt obj )
{
	if (!isSupported())
	{
		tpLogError("%s - missing GLSL support",__FUNCTION__);
		return;
	}

//	const tpUInt GL_INFO_LOG_LENGTH = 35716;
	tpInt maxLen(0), infoLogLen(0);
	if (gl2.a().IsShader.f(obj))
	{

		gl2.a().GetShaderiv.f(obj,GL_INFO_LOG_LENGTH,&maxLen);
		GLchar* infoLog = new GLchar[maxLen];
		gl2.a().GetShaderInfoLog.f(obj, maxLen, &infoLogLen, infoLog);

		if (infoLogLen)
		{
			tpString log2string;
			log2string.set((tpChar*)&infoLog[0],infoLogLen);

			tpLogMessage("%s\n%s\n",__FUNCTION__,log2string.c_str());
		}
		delete [] infoLog;

	} else
	{
		gl2.a().GetProgramiv.f(obj,GL_INFO_LOG_LENGTH,&maxLen);
		GLchar* infoLog = new GLchar[maxLen];
		gl2.a().GetProgramInfoLog.f(obj, maxLen, &infoLogLen, infoLog);

		if (infoLogLen)
		{
			tpString log2string;
			log2string.set(&infoLog[0],infoLogLen);
			tpLogMessage("%s\n%s\n",__FUNCTION__,log2string.c_str());
		}
		delete [] infoLog;

	}
}

tpVoid tpGLProgram::compile()
{
	if (!isSupported())
	{
		tpLogError("%s - missing GLSL support",__FUNCTION__);
		return;
	}

	int success(0);
	gl2.a().CompileShader.f(mVertexShader);
	gl2.a().GetShaderiv.f(mVertexShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) log(mVertexShader);
	else tpLogNotify("%s - vertex shader compilation successful",__FUNCTION__);

	success = 0;
	gl2.a().CompileShader.f(mFragmentShader);
	gl2.a().GetShaderiv.f(mFragmentShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) log(mFragmentShader);
	else tpLogNotify("%s - fragment shader compilation successful",__FUNCTION__);
}

tpVoid tpGLProgram::destroy()
{
	gl2.a().DeleteProgram.f(mProgram);
	gl2.a().DeleteShader.f(mVertexShader);
	gl2.a().DeleteShader.f(mFragmentShader);

	mReady = false;
}

tpInt tpGLProgram::getUniformLocation(const tpString& name)
{
	return gl2.a().GetUniformLocation.f(getProgramObject(),name.c_str());
}

tpInt tpGLProgram::getAttributeLocation( const tpChar* name )
{
	return gl2.a().GetAttribLocation.f(getProgramObject(),name);
}

tpVoid tpGLProgram::bindAttributeLocation( const tpString& name, tpUInt location )
{
	gl2.a().BindAttribLocation.f(mProgram,location, (const GLchar*) name.c_str());
}


void tpGLProgram::setVertexShaderSource( const tpString& val )
{
	const GLchar* vs_src = (const GLchar*)val.c_str();
	const tpInt valLen(val.getLength());
	tpLogNotify("%s loading %d bytes into shader",__FUNCTION__,valLen);
	gl2.a().ShaderSource.f(mVertexShader, 1, &vs_src, NULL);
}



void tpGLProgram::setFragmentShaderSource( const tpString& val, bool needPrecision )
{
	tpString shader;

	// normally only in OpenGL ES - but some Emulators don't like it either
	if (needPrecision)
	{
		// we fence it with a define
		shader.append("#ifdef GL_ES\nprecision mediump float;\n#endif\n");
	}
	shader.append(val);

	const GLchar* fs_src = (const GLchar*)shader.c_str();
	const tpInt valLen(shader.getLength());
	gl2.a().ShaderSource.f(mFragmentShader, 1, &fs_src, &valLen);

}

void tpGLProgram::loadBinary( const tpArray<tpChar>& blob, tpUInt asobject )
{
	// only OpenGL ES 2.0
#if 0
	GLint shader_binary_format_count(0);
	glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS,&shader_binary_format_count);

	tpUInt obj(0);
	switch (asobject)
	{
		case 0:
			obj = m_program;
			break;
		case 1:
			obj = m_vs;
			break;
		case 2:
			obj = m_fs;
		default:
			break;
	}

	if (shader_binary_format_count > 0)
	{
		tpArray<tpInt> formats;
		formats.preallocate(shader_binary_format_count,0);
		glGetIntegerv(GL_SHADER_BINARY_FORMATS,formats.getData());

		for (tpInt format = 0; format < shader_binary_format_count; format++ )
		{
			GLenum shader_format = (GLenum)formats[format];

			tpLogMessage("%s - try to load binary in format %d/%d=0x%x",__FUNCTION__,format,shader_binary_format_count,shader_format);

			glShaderBinary(1,&obj,shader_format,blob.getData(),blob.getSize());
			if (glGetError() == 0)
			{
				tpLogNotify("%s - shader binary loaded!",__FUNCTION__);
				return;
			} else {
				tpLogMessage("%s - failed with format %d=0x%x",__FUNCTION__,format,shader_format);
			}
		}
	} else {
		tpLogMessage("%s - no shader binary formats supported!");
	}
#endif
}

void tpGLProgram::loadSource (const tpString& file, tpUInt asobject )
{
	tpLogMessage("%s - zero",__FUNCTION__);
	FILE* f = fopen(file.c_str(),"r");
	if (f)
	{
		tpArray<tpChar> blob;
		char c(0);

		while( !feof( f ) )
		{
			c = fgetc(f);
			if ( c != EOF ) blob << c;
		}
		fclose(f);
		tpLogMessage("%s - one",__FUNCTION__);

		tpString blob2string;
		blob2string.set(blob.getData(),blob.getSize());
		blob2string.substitute(EOF,' ');

		tpLogMessage("%s - one",__FUNCTION__);

		tpLogNotify("%s - loaded shader source %s with %d bytes\n\n'%s'\n\n",
			__FUNCTION__,file.c_str(),blob2string.getLength(),blob2string.c_str());

		switch (asobject)
		{
		case kLoadVertexShader:
			setVertexShaderSource(blob2string);
			break;
		case kLoadFragmentShader:
			setFragmentShaderSource(blob2string,true);
			break;
		default:
			break;
		}

	} else {
		tpLogError("%s - couldn't find shader source '%s'",__FUNCTION__,file.c_str());
	}
}

void tpGLProgram::loadBinary( const tpString& file, tpUInt asobject )
{
	FILE* f = fopen(file.c_str(),"rb");
	if (f)
	{

		tpArray<tpChar> blob;

		while (!feof(f))
		{
			blob << fgetc(f);
		}
		fclose(f);

		tpLogNotify("%s - loaded shader binary %s with %d bytes",
			__FUNCTION__,file.c_str(),blob.getSize());

		loadBinary(blob,asobject);

	} else {
		tpLogError("%s - couldn't find shader binary '%s'",__FUNCTION__,file.c_str());
	}
}

bool tpGLProgram::isSupported() const
{
	return ( gl2.a().CreateShader.isValid() && gl2.a().DeleteShader.isValid() );
}

