#ifndef TP_FUNCTOID_H
#define TP_FUNCTOID_H

#include <tp/globals.h>
#include <tp/map.h>
#include <tp/string.h>
#include <tp/library.h>

class tpFunctoid;

typedef tpMap<tpFunctoid*,tpString> tpFunctoidArray;


struct TP_API tpFunctoid {

	virtual void assign(const tpLibrary& lib, const tpString& name) = 0;

	static void load(const tpLibrary* lib,tpFunctoidArray& farray);
};

template <typename T>
struct tpFunctoidImpl : tpFunctoid
{
	T f;

	void assign(const tpLibrary& lib, const tpString& name)
	{
		f = reinterpret_cast<T>(lib.getAddress(name));
	}
};



#endif
