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

#ifndef _TP_DYNAMICLIBRARY_H_
#define _TP_DYNAMICLIBRARY_H_


#include <tp/globals.h>
#include <tp/array.h>
#include <tp/string.h>
#include <tp/referenced.h>

//


class TP_API tpLibrary : public tpReferenced {

	void* m_handle;

public:

	TP_TYPE_DECLARE;

	/**
	 * Standard C'tor
	 */
	tpLibrary();

	/**
	 * Copy C'tor	
	 */
	tpLibrary(const tpLibrary& lib);

	/**
	 * Opens a file as a dynamic library.
	 *
	 *	\param file filename for the library
	 *	\return true if successful
	 */
	bool open(const tpString& file);
	
	/**
	 * Closes the dynamic library. This call tries to
	 * unload the library if possible.
	 *
	 *	\return true if successful
	 */
	bool close();

	/**
	 * Get an address from the library. This function 
	 * returns a raw pointer to the address of the 
	 * function. Be warned, always cast to the correct 
	 * type right at that call. These addresses can't
	 * be stored or copied.
	 *	
	 * \param funcName name of the function/symbol
	 * \return address if successful, otherwise 0x0
	 */
	void* getAddress(const tpString& funcName) const;

	/**
	 * Convienience function for checking if the library
	 * was sucessfully loaded.
	 *
	 * \return true if the library was loaded successfully
	 */
	bool isValid() const { return (m_handle != 0x0000); }

	/**
	 * Returns the actual name/path of the library loaded.
	 *
	 *	\return full path of the module
	 */
	tpString getModuleName() const;

	/**
	 * Returns a pointer to the actual handle of the 
	 * library loaded.
	 *	
	 * \return address or 0x0 if not a valid library.
	 */
	void* getHandle() const;

	/**
	 * Convienience function for loading libraries
	 * 
	 * \param name file name 
	 * \return an object with the library linked or 0
	 */
	static tpLibrary* load( const tpString& name );

protected:

	virtual ~tpLibrary();	 
};

#endif
