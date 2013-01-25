/*
 * Copyright (C) 1999-2013 Hartmut Seichter
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

	typedef T element_type;
	typedef T* iterator;
	typedef const T* const_iterator;

	static const tpUInt element_size = sizeof(T);

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


	T* begin() { return mData; }
	const T* begin() const { return mData; }
	T* end() { return mData + mSize; }
	const T* end() const { return mData + mSize; }

	T& front() { return *begin(); }
	const T& front() const { return *begin(); }

	T& back() { return *(end() - 1); }
	const T& back() const { return *(end() - 1); }


protected:

	void grow(tpUInt size);

	T* mData;
	tpUInt mCapacity;
	tpUInt mSize;

};


// ---------------------------------------------------


template <typename T> inline tpStack<T>::tpStack()
	: mData(0),
	mCapacity(0),
	mSize(0)
{
}


template <typename T> inline tpStack<T>::tpStack(const tpStack<T>& stack)
	: mData(0),
	mCapacity(0),
	mSize(0)
{
	*this = stack;
}


template <typename T> inline tpStack<T>::~tpStack()
{
	empty();
}


template <typename T> inline void tpStack<T>::pop()
{
	--mSize;
}


template <typename T> inline void tpStack<T>::push(T val)
{
	// should use a better growing strategy
	if (mSize <= mCapacity) grow(mSize + 2);

	mData[mSize] = val;

	++mSize;
}


template <typename T> inline T tpStack<T>::getTop()
{
	return mData[mSize - 1];
}


template <typename T> inline const T& tpStack<T>::getTop() const
{
	return mData[mSize - 1];
}


template <typename T> inline void tpStack<T>::operator = (const tpStack& stack)
{
	grow(stack.mSize);

	T* src = stack.mData;
	T* dest = mData;

	for (tpULong i = 0; i < mSize; ++i)
	{
		*dest = *src;
		++dest;
		++src;
	}
}


template <typename T> inline void tpStack<T>::grow(tpUInt size)
{

	if ((size <= mCapacity) || (!size)) return;

	T* _temp = new T[size];

	T* src = mData;
	T* dest = _temp;

	for (tpUInt i = 0; i < mSize;++i)
	{
		*dest = *src;
		++dest;
		++src;
	}

	if (mData) delete [] mData;

	mCapacity = size;
	mData = _temp;

}


template <typename T> inline bool tpStack<T>::isEmpty() const
{
	return 0 == mSize;
}


template <typename T> inline void tpStack<T>::empty()
{
	if (mData) delete [] mData;
	mData = 0;
	mSize = 0;
	mCapacity = 0;
}


template <typename T> inline tpUInt tpStack<T>::getSize() const
{
	return mSize;
}


template <typename T> inline tpUInt tpStack<T>::getMaxSize() const
{
	return mCapacity;
}



#endif
