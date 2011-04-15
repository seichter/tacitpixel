#ifndef TPARRAY_H
#define TPARRAY_H

#include <tp/types.h>
#include <tp/utils.h>

/*!
 \class tpArray
 \brief a dynamic array 
 
 This replaces the STL container vector
 to achieve more independence from libs.
 Performance is similar to STL as you can
 see in the tapTest example.
 */

template <typename T> class tpArray {
public:
	
	typedef T element_type;
	typedef T* iterator;
	typedef const T* const_iterator;
	
	static const tpUInt element_size = sizeof(T);
	static const tpUInt max_capacity = sizeof(tpUInt);
	
	
	/** \brief c'tor (standard size is 30)
	 
	 Constructor of the array.
	 \param size initial size of the array (default = 30)
	 */
	tpArray();
	
	/** \brief copy c'tor
	 
	 Copies an existing array to this array.
	 \param array input array
	 */
	tpArray(const tpArray&);
	
	/** d'tor
	 */
	~tpArray();
	
	/** \brief get the data pointer
	 
	 Access the raw data (readonly).
	 \return T data pointer
	 */
	const T* getData() const;
	
	/** \brief get the data pointer
	 
	 Access the raw data
	 \return T data pointer
	 */
	T* getData();
	
	/** \brief get the size of the array
	 
	 Returns the size of the array.
	 \return size
	 */
	const tpSizeT& getSize() const;
	
	/** \brief grow the array
	 Grow the array by double the existing size.
	 */
	void grow();
	//! shrink to given size
	// void shrink(tpULong size);
	
	/** \brief add an item
	 
	 Add an existing item.
	 \param item the item to add
	 \return reference of array for chaining
	 */
	tpArray<T>& add(const T& item);
	
	/** \brief removes an item
	 
	 Removes an item from the array.
	 \param index is the pointer to the object to remove
	 
	 */
	T* erase(T* iter);
	
	
	//! erase the item at pos
	T* erase(tpSizeT pos);
	
	/** \brief removes one item from the end
	 
	 Removes an item from the array.
	 \param index is the index in the array
	 */
	void removeEnd();
	
	///** \brief removes an item
	
	//Removes an item from the array.
	//\param value
	//*/
	//void remove(const T& value);
	
	/** \brief find an item
	 
	 Find an item by value
	 \param value is the value to find in the array
	 \return index or -1 if fail or not found
	 */
	tpSizeT find(const T& value) const;
	
	/** \brief copy from array
	 
	 Copy another array to this array.
	 \param array array to copy
	 \return reference to itself
	 */
	tpArray& operator = (const tpArray&);
	
	/** \brief get item on position
	 
	 Returns single item on given position.
	 \param index index where item is located in the array.
	 \return reference to item
	 */
	
	T& operator [] (tpSizeT index);
	
	/** \brief get item value on position
	 
	 Returns the single item value on given position.
	 \param index index where item is located.
	 \return value of item
	 */
	const T& operator[] (tpSizeT index) const;
	
	/** \brief add an item
	 
	 Add item to the array.
	 \param item item to add
	 \return reference to added item
	 */
	T& operator+=(const T& item);
	
	/** \brief merge two arrays
	 
	 Merge two arrays.
	 \param merge array to merge with this
	 \return value of merged array
	 */
	tpArray<T> operator+(const tpArray& merge);
	
	/** \brief compare array for equality
	 
	 Compare two tpArray's for equality.
	 \param rs array as right-side parameter
	 \return TRUE if equal, otherwise false
	 */
	bool operator==(const tpArray& rs) const;
	
	/** \brief compare for difference
	 
	 Compare two arrays for difference.
	 \param rs array to compare to
	 \return TRUE if different, otherwise false
	 */
	bool operator!=(const tpArray& rs) const;
	
	/** \brief check if the array is empty
	 
	 Returns true when no items are in the array
	 \return TRUE if empty, otherwise false
	 */
	bool isEmpty() const;
	
	/** \brief empties the array
	 
	 Empties the array. No check is done here!
	 */
	void empty();
	
	/** \brief get the maximum capacity
	 
	 Get the current capacity of the array. This value
	 is dynamic.
	 */
	tpSizeT getCapacity() const;
	
	/** \brief resizes the array to optimal size
	 
	 Optimize for memory usage. Only use it with
	 large arrays! It can be counterproductive.
	 */
	void optimize();
	
	
	/**
	 * Overloaded operator to add items to the array
	 *
	 * \param value value
	 * \return reference to array for chaining
	 */
	tpArray& operator << (const T& value);
	
	//tpArray<T>& tpArray<T>::operator << (const tpArray<T>& other);
	
	/**
	 * Reserves memory
	 * 
	 * \param newsize 
	 */
	void reserve(tpSizeT newsize);
	
	/**
	 * Resize the array fit to the new size.
	 * 
	 * \param newsize  
	 */
	void resize(tpSizeT newsize, T v = T());
	
	/**
	 *	Set all items
	 *
	 * \param data 
	 */
	void assign(tpSizeT n, const T& data);
	
	void assign(T* first, T* last);

	/**
	 *	
	 */
	void preallocate(tpSizeT newsize, const T& data);
	
	tpArray& clone(tpArray<T>& l);
	
	void clear();
	
	void copy( const T* orig, tpUInt size, tpUInt pos = 0 );
	
	tpArray<T>& append( const tpArray<T>& rs );
	
	tpArray<T>& prepend( const tpArray<T>& rs );
	
	tpArray<T>& reverse();
	
	
	tpArray<T>& insert( const tpArray<T>& ins, tpSizeT pos );
	
	tpArray<T>& insert( const T* ins, tpSizeT pos, tpUInt length );
	
	
	T* begin() { return m_data; }
	const T* begin() const { return m_data; }
	T* end() { return m_data + m_size; }
	const T* end() const { return m_data + m_size; }
	
	T& front() { return *begin(); }
	const T& front() const { return *begin(); }
	
	T& back() { return *(end() - 1); }
	const T& back() const { return *(end() - 1); }

private:
	
	//! data pointer
	T* m_data;
	
	//! current size
	tpSizeT m_size;
	
	//! current allocated size
	tpSizeT m_maxsize;
};

