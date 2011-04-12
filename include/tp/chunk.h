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
#include <tp/globals.h>

/*!	\class tpChunk
	\brief a very simple memory chunk handler for naive types

	Wraps up allocating and deleting memory for random access. 
*/

class TP_API tpChunk {
	
	//! Copy c'tor.
	tpChunk(const tpChunk& origin) {};
	
	//! assignment
	tpChunk& operator = (const tpChunk& rhs) { return *this; }
	
public:

	//! Standard c'tor.
	explicit tpChunk(tpSizeT size = 0);

	//! D'tor.
	virtual ~tpChunk();
	
	//! set raw data size
	virtual void setSize(tpSizeT size);

	//! Remove all data
	virtual void empty();
		
	//! Get data pointer
	const void* getData() const { return m_ptr; }
	
	//! Get data pointer
	void* getData() { return m_ptr; }
	
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

class TP_API tpSizedChunk : public tpChunk {
public: 

	virtual void setSize(tpSizeT size);

	virtual tpSizeT getSize() const { return m_size; }
	
	virtual void empty();
	
	void copy(const void* ptr, tpSizeT size);

protected:

	tpSizeT m_size;
	
};


#endif

