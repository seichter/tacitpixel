#include <tp/filename.h>

tpFilename::tpFilename() : tpString()
{
}

tpFilename::tpFilename(const tpFilename& c) : tpString(c)
{
}

tpFilename
tpFilename::getExtension() const
{
	return tpFilename();
	//return tpString("");
	//return tpFilename(this->afterLast('.'));
}