template <typename T>
void tpArray<T>::copy( const T* orig, tpUInt length, tpUInt pos )
{
	resize(length);
	for (tpSizeT i = 0; i < length; i++ ) { m_data[i] = orig[i + pos]; }
	m_size = length;
}


template <typename T>
void tpArray<T>::clear()
{
	delete [] m_data;
	m_size = m_maxsize = 0;
}

template <typename T>
void tpArray<T>::removeEnd()
{
	if (m_size) m_size--;
}

template <typename T>
tpArray<T>& tpArray<T>::clone( tpArray<T>& l )
{
	if (l.getSize() != getSize())
	{
		resize(l.getSize());
		
		T* src = l.m_data;
		T* dest = m_data;
		
		for (tpSizeT i = 0; i < l.m_size;++i)
		{
			*dest = *src;
			++dest;
			++src;
		};
	}
	
	return *this;
}


//-----------------------------------------------------------------------------


template <typename T> tpArray<T>::tpArray() : m_data(0), m_size(0), m_maxsize(0)
{
}

template <typename T> tpArray<T>::tpArray(const tpArray& origin)
: m_data(0), m_size(0), m_maxsize(0)
{
	*this = origin;
}

template <typename T> T& tpArray<T>::operator [] (tpSizeT index)
{
	return m_data[index];
}

template <typename T> const T& tpArray<T>::operator [] (tpSizeT index) const
{
	return m_data[index];
}


template <typename T> tpArray<T>& tpArray<T>::operator = (const tpArray& rhs)
{
	if (this != &rhs)
	{
		reserve(rhs.getSize());
		const T* rhs_iter = rhs.begin();
		T* this_iter = this->begin();
		while( this_iter != this->end() && 
		  	rhs_iter != rhs.end() )
		{
			*this_iter = *rhs_iter;
			this_iter++; rhs_iter++;
		}
	}
	return *this;
}

template <typename T> tpArray<T>& tpArray<T>::add(const T& item)
{
	tpSizeT end_pos = m_size;
	if (getSize() == getCapacity()) grow();
	m_data[end_pos] = item;
	++m_size;
	return *this;
}


template <typename T> T* tpArray<T>::erase(T* iter)
{
	for (T* i = iter; i != end() - 1; i++)
	{
		*i = *(i + 1);
	}
	--m_size;
	return iter;
}

template <typename T> T* tpArray<T>::erase(tpSizeT pos)
{
	T* iter = begin() + pos;
	erase(iter);
	return iter;
}


template <typename T> tpSizeT tpArray<T>::find(const T& value) const
{
    for (tpSizeT i = 0; i < m_size; ++i) if (value == m_data[i]) return i;
    return static_cast<tpSizeT>(-1);	
}

