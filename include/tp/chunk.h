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

#ifndef TP_CHUNK_H
#define TP_CHUNK_H


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

	/**
	 * Set size of memory chunk. Please be aware
	 * that the chunk always will be resized, even
	 * if it will be small than the previous size.
	 *
	 * @param size new size of chunk
	 */
	virtual void setSize(tpSizeT size);

	/**
	 * Returns size of the chunk
	 *
	 * @return size of the memory chunk
	 */
	virtual tpSizeT getSize() const { return m_size; }

	/**
	 * Clears and frees the memory of the memory chunk
	 */
	virtual void empty();

	/**
	 * Copies the content of another memory pointer
	 * into the memory chunk. The routine expects the
	 * user to have the size of the chunk set correctly.
	 * Hence, only getSize() bytes are copied.
	 *
	 * @param ptr pointer to memory to be copied from
	 */
	void copy(const void* ptr);

protected:

	tpSizeT m_size;

};


#endif

