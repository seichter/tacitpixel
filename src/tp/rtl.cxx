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

#include <tp/rtl.h>


class tpLibraryGetProcAddress : public tpGetProcAddress {
	tpRefPtr<tpLibrary> mLibrary;
public:
	tpLibraryGetProcAddress(tpLibrary *lib = 0) 
		: tpGetProcAddress()
		, mLibrary(lib)
	{}

	void* operator()(const char* name)
	{
		return mLibrary.isValid() ? mLibrary->getAddress(name) : 0L;
	}

};


void tpRuntimeLoader::close()
{
	mLibrary = 0;
}

void tpRuntimeLoader::load(tpGetProcAddress& gpa)
{
	tpFunctoid::load(gpa,mFunctions);
}

bool tpRuntimeLoader::load( const tpString& name )
{
	mLibrary = tpLibrary::load(name);

	if (mLibrary.isValid())
	{

		tpLibraryGetProcAddress lgpa(mLibrary.get());

		tpFunctoid::load(lgpa,mFunctions);

		return true;
	}

	return false;
}

