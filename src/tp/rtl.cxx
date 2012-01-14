#include <tp/rtl.h>

void tpRTL::close()
{
	mLibrary = 0L;
}

bool tpRTL::open( const tpString& name )
{
	mLibrary = tpLibrary::load(name);

	if (mLibrary.isValid())
	{

		return true;
	}

	return false;
}
