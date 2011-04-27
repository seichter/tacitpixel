/* 

    libTAP - The Architectural Playground 
    a minimalistic collaborative virtual environment framework
    
    Copyright (c) 1999-2003 Hartmut Seichter 

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

#ifndef TPSTACK_H
#define TPSTACK_H 1

#include <tp/types.h>


/**
	\brief a simple stack container
	
	The tpStack container is a substitute to STL stack.
	It has a different API but 
	Performance issues are tackled.
*/
template <typename T> class tpStack {
public:
	//! standard constructor.
	tpStack();

	//! copy constructor
	tpStack(const tpStack& stack);

	//! d'tor
	~tpStack();

	/**
	\brief put value on the top of the stack
	\param val value to be copied on the top of the stack.
	
	Copies the value on top of the stack. It also increments
	the preoccupied memory space on request.
	*/
	void push(T val);

	/** 
	\brief removes value from the top of the stack
	\return returns the value of the top element

	Copies the value of the top element of the stack
	and removes it from the stack.
	*/
	void pop();

	/**
	\brief copies the value of the top of the stack
	\return value of the top element in the stack
	
	Copies the value of the top element of the stack. 
	Elements keep untouched.
	*/
	T getTop();

	const T& getTop() const;

	/**
	\brief assignment from other stack
	
	Copies the elements of another stack to this one.
	*/
	void operator = (const tpStack& stack);

	/** 
	\brief removes all elements from the stack
	
	Removes all elements from the stack and frees the 
	memory occupied by the stack.
	*/
	void empty();

	/** 
	\brief checks if the stack is empty
	\return TRUE if the stack is empty, otherwise FALSE
	
	Checks if this stack is empty.
	*/
	bool isEmpty() const;
	
	/** 
	\brief get the actual size of the stack
	\return number of elements in the stack
	
	Get the number of elements of the stack.
	*/
	tpUInt getSize() const;
	
	tpUInt getMaxSize() const;

protected:

	void grow(tpUInt size);

	T* m_data;
	tpUInt m_datasize;
	tpUInt m_size;

};


// ---------------------------------------------------


template <typename T> inline tpStack<T>::tpStack()
	: m_data(0),
	m_datasize(0),
	m_size(0)
{
};


template <typename T> inline tpStack<T>::tpStack(const tpStack<T>& stack)
	: m_data(0),
	m_datasize(0),
	m_size(0)
{
	*this = stack;
};

template <typename T> inline tpStack<T>::~tpStack()
{
	empty();
};


template <typename T> inline void tpStack<T>::pop()
{
	--m_size;
};

template <typename T> inline void tpStack<T>::push(T val)
{
	// should use a better growing strategy
	if (m_size <= m_datasize) grow(m_size + 2);
	
	m_data[m_size] = val;

	++m_size;
};


template <typename T> inline T tpStack<T>::getTop()
{
	return m_data[m_size - 1];
};


template <typename T> inline const T& tpStack<T>::getTop() const
{
	return m_data[m_size - 1];
};

template <typename T> inline void tpStack<T>::operator = (const tpStack& stack)
{

	grow(stack.m_size);

	T* src = stack.m_data;
	T* dest = m_data;

	for (tpULong i = 0; i < m_size; ++i)
	{
		*dest = *src;
		++dest;
		++src;
	};
	// m_size = stack.m_size;
};


template <typename T> inline void tpStack<T>::grow(tpUInt size)
{
	
	if ((size <= m_datasize) || (!size)) return;

	T* _temp = new T[size];

	T* src = m_data;
	T* dest = _temp;

	for (tpUInt i = 0; i < m_size;++i)
	{
		*dest = *src;
		++dest;
		++src;
	};

	if (m_data) delete [] m_data;

	m_datasize = size;	
	m_data = _temp;
	
};


template <typename T> inline bool tpStack<T>::isEmpty() const
{
	return 0 == m_size;
}


template <typename T> inline void tpStack<T>::empty()
{
	if (m_data) delete [] m_data;
	m_data = 0;
	m_size = 0;
	m_datasize = 0;
}


template <typename T> inline tpUInt tpStack<T>::getSize() const
{
	return m_size;
};

template <typename T> inline tpUInt tpStack<T>::getMaxSize() const
{
	return m_datasize;
};



#endif
