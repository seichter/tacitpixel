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
