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

#ifndef TPREFPTR_H
#define TPREFPTR_H

#include <tp/types.h>
#include <tp/referenced.h>

template <class T> class tpRefPtr {
public:

	tpRefPtr();
	tpRefPtr(T *instance);

	tpRefPtr(const tpRefPtr&);
	~tpRefPtr();

	inline tpRefPtr& operator = (const tpRefPtr&);
	inline tpRefPtr& operator = (T*);

	inline bool isValid() const;

	inline T* operator->() const;
	inline T& operator *() const;

	inline T* get() const;

	inline bool operator == (const tpRefPtr& pr);

protected:
	T* m_ptr;

};

template <class T>
bool tpRefPtr<T>::operator==( const tpRefPtr& pr )
{
	return (m_ptr == pr.m_ptr);
}


template <class T> tpRefPtr<T>::tpRefPtr()
	: m_ptr(0L)
{
}

template <class T> tpRefPtr<T>::tpRefPtr(T *instance) : m_ptr(instance)
{
    if (m_ptr) 
	{
        m_ptr->pushRef();
    }
}


template <class T> tpRefPtr<T>::tpRefPtr(const tpRefPtr& cpy) : m_ptr(cpy.m_ptr)
{
    if (m_ptr) 
	{
        m_ptr->pushRef();
    }
}

template <class T> inline tpRefPtr<T>& tpRefPtr<T>::operator = (const tpRefPtr& rhs)
{

	if (this->m_ptr == rhs.m_ptr) return *this;

	T* _tmp_ptr = m_ptr;

	m_ptr = rhs.m_ptr;

	if (m_ptr) m_ptr->pushRef();
	if (_tmp_ptr) _tmp_ptr->popRef();

	return *this;
};


template <class T> inline tpRefPtr<T>& tpRefPtr<T>::operator = (T* rhs)
{
	if (m_ptr == rhs) return *this;

	T* _tmp_ptr = m_ptr;

	m_ptr = rhs;

	if (m_ptr) m_ptr->pushRef();
	if (_tmp_ptr) _tmp_ptr->popRef();

	return *this;
}

template <class T> inline bool tpRefPtr<T>::isValid() const
{
	return (m_ptr != 0L);
}

template <class T> inline T* tpRefPtr<T>::operator->() const
{
	return m_ptr;
}

template <class T> inline T* tpRefPtr<T>::get() const
{
	return m_ptr;
}

template <class T> inline T& tpRefPtr<T>::operator *() const
{
	return *m_ptr;
}

template <class T> inline tpRefPtr<T>::~tpRefPtr()
{
	if (m_ptr) 
	{
	    m_ptr->popRef();
	}
	m_ptr = 0L;

}

#endif

