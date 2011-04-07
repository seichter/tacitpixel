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

#ifndef TPDYNAMICLIBRARY_H
#define TPDYNAMICLIBRARY_H


#include <tp/globals.h>
#include <tp/array.h>
#include <tp/string.h>
#include <tp/referenced.h>


typedef tpArray<tpString> tpLibrarySearchPaths;

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
	 * function.
	 *	
	 * \param funcName name of the function/symbol
	 * \return address if successful, otherwise 0x0
	 */
	void* getAddress(const tpString& funcName);

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
