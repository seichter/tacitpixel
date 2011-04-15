/*
 * Twisted Pair Visualization Engine
 *
 * Copyright (c) 1999-2009 Hartmut Seichter 
 * 
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the Twisted Pair License (TPL) version 1.0 or (at your option) 
 * any later version. The full license text is available in the LICENSE file 
 * included with this distribution, and on the technotecture.com website
 *
 */


#include <tp/module.h>
#include <tp/log.h>

tpModuleManager::tpModuleManager() : tpReferenced() {
}
	
tpModuleManager::~tpModuleManager()
{
}

tpModuleManager* tpModuleManager::get( bool destroy )
{
	static tpRefPtr<tpModuleManager> s_modulemanager( (destroy) ? 0L : new tpModuleManager() );

	return s_modulemanager.get();
} 

tpVoid tpModuleManager::add( tpReferenced* module )
{
	m_modules.add(module);
}


tpVoid tpModuleManager::remove( tpReferenced* module )
{
	// we need to find the module and delete our own
	// reference to it. This forces the module 
	// to be deleted in the correct context
	tpSizeT idx = m_modules.find(module);
	m_modules[idx] = 0;
	m_modules.erase(idx);
}


const tpModuleList& tpModuleManager::getModules() const
{
	return m_modules;
}


//////////////////////////////////////////////////////////////////////////

