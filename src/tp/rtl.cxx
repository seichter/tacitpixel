#include <tp/rtl.h>


class tpLibraryGetProcAddress : public tpGetProcAddress {
	tpRefPtr<tpLibrary> mLibrary;
public:
	tpLibraryGetProcAddress(tpLibrary *lib = 0) 
		: tpGetProcAddress()
		, mLibrary(lib)
	{}

	void* operator()(const char* name)
	{
		return mLibrary.isValid() ? mLibrary->getAddress(name) : 0L;
	}

};


void tpRuntimeLoader::close()
{
	mLibrary = 0;
}

void tpRuntimeLoader::load(tpGetProcAddress& gpa)
{
	tpFunctoid::load(gpa,mFunctions);
}

bool tpRuntimeLoader::load( const tpString& name )
{
	mLibrary = tpLibrary::load(name);

	if (mLibrary.isValid())
	{

		tpLibraryGetProcAddress lgpa(mLibrary.get());

		tpFunctoid::load(lgpa,mFunctions);

		return true;
	}

	return false;
}

