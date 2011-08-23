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

