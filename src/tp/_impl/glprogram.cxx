#include "glprogram.h"

#include "tp/host.h"

#if defined(TP_OS_OSX)
	#include <OpenGL/OpenGL.h>
	#include <OpenGL/gl3.h>
	#include <OpenGL/gl3ext.h>
#elif defined(TP_OS_LINUX)
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

//#define GL_COMPILE_STATUS 0x8B81
//#define GL_FRAGMENT_SHADER 0x8B30
//#define GL_VERTEX_SHADER 0x8B31

//#define GL_NUM_SHADER_BINARY_FORMATS 0x8DF9
//#define GL_SHADER_BINARY_FORMATS 0x8DF8


tpGLProgram::tpGLProgram() : mVertexShader(0),
	mFragmentShader(0),
	mProgram(0),
	mReady(false)
{
	mVertexShader = glCreateShader(GL_VERTEX_SHADER);
	mFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
}


tpGLProgram::~tpGLProgram()
{
	destroy();
}

tpVoid tpGLProgram::use() const
{
	if (mReady && glIsProgram(mProgram))
	{
		glUseProgram(mProgram);
//		TP_REPORT_GLERROR()

	}
}

tpVoid tpGLProgram::link()
{
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
	if (glIsShader(obj))
	{

		glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLen);
		GLchar* infoLog = new GLchar[maxLen];
		glGetShaderInfoLog(obj, maxLen, &infoLogLen, infoLog);

		if (infoLogLen)
		{
			tpString log2string;
			log2string.set((tpChar*)&infoLog[0],infoLogLen);

			tpLogMessage("%s\n%s\n",__FUNCTION__,log2string.c_str());
		}
		delete [] infoLog;

	} else
	{
		glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLen);
		GLchar* infoLog = new GLchar[maxLen];
		glGetProgramInfoLog(obj, maxLen, &infoLogLen, infoLog);

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
	glCompileShader(mVertexShader);
	glGetShaderiv(mVertexShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) log(mVertexShader); else tpLogNotify("%s - vertex shader compilation successful",__FUNCTION__);

	success = 0;
	glCompileShader(mFragmentShader);
	glGetShaderiv(mFragmentShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) log(mFragmentShader); else tpLogNotify("%s - fragment shader compilation successful",__FUNCTION__);
}

tpVoid tpGLProgram::destroy()
{
	glDeleteProgram(mProgram);
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragmentShader);

	mReady = false;
}

tpInt tpGLProgram::getUniformLocation(const tpString& name)
{
	return glGetUniformLocation(getProgramObject(),name.c_str());
}

tpInt tpGLProgram::getAttributeLocation( const tpChar* name )
{
	return glGetAttribLocation(getProgramObject(),name);
}

tpVoid tpGLProgram::bindAttributeLocation( const tpString& name, tpUInt location )
{
	glBindAttribLocation(mProgram,location, (const GLchar*) name.c_str() );
}


void tpGLProgram::setVertexShaderSource( const tpString& val )
{
	const GLchar* vs_src = (const GLchar*)val.c_str();
	const tpInt valLen(val.getLength());
	tpLogNotify("%s loading %d bytes into shader",__FUNCTION__,valLen);
	glShaderSource(mVertexShader, 1, &vs_src, NULL);
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
	glShaderSource(mFragmentShader, 1, &fs_src, &valLen);

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
		case LOAD_AS_VERTEX_SHADER:
			setVertexShaderSource(blob2string);
			break;
		case LOAD_AS_FRAGMENT_SHADER:
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
	return (
		0 != glCompileShader &&
		0 != glShaderSource );
}

