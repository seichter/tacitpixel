#include <tp/egl.h>

tpEGL::tpEGL()
{
}

tpEGL* tpEGL::get( bool destroy /*= false*/ )
{
	static tpRefPtr<tpEGL> mEGL;
	if (!mEGL.isValid()) mEGL = new tpEGL;
	if (destroy) mEGL = 0L;
	return mEGL.get();
}
