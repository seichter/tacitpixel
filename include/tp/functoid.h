#ifndef TP_FUNCTOID_H
#define TP_FUNCTOID_H

#include <tp/globals.h>
#include <tp/map.h>
#include <tp/string.h>

class tpFunctoid;

typedef tpMap<tpFunctoid*,tpString> tpFunctoidArray;


struct TP_API tpGetProcAddress {
	virtual void* operator()(const char* name) = 0;
};


class TP_API tpFunctoid {
public:

	virtual void assign(tpGetProcAddress& gpa,const tpString& name) = 0;

	static void load(tpGetProcAddress& gpa,tpFunctoidArray& farray);
};

template <typename T>
struct tpFunctoidImpl : tpFunctoid
{
	T f;

	void assign(tpGetProcAddress& gpa, const tpString& name)
	{
		f = reinterpret_cast<T>(gpa(name.c_str()));
	}
};



#endif
