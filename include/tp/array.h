/*
 * Copyright (C) 1999-2012 Hartmut Seichter
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
#ifndef _TP_ARRAY_H_
#define _TP_ARRAY_H_

#include <tp/types.h>
#include <tp/utils.h>

/**
  * @brief a dynamic array similar to an std::vector with reduced features
  */
template <typename T, typename Tidx = tpUInt>
class tpArray {
public:

	typedef T element_type;
	typedef T* iterator;
	typedef const T* const_iterator;

	static const tpUInt element_size = sizeof(T);
	static const tpUInt max_capacity = sizeof(Tidx);
	static const Tidx	kNotFound = -1;


	/** Constructor */
	tpArray();

	/**
	  * @brief copy constructor
	  * @param array input array
	  */
	tpArray(const tpArray& array);

	/** Destructor*/
	~tpArray();

	/** get pointer to the data array (const) */
	const T* getData() const { return this->mData; }

	/** get pointer to the data array */
	T* getData() { return this->mData; }

	/** get element in the data array */
	const T& operator[] (Tidx index) const;

	/** get element in the data array */
	T& operator [] (Tidx index);

	/**
	  * @brief get element count of array
	  * @return (const Tidx&) number of elements
	  */
	const Tidx& getSize() const;

	/**
	  * @brief add an element to the array. similar to push_back()
	  * @param item (const T &) element to be added
	  * @return (tpArray<T,Tidx>&) return reference to self
	  */
	tpArray<T,Tidx>& add(const T& item);

	/**
	 * @brief erase an element from the array.
	 * @param iter (T *) pointer to element
	 * @return (T*) returns the element for destruction or other usage
	 */
	T* erase(T* iter);

	/**
	  * @brief Erase an element at position from the array.
	  * @param pos (Tidx) position of the element
	  * @return (T*) return the element
	  */
	T* erase(Tidx pos);

	/**
	  * @brief remove the last element in the array
	  * @return (T*) return the removed element
	  */
	T* removeEnd();

	/**
	  * @brief find the index of a value
	  * @param value (const T &) value to be found
	  * @return (Tidx) position of the item
	  */
	Tidx find(const T& value) const;

	/**
	  * @brief assignment operator
	  * @param & (const tpArray) input array
	  * @return (tpArray&) reference to *this
	  */
	tpArray& operator = (const tpArray&);

	/**
	  * @brief merge two arrays
	  * @param merge array to merge with this
	  * @return value of merged array
	  */
	tpArray<T,Tidx> operator+(const tpArray& merge);

	/**
	  * @brief comparison operator
	  * @param rs array to compare to
	  * @return true if equal, otherwise false
	  */
	bool operator==(const tpArray& rs) const;

	/**
	  * @brief comparison operator
	  * @param rs array to compare to
	  * @return true if different, otherwise false
	  */
	bool operator!=(const tpArray& rs) const;

	/** return true if empty, otherwise false */
	bool isEmpty() const;

	/** empties the array (same as ::clear() */
	void clear();

	/** get maximum capaxity of the array */
	Tidx getCapacity() const;

	/**
	  * @brief resizes the array to optimal size
	  *
	  * Optimize for memory usage. Only use it with
	  * large arrays as it can be counterproductive.
	  */
	void optimize();

	/**
	  * @brief Overloaded operator to add items to the array
	  * @param value value
	  * @return reference to array for chaining
	  */
	tpArray& operator << (const T& value);

	/**
	  * @brief reserves memory for items
	  * @param newsize size in elements
	  */
	void reserve(Tidx newsize);

	/**
	  * @brief Resize the array fit to the new size.
	  * @param newsize size in elements
	  * @param v set all values to this value
	  */
	void resize(Tidx newsize, T v = T());

	/**
	  * @brief assign a value to items in the array
	  * @param data value that is being used to assign
	  */
	void assign(Tidx n, const T& data);

	/**
	  * @brief sets the array to the values described between
	  * the two values first and last
	  * @param first start of the array
	  * @param last item in the array
	  */
	void assign(const T* first, const T* last);

	void preallocate(Tidx newsize, const T& data);

	//tpArray& clone(tpArray<T,Tidx>& l);

	void release();

	void copy( const T* orig, Tidx size, Tidx pos = 0 );

	tpArray<T,Tidx>& append( const tpArray<T,Tidx>& rs );

	tpArray<T,Tidx>& prepend( const tpArray<T,Tidx>& rs );

	tpArray<T,Tidx>& reverse();


	tpArray<T,Tidx>& insert( const tpArray<T,Tidx>& ins, Tidx pos );

