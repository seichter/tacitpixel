#include <tp/functoid.h>


/*static*/ void
tpFunctoid::load(const tpLibrary* lib,tpFunctoidArray& farray)
{
	for(tpFunctoidArray::iterator it = farray.begin();
		it != farray.end();
		++it)
	{
		(*it).getKey()->assign(*lib,(*it).getValue());
	}
}
