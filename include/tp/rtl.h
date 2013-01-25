/*
 * Copyright (C) 1999-2013 Hartmut Seichter
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

#ifndef TP_RTL_H
#define TP_RTL_H

#include <tp/referenced.h>
#include <tp/library.h>
#include <tp/refptr.h>
#include <tp/functoid.h>


class TP_API tpRuntimeLoader : public tpReferenced {
protected:

	tpFunctoidArray mFunctions;
	tpRefPtr<tpLibrary> mLibrary;

public:

	tpFunctoidArray& getFunctions() { return mFunctions; }


	/**
	 * Opens library and loads signatures
	 *
	 * @param name filename of the library to load
	 * @return true if successful
	 */
	bool load( const tpString& name );

	/**
	 * Loads a signatures
	 *
	 * @param gpa is a runtime loader function
	 */
	void load(tpGetProcAddress& gpa);
	
	/**
	 * Closes the library and clears all pointers
	 */
	void close();

};


#endif
