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

#ifndef TPSCOPEPTR_H
#define TPSCOPEPTR_H

#include "tp/types.h"

/*!
	\class tpScopePtr
	\brief a smartpointer implementation
	
	This kind of referencing is needed for handling
	referenced nodes in a scenegraph.
*/

template <class T> class tpScopePtr
{
public:
	/*! brief standard c'tor		
	*/
	tpScopePtr();

	/*! c'tor with initializing pointer
		\param ptr pointer to manage
	*/
	tpScopePtr(T* ptr);

	/*! copy c'tor
		\param ref reference pointer to copy
	*/
	tpScopePtr(const tpScopePtr& ref);

	/*! d'tor
	*/
	virtual ~tpScopePtr();

	/*! get the pointer		
	*/
	T* operator->() const;

	/*! unref the managed pointer
		\result 
	*/
	T& operator *() const;

	/*! copy operator 
		\param ptr pointer to manage
		\result reference to this ptr
	*/
	tpScopePtr<T>& operator=(T* ptr);

	void set(T* ptr);

	//! 
	tpScopePtr<T>& operator=(const tpScopePtr& ref);


	bool isValid() const;
	

protected:
	T* m_ptr;
	void cleanup();

};

// ----------------------------------------------------------------------------------------

template <class T> inline tpScopePtr<T>::tpScopePtr() : m_ptr(NULL)
{
};

template <class T> inline tpScopePtr<T>::tpScopePtr(T* ptr) : m_ptr(ptr)
{
};

template <class T> inline tpScopePtr<T>::tpScopePtr(const tpScopePtr& ref)
	: m_ptr(ref.m_ptr)
{
};

template <class T> inline tpScopePtr<T>::~tpScopePtr()
{
	this->cleanup();
};

template <class T> inline T* tpScopePtr<T>::operator->() const
{
	return m_ptr;
};

template <class T> inline T& tpScopePtr<T>::operator *() const
{
	return *m_ptr;
};


template <class T> inline tpScopePtr<T>& tpScopePtr<T>::operator=(T* ptr)
{
	cleanup();
	m_ptr = ptr;
	return *this;
};

template <class T> inline void tpScopePtr<T>::set(T* ptr)
{
	cleanup();
	m_ptr = ptr;
};


template <class T> inline tpScopePtr<T>& tpScopePtr<T>::operator=(const tpScopePtr<T>& ref)
{
	if (this->m_ptr == ref.m_ptr) return *this;
	if (ref.m_ptr) m_ptr = ref.m_ptr;
	return *this;
};

template <class T> inline void tpScopePtr<T>::cleanup()
{
	if (m_ptr != NULL) delete m_ptr;
};

template <class T> inline bool tpScopePtr<T>::isValid() const
{
	return (m_ptr != NULL);
};

#endif
