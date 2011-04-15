#include "tpGLShader.h"


#include "tpGL.h"

#define GL_COMPILE_STATUS 0x8B81
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31 

#define GL_NUM_SHADER_BINARY_FORMATS 0x8DF9
#define GL_SHADER_BINARY_FORMATS 0x8DF8


tpGLProgram::tpGLProgram() : m_vs(0), 
	m_fs(0), 
	m_program(0), 
	m_ready(false)
{
	m_vs = tpGL::glCreateShader(GL_VERTEX_SHADER);
	m_fs = tpGL::glCreateShader(GL_FRAGMENT_SHADER);
}


tpGLProgram::~tpGLProgram()
{
	destroy();
}

tpVoid tpGLProgram::use() const
{
	if (m_ready && tpGL::glIsProgram(m_program))
	{
		tpGL::glUseProgram(m_program);
		TP_REPORT_GLERROR()

	}
}

tpVoid tpGLProgram::link()
{
	if (!isSupported())
	{
		tpLogError("%s - missing GLSL support",__FUNCTION__);
		return;
	}

	m_program = tpGL::glCreateProgram();
	tpGL::glAttachShader(m_program,m_vs);
	tpGL::glAttachShader(m_program,m_fs);
	tpGL::glLinkProgram(m_program);

	tpLogMessage("%s - %d (%d)",__FUNCTION__,m_program,tpGL::glIsProgram(m_program));

	m_ready = true;

	this->log(m_program);
}

void tpGLProgram::log( tpUInt obj )
{
	if (!isSupported())
	{
		tpLogError("%s - missing GLSL support",__FUNCTION__);
		return;
	}

	const tpUInt GL_INFO_LOG_LENGTH = 35716;
	tpInt maxLen(0), infoLogLen(0);
	if (tpGL::glIsShader(obj))
	{

		tpGL::glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLen);
		GLchar* infoLog = new GLchar[maxLen];
		tpGL::glGetShaderInfoLog(obj, maxLen, &infoLogLen, infoLog);

		if (infoLogLen)
		{
			tpString log2string(&infoLog[0],infoLogLen);
			tpLogMessage("%s\n%s\n",__FUNCTION__,log2string.c_str());
		}
		delete [] infoLog;

	} else 
	{
		tpGL::glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLen);
		GLchar* infoLog = new GLchar[maxLen];
		tpGL::glGetProgramInfoLog(obj, maxLen, &infoLogLen, infoLog);

		if (infoLogLen)
		{
			tpString log2string(&infoLog[0],infoLogLen);
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
	tpGL::glCompileShader(m_vs);
	tpGL::glGetShaderiv(m_vs, GL_COMPILE_STATUS, &success);
	if (success == tpGL::_FALSE) log(m_vs); else tpLogNotify("%s - vertex shader compilation successful",__FUNCTION__);

	success = 0;
	tpGL::glCompileShader(m_fs);
	tpGL::glGetShaderiv(m_fs, GL_COMPILE_STATUS, &success);
	if (success == tpGL::_FALSE) log(m_fs); else tpLogNotify("%s - fragment shader compilation successful",__FUNCTION__);
}

tpVoid tpGLProgram::destroy()
{
	tpGL::glDeleteProgram(m_program);
	tpGL::glDeleteShader(m_vs);
	tpGL::glDeleteShader(m_fs);

	m_ready = false;
}

tpInt tpGLProgram::getUniformLocation(const tpString& name)
{
	return tpGL::glGetUniformLocation(getProgramObject(),name.c_str());
}

tpInt tpGLProgram::getAttributeLocation( const tpChar* name )
{
	return tpGL::glGetAttribLocation(getProgramObject(),name);
}

tpVoid tpGLProgram::bindAttributeLocation( const tpString& name, tpUInt location )
{
	tpGL::glBindAttribLocation(m_program,location, (const GLchar*) name.c_str() ); 
}


void tpGLProgram::setVertexShaderSource( const tpString& val )
{
	const GLchar* vs_src = (const GLchar*)val.c_str();
	const tpInt valLen(val.getLength());
	tpLogNotify("%s loading %d bytes into shader",__FUNCTION__,valLen);
	tpGL::glShaderSource(m_vs, 1, &vs_src, NULL);
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
	tpGL::glShaderSource(m_fs, 1, &fs_src, &valLen);

}

void tpGLProgram::loadBinary( const tpArray<tpChar>& blob, tpUInt asobject )
{
	GLint shader_binary_format_count(0);
	tpGL::glGetIntegerv(GL_NUM_SHADER_BINARY_FORMATS,&shader_binary_format_count);

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
		tpGL::glGetIntegerv(GL_SHADER_BINARY_FORMATS,formats.getData());

		for (tpInt format = 0; format < shader_binary_format_count; format++ )
		{	
			GLenum shader_format = (GLenum)formats[format];
			
			tpLogMessage("%s - try to load binary in format %d/%d=0x%x",__FUNCTION__,format,shader_binary_format_count,shader_format);

			tpGL::glShaderBinary(1,&obj,shader_format,blob.getData(),blob.getSize());
			if (tpGL::glGetError() == 0)
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

		tpString blob2string(blob.getData(),blob.getSize());
		blob2string.subst(EOF,' ');

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
		0 != tpGL::glCompileShader &&
		0 != tpGL::glShaderSource ); 
}

