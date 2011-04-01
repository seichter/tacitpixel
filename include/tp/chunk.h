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
	\brief a very simple memory handler

	Wraps up allocating and deleting memory for
	random access. It can not allocate memory
	dynamicly.
*/

template <class T> class tpChunk
{
public:

	//! Standard c'tor. (size should be > 0)
	tpChunk();

	//! Standard c'tor. (size should be > 0)
	tpChunk(tpUInt size);

	//! Copy c'tor.
	tpChunk(const tpChunk<T>& origin);

	//! D'tor.
	virtual ~tpChunk();

	//! Access memory via index. No range check will be done!
	T& operator [] (tpUInt index);

	T operator [] (tpUInt index) const;

	// tpChunk<T> operator + (const tpChunk& merge);

	tpChunk<T>& operator += (const tpChunk& merge);


	tpChunk<T>& append(const T* mem, tpULong size);

	
	//! Get data pointer. You can not write to that pointer!
	T* getData() const;

	//! Get the allocated size. 
	tpUInt getSize() const;


	//! Remove all data
	tpVoid empty();
	
	//! overwrite all data
	tpVoid clear(T data);

	//! Allocate data
	tpVoid allocate(tpUInt size);

	
	T* m_data;

	tpUInt m_size;
};


// -----------------------------------------------------------


template <class T> inline tpChunk<T>::tpChunk() 
	: m_data(0),
	m_size(0)
{
}


template <class T> inline tpChunk<T>::tpChunk(tpUInt size)
{
	m_data = new T[size];
	m_size = size;
};


template <class T> inline tpChunk<T>::tpChunk(const tpChunk<T>& origin)
{
	m_size = 0;
	m_data = 0;
	
	if (origin.m_size)
	{
		m_data = new T[origin.getSize()]; 
		m_size = origin.m_size;
		memcpy(m_data,origin.m_data,m_size);
	};
};



template <class T> inline tpChunk<T>::~tpChunk()
{
	delete [] m_data;
	m_data = NULL;
};

template <class T> inline tpUInt tpChunk<T>::getSize() const
{
	return m_size;
};

template <class T> inline T& tpChunk<T>::operator [] (tpUInt index)
{
	return m_data[index];
};

template <class T> inline T tpChunk<T>::operator [] (tpUInt index) const
{
	return m_data[index];
};

template <class T> inline T* tpChunk<T>::getData() const
{
	return m_data;
};


template <class T> inline tpVoid tpChunk<T>::empty()
{
	delete [] m_data;
	m_size = 0;
};


template <class T> inline tpVoid tpChunk<T>::clear(T data)
{
	for (tpUInt i = 0; i < m_size; ++i) m_data[i] = data;
};

template <class T> inline tpVoid tpChunk<T>::allocate(tpUInt size)
{
	if (m_size) empty();
	m_data = new T[size];
	m_size = size;
};

template <typename T> inline tpChunk<T>& tpChunk<T>::append(const T* mem, tpULong size)
{
	T *_new = new T[m_size + size];

	for (tpULong i = 0; i < m_size; ++i) _new[i] = m_data[i];
	for (tpULong j = 0; j < size; j++) _new[m_size + j] = mem[j];

	delete [] m_data;

	m_data =_new;

	m_size += size;

	return *this;

};

template <typename T> inline tpChunk<T>& tpChunk<T>::operator += (const tpChunk& merge)
{

	T *_new = new T[m_size + merge.m_size];

	for (tpULong i = 0; i < m_size; ++i) _new[i] = m_data[i];
	for (tpULong j = 0; j < merge.m_size; j++) _new[m_size + j] = merge.m_data[j];

	delete [] m_data;

	m_data =_new;

	m_size += merge.m_size;

	return *this;
};







#endif

