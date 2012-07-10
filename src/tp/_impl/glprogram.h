
#ifndef TP_GLSHADER_H
#define TP_GLSHADER_H 1

#include <tp/referenced.h>
#include <tp/log.h>
#include <tp/string.h>

class tpGLProgram : public tpReferenced {
protected:

	tpUInt mVertexShader;
	tpUInt mFragmentShader;
	tpUInt mProgram;

	bool mReady;

public:

	enum {
		kLoadProgram	= 0x0,
		kLoadVertexShader,
		kLoadFragmentShader
	};

	void setVertexShaderSource(const tpString& val);

	void setFragmentShaderSource( const tpString& val, bool needPrecision );

	bool isReady() const { return mReady; }

	bool isSupported() const;

	tpGLProgram();

	tpVoid bindAttributeLocation( const tpString& name, tpUInt location );

	tpUInt getProgramObject() const { return mProgram; }

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
