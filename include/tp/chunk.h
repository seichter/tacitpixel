/*

    libTAP - The Architectural Playground
    a minimalistic collaborative virtual environment framework

    Copyright (C) 1999-2003 Hartmut Seichter

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA


*/

#ifndef TPCHUNK_H
#define TPCHUNK_H


#include <tp/types.h>

/*!	\class tpChunk
	\brief a very simple memory chunk handler for naive types

	Wraps up allocating and deleting memory for random access. 
*/

class tpChunk {
public:

	//! Standard c'tor.
	tpChunk(tpSizeT size = 0);

	//! Copy c'tor.
	tpChunk(const tpChunk& origin);

	//! D'tor.
	~tpChunk();
	
	//! copy data
	void copy(tpChunk& dest) const;

	//! merge operator
	tpChunk& operator += (const tpChunk& merge);
	
	//! assignment operator
	tpChunk& operator  = (const tpChunk& assign);
	
	//! Get data pointer
	const void* getData() const { return m_ptr; }
	
	//! Get data pointer
	void* getData() { return m_ptr; }
	
	//! set raw data size
	void setSize(tpSizeT size);

	//! Remove all data
	void empty();
	
	//! typed pointer
	template <typename T>
	T* ptr() { return static_cast<T*>(m_ptr); }
	
	//! typed pointer
	template <typename T>
	const T* ptr() const { return static_cast<T*>(m_ptr); }
	
	//! get an item
	template <typename T> 
	T& at(tpSizeT idx) { return this->ptr<T>()[idx]; }
	
	//! get an item
	template <typename T> 
	const T& at(tpSizeT idx) const { return this->ptr<T>()[idx]; }
	
	
	template <typename T>
	void reserve(tpSizeT size) { this->reserve<sizeof(T)>(size); }
	
	template <int itemsize>
	void reserve(tpSizeT size) { setSize(itemsize * size ); }
	
protected:
	
	void* m_ptr;

};



#endif