	tpArray<T,Tidx>& insert( const T* ins, Tidx pos, tpUInt length );


	T* begin() { return mData; }
	const T* begin() const { return mData; }
	T* end() { return mData + mSize; }
	const T* end() const { return mData + mSize; }

	T& front() { return *begin(); }
	const T& front() const { return *begin(); }

	T& back() { return *(end() - 1); }
	const T& back() const { return *(end() - 1); }

	/**
	  * @brief checks if an item is already in the array
	  * @param item item to search for
	  * @return true if the item exists already
	  */
	bool contains(const T &item) const;

	tpArray<T, Tidx>& swap(tpArray<T, Tidx> &other);


private:

	void grow();

	//! data pointer
	T* mData;

	//! current size
	Tidx mSize;

	//! current allocated size
	Tidx mCapacity;
};

template <typename T,typename Tidx>
void tpArray<T,Tidx>::copy( const T* orig, Tidx length, Tidx offset )
{
	resize(length);
	for (Tidx i = 0; i < length; i++ ) { mData[i] = orig[i + offset]; }
}


template <typename T,typename Tidx>
void tpArray<T,Tidx>::release()
{
	if (mData) { delete [] mData; mData = 0; }
	mSize = mCapacity = 0;
}

template <typename T,typename Tidx>
T* tpArray<T,Tidx>::removeEnd()
{
	if (mSize) {
		mSize--;
		return &mData[mSize];
	}
	return 0;
}

//-----------------------------------------------------------------------------


template <typename T,typename Tidx>
tpArray<T,Tidx>::tpArray()
	: mData(0)
	, mSize(0)
	, mCapacity(0)
{
}

template <typename T,typename Tidx>
tpArray<T,Tidx>::tpArray(const tpArray& origin)
	: mData(0)
	, mSize(0)
	, mCapacity(0)
{
	*this = origin;
}

template <typename T,typename Tidx> T&
tpArray<T,Tidx>::operator [] (Tidx index)
{
	return mData[index];
}

template <typename T,typename Tidx> const T& tpArray<T,Tidx>::operator [] (Tidx index) const
{
	return mData[index];
}


template <typename T,typename Tidx> tpArray<T,Tidx>&
tpArray<T,Tidx>::operator = (const tpArray& rhs)
{
	if (this != &rhs)
	{
		this->assign(rhs.begin(),rhs.end());
	}
	return *this;
}

template <typename T,typename Tidx>
tpArray<T,Tidx>& tpArray<T,Tidx>::add(const T& item)
{
	Tidx end_pos = mSize;
	if (getSize() == getCapacity()) grow();
	mData[end_pos] = item;
	++mSize;
	return *this;
}

template <typename T,typename Tidx>
bool tpArray<T,Tidx>::contains(const T& item) const
{
	for (const_iterator i = this->begin();
		 i != this->end();
		 ++i)
	{
		if (item == (*i)) return true;
	}
	return false;
}


template <typename T,typename Tidx>
T* tpArray<T,Tidx>::erase(T* iter)
{
	for (T* i = iter; i != end() - 1; i++)
	{
		*i = *(i + 1);
	}
	--mSize;
	return iter;
}

template <typename T,typename Tidx> T* tpArray<T,Tidx>::erase(Tidx pos)
{
	T* iter = begin() + pos;
	erase(iter);
	return iter;
}


template <typename T,typename Tidx> Tidx tpArray<T,Tidx>::find(const T& value) const
{
	for (Tidx i = 0; i < mSize; ++i) if (value == mData[i]) return i;
	return static_cast<Tidx>(-1);
}

template <typename T,typename Tidx> tpArray<T,Tidx> tpArray<T,Tidx>::operator+(const tpArray& merge)
{
	tpArray<T,Tidx> _new(mSize + merge.mSize);

	Tidx i = 0;

	for (i = 0; i < mSize; i++) _new.mData[i] = mData[i];
	for (tpULong j = 0; j < merge.mSize; j++) _new.mData[mSize+i] = merge.mData[i];

	return _new;
}

template <typename T,typename Tidx> tpArray<T,Tidx>::~tpArray()
{
	this->release();
}

template <typename T,typename Tidx>
const Tidx& tpArray<T,Tidx>::getSize() const
{
	return mSize;
}


template <typename T,typename Tidx>
void tpArray<T,Tidx>::reserve(Tidx n)
{
	//tpLogNotify("reserve 0x%x (%d) data: 0x%x",this,n,m_data);
	if ( n > getCapacity() )
	{
		T* tmp = new T[n];
		for(Tidx i = 0; i < mSize;++i) tmp[i] = mData[i];
		delete [] mData; mData = 0;
		mData = tmp;
		mCapacity = n;
	}
	//tpLogNotify("reserve 0x%x (%d) data: 0x%x",this,n,m_data);
}

