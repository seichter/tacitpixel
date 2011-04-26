/*
* Twisted Pair Visualization Engine
*
* Copyright (c) 1999-2009 Hartmut Seichter 
* 
* This library is open source and may be redistributed and/or modified under  
* the terms of the Twisted Pair License (TPL) version 1.0 or (at your option) 
* any later version. The full license text is available in the LICENSE file 
* included with this distribution, and on the technotecture.com website.
*
*/

#ifndef TPMODULE_H
#define TPMODULE_H

#include <tp/globals.h>
#include <tp/referenced.h>
#include <tp/array.h>
#include <tp/refptr.h>

typedef tpArray< tpRefPtr< tpReferenced > > tpModuleList;

/**
 * \class tpModuleManager
 *
 * tpModuleManager is the big grand central for fetching external 
 * modules for Twisted Pair
 */
class TP_API tpModuleManager : public tpReferenced {
protected:

	tpModuleManager();

	virtual ~tpModuleManager();

	tpModuleList m_modules;

public:

	void add(tpReferenced* module);

	void remove(tpReferenced* module );

	const tpModuleList& getModules() const;	

	static tpModuleManager* get(bool destroy = false);
};

template <class T> class tpModuleInitializer {
public:

	tpModuleInitializer() : m_module(0)
	{
		m_module = new T;
		tpModuleManager::get()->add(m_module.get());
	}

	~tpModuleInitializer()
	{
		tpModuleManager::get()->remove(m_module.get());
	}

	T* get() { return m_module.get(); }

protected:

	tpRefPtr<T> m_module;
};

extern "C"
{
    typedef void (*tpModuleFunction)();
}

struct tpModuleFunctionProxy
{
    tpModuleFunctionProxy(tpModuleFunction f) { f(); }
};

#define TP_MODULE_USE(name) \
    extern "C" tpVoid tpModule_##name(void); \
    static tpModuleFunctionProxy tpModuleProxy_##name(&tpModule_##name);

#define TP_MODULE_REGISTER(name,klass) \
    extern "C" tpVoid tpModule_##name(void) {} \
    static tpModuleInitializer<klass> gs_module_##klass;


#define TP_MODULE_HOOK(name) \
	TP_API tpVoid tpModuleHook##name() {}

#endif

 
