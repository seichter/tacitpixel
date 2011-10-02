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

#ifndef TP_REFPTR_H
#define TP_REFPTR_H

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

	inline bool operator == (const tpRefPtr& pr) const;
	inline bool operator == (const T* pr) const;
	
	

protected:
	T* m_ptr;

};

template <class T>
bool tpRefPtr<T>::operator==( const tpRefPtr& pr ) const
{
	return (m_ptr == pr.m_ptr);
}

template <class T>
bool tpRefPtr<T>::operator==( const T* pr ) const
{
	return (m_ptr == pr);
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