template <typename T,typename Tidx> void tpArray<T,Tidx>::grow()
{
	reserve( getSize() ? getSize() * 2 : 8 );
}

template <typename T,typename Tidx>
void tpArray<T,Tidx>::resize(Tidx newsize, T v /*= T()*/)
{
	reserve(newsize);
	//\todo implement fill (should only set the overlapping part)
	mSize = newsize;
}


template <typename T,typename Tidx>
void tpArray<T,Tidx>::assign(Tidx n, const T& data)
{
	resize(n);
	iterator iter = begin();
	for (Tidx i = 0; i < n; ++i) { *iter = data; ++iter; }
}

template <typename T,typename Tidx>
void tpArray<T,Tidx>::assign(const T* first, const T* last)
{
	clear();
	for (const_iterator i = first;
		 i != last;
		 ++i)
	{
		this->add((*i));
	}
}


template <typename T,typename Tidx>
bool tpArray<T,Tidx>::operator==(const tpArray& rs) const
{
	if (mSize != rs.mSize) return false;
	for (Tidx i = 0; i < mSize;i++) if (rs.mData[i] != mData[i]) return false;
	return true;
}

template <typename T,typename Tidx> bool tpArray<T,Tidx>::operator!=(const tpArray& rs) const
{
	return !operator==(rs);
}


template <typename T,typename Tidx>
bool tpArray<T,Tidx>::isEmpty() const
{
	return (mSize == 0);
}

template <typename T,typename Tidx>
void tpArray<T,Tidx>::clear()
{
	// just set size to null
	mSize = 0;
}

template <typename T,typename Tidx> Tidx tpArray<T,Tidx>::getCapacity() const
{
	return mCapacity;
}

template <typename T,typename Tidx> void tpArray<T,Tidx>::optimize()
{
	reserve(mSize);
}

template <typename T,typename Tidx> tpArray<T,Tidx>& tpArray<T,Tidx>::operator << (const T& value)
{
	add(value);
	return *this;
}

template <typename T,typename Tidx>
void tpArray<T,Tidx>::preallocate( Tidx newsize, const T& data )
{
	assign(newsize,data);
}

template <typename T,typename Tidx>
tpArray<T,Tidx>& tpArray<T,Tidx>::append( const tpArray<T,Tidx>& rs )
{
	tpInt rsSize = rs.getSize();
	for (tpInt i = 0; i < rsSize; i++)
	{
		add(rs.mData[i]);
	}

	return *this;
}

template <typename T,typename Tidx>
tpArray<T,Tidx>& tpArray<T,Tidx>::prepend( const tpArray<T,Tidx>& rs )
{
	tpArray<T,Tidx> tmp = rs;
	tpInt size = getSize();
	for (tpInt i = 0; i < size; i++)
	{
		tmp.add(mData[i]);
	}
	*this = tmp;

	return *this;
}

template <typename T,typename Tidx>
tpArray<T,Tidx>& tpArray<T,Tidx>::insert( const tpArray<T,Tidx>& ins, Tidx pos )
{
	if ( 0 == pos )
	{
		prepend(ins);

	} else
		if ( pos < getSize() )
		{
			tpArray<T,Tidx> front;
			front.copy(getData(), pos);
			tpArray<T,Tidx> back;
			back.copy(getData() + pos, getSize() - pos);

			front.append(ins);
			front.append(back);

			*this = front;
		}

	return *this;
}

template <typename T,typename Tidx>
tpArray<T,Tidx>& tpArray<T,Tidx>::insert( const T* ins, Tidx pos, tpUInt length )
{
	tpArray<T,Tidx> tmp;
	tmp.copy(ins,length,0);

	return insert(tmp,pos);
}

template <typename T,typename Tidx>
tpArray<T,Tidx>& tpArray<T,Tidx>::reverse()
{
	tpArray<T,Tidx> tmp = *this;

	T* v_ptr = getData();
	for (Tidx i = getSize();i;i--)
	{
		*v_ptr++ = tmp[getSize()-i];
	}
	return *this;
}

template <typename T,typename Tidx>
tpArray<T,Tidx>& tpArray<T,Tidx>::swap(tpArray<T,Tidx>& other)
{
	tpSwap(mData,other.mData);
	tpSwap(mCapacity,other.mCapacity);
	tpSwap(mSize,other.mSize);

	return *this;
}

#endif
