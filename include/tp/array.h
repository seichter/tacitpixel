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
	
	
	/**
	 * Constructor
	 */
	tpArray();
	
	/**
	 * Copy Constructor
	 * 
	 * \param array (const tpArray &)
	 */
	tpArray(const tpArray& array);

	/**
	 * Destructor
	 */
	~tpArray();
	

	const T* getData() const { return this->m_data; }
	T* getData() { return this->m_data; }
	
	T& operator [] (tpSizeT index);
	const T& operator[] (tpSizeT index) const;

	/**
	 * Get element count of array
	 * 
	 * \return (const tpSizeT&) number of elements in 
	 * the array
	 */
	const tpSizeT& getSize() const;
	
	
	/**
	 * Add element to the array. Similar to push_back in STL
	 * 
	 * \param item (const T &) element to be added
	 * \return (tpArray<T>&) return reference to self
	 */
	tpArray<T>& add(const T& item);
	

	/**
	 * Erase an element from the array. Similar to erase in STL
	 * 
	 * \param iter (T *) pointer to element
	 * \return (T*) returns the element for destruction or other usage
	 */
	T* erase(T* iter);
	
	/**
	 * Erase an element at position from the array. Similar to erase in STL
	 * 
	 * \param pos (tpSizeT) position of the element
	 * \return (T*) return the element
	 */
	T* erase(tpSizeT pos);
	
	
	/**
	 * Remove the last element in the array
	 * 
	 * \return (T*) return the removed element
	 */
	T* removeEnd();
	
	/**
	 * Try to find a value in the array
	 * 
	 * \param value (const T &) value to be found
	 * \return (tpSizeT) position of the item
	 */
	tpSizeT find(const T& value) const;
	
	/**
	 * Copy to the array
	 * 
	 * \param & (const tpArray)
	 * \return (tpArray&)
	 */
	tpArray& operator = (const tpArray&);

	
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
	
	//tpArray& clone(tpArray<T>& l);
	
	void release();
	
	void copy( const T* orig, tpSizeT size, tpSizeT pos = 0 );
	
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


	void grow();

	
	//! data pointer
	T* m_data;
	
	//! current size
	tpSizeT m_size;
	
	//! current allocated size
	tpSizeT m_capacity;
};

template <typename T>
void tpArray<T>::copy( const T* orig, tpSizeT length, tpSizeT pos )
{
	resize(length);
	for (tpSizeT i = 0; i < length; i++ ) { m_data[i] = orig[i + pos]; }
	m_size = length;
}


template <typename T>
void tpArray<T>::release()
{
	//tpLogNotify("release 0x%x size:%d capacity:%d (0x%x) element size: %d '%s'",this,m_size,m_capacity,&m_data[0],sizeof(T),typeid(T).name());
	if (m_data) { delete [] m_data; m_data = 0; }
	//tpLogNotify("release 0x%x size:%d capacity:%d (0x%x)",this,m_size,m_capacity,&m_data[0]);
	m_size = m_capacity = 0;
}

template <typename T>
T* tpArray<T>::removeEnd()
{
	if (m_size) {
		m_size--;
		return m_data[m_size];
	}
	return 0;
}

//template <typename T>
//void tpArray<T>::copy( tpArray<T>& l ) const
//{
//	if (&l != this)
//	{
//		l.resize(getSize());
//		for ( tpSizeT i = 0; i < m_size; ++i ) l.m_data[i] = m_data[i];
//	}
//	
//	return *this;
//}


//-----------------------------------------------------------------------------


template <typename T>
tpArray<T>::tpArray() : m_data(0), m_size(0), m_capacity(0)
{
}

template <typename T>
tpArray<T>::tpArray(const tpArray& origin)
: m_data(0), m_size(0), m_capacity(0)
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

template <typename T> 
tpArray<T>& tpArray<T>::add(const T& item)
{
	tpSizeT end_pos = m_size;
	if (getSize() == getCapacity()) grow();
	m_data[end_pos] = item;
	++m_size;
	return *this;
}


template <typename T> 
T* tpArray<T>::erase(T* iter)
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
	this->release();
}

template <typename T> const tpSizeT& tpArray<T>::getSize() const
{
	return m_size;
}


template <typename T> 
void tpArray<T>::reserve(tpSizeT n)
{
	//tpLogNotify("reserve 0x%x (%d) data: 0x%x",this,n,m_data);
	if ( n > getCapacity() ) 
	{
		T* tmp = new T[n];
		for(tpSizeT i = 0; i < m_size;++i) tmp[i] = m_data[i];
		delete [] m_data; m_data = 0;
		m_data = tmp;
		m_capacity = n;
	}
	//tpLogNotify("reserve 0x%x (%d) data: 0x%x",this,n,m_data);
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
	return m_capacity;
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
