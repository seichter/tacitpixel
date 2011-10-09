#include <tp/arguments.h>



tpArguments::tpArguments(int *argc, char* argv[])
{
	if (argc)
	{
		for (tpUInt i = 0;
			 i < *argc;
			 ++i)
		{
			this->mArguments.add(argv[i]);
		}
	}
}

bool
tpArguments::get(const tpString& param,tpString& value) const
{
	for (tpStringArray::const_iterator i = mArguments.begin();
		 i != mArguments.end();
		 ++i)
	{
		if (param == (*i))
		{
			if (i != &mArguments.back()) { i++; value = (*i); }
			return true;
		}
	}
	return false;
}

bool
tpArguments::get(const tpString& param) const
{
	for (tpStringArray::const_iterator i = mArguments.begin();
		 i != mArguments.end();
		 ++i)
	{
		if (param == (*i))
		{
			return true;
		}
	}
	return false;
}




