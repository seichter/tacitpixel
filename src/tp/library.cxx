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
#include <tp/library.h>
#include <tp/config.h>
#include <tp/log.h>
#include <tp/system.h>

#if defined(WIN32) || defined(WINCE)
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#elif defined(HAVE_DLFCN_H)
#	include <dlfcn.h>
#endif

tpLibrary::tpLibrary() : m_handle(0) {
}

tpLibrary::tpLibrary(const tpLibrary& lib) : m_handle(lib.m_handle) {
}

tpLibrary::~tpLibrary() 
{
	close();
}

void*
tpLibrary::getHandle() const {
	return m_handle;
}

bool
tpLibrary::close()
{
	bool result(false);
#if defined(WIN32)
	result = (TRUE == FreeLibrary(static_cast<HMODULE>(m_handle)));
#elif defined(HAVE_DLFCN_H)
	result = (0 == dlclose(m_handle));
	
	/*
	if (!result)
	{
		char* errstr;
		errstr = tpStrDup(dlerror());
		if (errstr != NULL) {
			tpLogError("%s: %s",__FUNCTION__,errstr);
		}
	}
	*/
#endif

	tpLogNotify("%s - release %s",__FUNCTION__,result ? "ok" : "failed");

	return result;
}


tpString
tpLibrary::getModuleName() const {

	tpString result;
#if defined(_WIN32) || defined(_WIN32_WCE)
	if (m_handle) {

		LPTSTR name = (LPTSTR)malloc(sizeof(LPTSTR) * MAX_PATH);
		GetModuleFileName(static_cast<HMODULE>(m_handle),name,MAX_PATH);

		result.set(name);

		free(name);
	}
	
#endif

	return result;

}


bool
tpLibrary::open( const tpString& file )
{
	
	tpString actual_name = file; 

#if defined(WIN32) || defined(WINCE)
	m_handle = ::LoadLibraryA(actual_name.c_str());

//#if defined(WIN32)
//	if (m_handle) 
//	{		
//		typedef void (*tpHookFunc)(void);
//		tpHookFunc hookFunc = (tpHookFunc)getAddress("tpWinCEHook");
//		if (hookFunc) hookFunc();
//	}
//#endif

#elif defined(HAVE_DLFCN_H)

	m_handle = dlopen(actual_name.c_str(), RTLD_GLOBAL | RTLD_NOW) ;

	if (!m_handle)
	{
		actual_name = file + ".so";
		m_handle = dlopen(actual_name.c_str(), RTLD_GLOBAL | RTLD_NOW) ;
	}
	
	if (!m_handle)
	{
		actual_name = file + ".dylib";
		m_handle = dlopen(actual_name.c_str(), RTLD_GLOBAL | RTLD_NOW) ;

	}

	if (!m_handle)
	{
		tpString errstr(dlerror());
		if (!errstr.isEmpty()) {
			tpLogError("%s: %s",__FUNCTION__,errstr.c_str());
		};
	};
#endif

	tpLogNotify("%s '%s' handle: 0x%X '%s'",__FUNCTION__,
		actual_name.c_str(),
		m_handle,
		getModuleName().c_str());

	return (m_handle != 0x0);

}

void*
tpLibrary::getAddress( const tpString& funcName ) const
{
	void* address(0L);

	if (m_handle) {
#if defined(_WIN32) || defined(_WIN32_WCE)
		address = ::GetProcAddress(static_cast<HMODULE>(m_handle),funcName.c_str());
#elif defined(HAVE_DLFCN_H)
		address = dlsym(m_handle,funcName.c_str());
#endif
		
	}

	tpLogNotify("%s symbol:%s @0x%X",__FUNCTION__,funcName.c_str(),address);

	return address;
}


static tpArray<tpString> gs_libsearchpaths;

tpLibrary*
tpLibrary::load( const tpString& name )
{
	tpLibrary* lib = new tpLibrary();

	if ( false == lib->open(name) )
	{
		delete lib;
		lib = 0L;
	}
	return lib;
}

TP_TYPE_REGISTER(tpLibrary,tpReferenced,Library);