template <typename T> tpArray<T> tpArray<T>::operator+(const tpArray& merge)
{
	tpArray<T> _new(m_size + merge.m_size);
	
	tpSizeT i = 0;
	
	for (i = 0; i < m_size; i++) _new.m_data[i] = m_data[i];
	for (tpULong j = 0; j < merge.m_size; j++) _new.m_data[m_size+i] = merge.m_data[i];
	
	return _new;
};

template <typename T> T& tpArray<T>::operator+=(const T& item)
{
	add(item);
	return m_data[m_size - 1];
}

template <typename T> tpArray<T>::~tpArray()
{
	clear();
}


template <typename T> const T* tpArray<T>::getData() const
{
	return m_data;
}

template <typename T> T* tpArray<T>::getData()
{
	return m_data;
}

template <typename T> const tpSizeT& tpArray<T>::getSize() const
{
	return m_size;
}


template <typename T> void tpArray<T>::reserve(tpSizeT n)
{
	if ( n > getCapacity() ) {
		T* tmp = new T[n];
		for(iterator i = begin(); i != end();)  *tmp++ = *i++;
		if (m_data) delete [] m_data;
		m_data = tmp;
		m_maxsize = n;
	}
}

template <typename T> void tpArray<T>::grow()
{
	reserve( getSize() ? getSize() * 2 : 8 );	
}

template <typename T> 
void tpArray<T>::resize(tpSizeT newsize, T v /*= T()*/)
{
	reserve(newsize);
	//\todo implement fill (should only set the overlapping part)
	m_size = newsize;
}


template <typename T> 
void tpArray<T>::assign(tpSizeT n, const T& data)
{
	resize(n);
	iterator iter = begin();
	for (tpSizeT i = 0; i < n; ++i) { *iter = data; ++iter; }
}

template <typename T> 
void tpArray<T>::assign(T* first, T* last)
{
	T* ptr = first;
	while (ptr != last) {this->m_data.add(*ptr); ptr++; }
}


template <typename T>
bool tpArray<T>::operator==(const tpArray& rs) const
{
	if (m_size != rs.m_size) return false;
	for (tpSizeT i = 0; i < m_size;i++) if (rs.m_data[i] != m_data[i]) return false;
	return true;
}

template <typename T> bool tpArray<T>::operator!=(const tpArray& rs) const
{
	return !operator==(rs);
}


template <typename T> bool tpArray<T>::isEmpty() const
{
	return (m_size == 0);
}

template <typename T> void tpArray<T>::empty()
{
	m_size = 0;
}

template <typename T> tpSizeT tpArray<T>::getCapacity() const
{
	return m_maxsize;
};

template <typename T> void tpArray<T>::optimize()
{
	reserve(m_size);
}

template <typename T> tpArray<T>& tpArray<T>::operator << (const T& value) 
{
	add(value);
	return *this;
}

template <typename T>
void tpArray<T>::preallocate( tpSizeT newsize, const T& data )
{
	assign(newsize,data);
}

template <typename T>
tpArray<T>& tpArray<T>::append( const tpArray<T>& rs )
{
	tpInt rsSize = rs.getSize(); 	
	for (tpInt i = 0; i < rsSize; i++)
	{
		add(rs.m_data[i]);
	}
	
	return *this;
}

template <typename T>
tpArray<T>& tpArray<T>::prepend( const tpArray<T>& rs )
{
	tpArray<T> tmp = rs;
	tpInt size = getSize(); 	
	for (tpInt i = 0; i < size; i++)
	{
		tmp.add(m_data[i]);
	}
	*this = tmp;
	
	return *this;
}

template <typename T>
tpArray<T>& tpArray<T>::insert( const tpArray<T>& ins, tpSizeT pos )
{
	if ( 0 == pos )
	{
		prepend(ins);
		
	} else
		if ( pos < getSize() )
		{
			tpArray<T> front;
			front.copy(getData(), pos);
			tpArray<T> back;
			back.copy(getData() + pos, getSize() - pos);
			
			front.append(ins);
			front.append(back);
			
			*this = front;
		} 
	
	return *this;
}

template <typename T>
tpArray<T>& tpArray<T>::insert( const T* ins, tpSizeT pos, tpUInt length )
{
	tpArray<T> tmp;
	tmp.copy(ins,length,0);
	
	return insert(tmp,pos);
}

template <typename T>
tpArray<T>& tpArray<T>::reverse()
{
	tpArray<T> tmp = *this;
	
	T* v_ptr = getData();
	for (tpSizeT i = getSize();i;i--)
	{
		*v_ptr++ = tmp[getSize()-i];
	}
	return *this;
}


#endif
