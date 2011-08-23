/* 
 * Copyright (C) 1999-2011 Hartmut Seichter
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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
    extern "C" void tpModule_##name(void); \
    static tpModuleFunctionProxy tpModuleProxy_##name(&tpModule_##name);

#define TP_MODULE_REGISTER(name,klass) \
    extern "C" void tpModule_##name(void) {} \
    static tpModuleInitializer<klass> gs_module_##klass;

#endif

 
