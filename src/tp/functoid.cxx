#include <tp/functoid.h>


void 
tpFunctoid::load( tpGetProcAddress& gpa, tpFunctoidArray& farray )
{
	for(tpFunctoidArray::iterator it = farray.begin();
		it != farray.end();
		++it)
	{
		(*it).getKey()->assign(gpa,(*it).getValue());
	}
}
