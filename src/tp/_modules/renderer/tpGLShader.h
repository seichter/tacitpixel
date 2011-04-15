
#ifndef TP_GLSHADER_H 
#define TP_GLSHADER_H 1

#include <tp/referenced.h>
#include <tp/log.h>
#include <tp/string.h>

class tpGLProgram : public tpReferenced {
protected:

	tpUInt m_vs;
	tpUInt m_fs;
	tpUInt m_program;

	bool m_ready;

public:

	enum {
		LOAD_AS_PROGRAM	= 0x0,
		LOAD_AS_VERTEX_SHADER,
		LOAD_AS_FRAGMENT_SHADER
	};

	void setVertexShaderSource(const tpString& val);

	void setFragmentShaderSource( const tpString& val, bool needPrecision );

	bool isReady() const { return m_ready; }

	bool isSupported() const;

	tpGLProgram();


	tpVoid bindAttributeLocation( const tpString& name, tpUInt location );

	tpUInt getProgramObject() const { return m_program; }

	tpInt getAttributeLocation(const tpChar* name);
	tpInt getUniformLocation(const tpString& name);

	tpVoid destroy();

	tpVoid compile();

	tpVoid link();


	tpVoid use() const;

	void log(tpUInt obj);

	void loadBinary( const tpArray<tpChar>& blob, tpUInt asobject );
	void loadBinary( const tpString& file, tpUInt asobject );
	void loadSource (const tpString& file, tpUInt asobject );

protected:
	virtual ~tpGLProgram();
};

#endif
