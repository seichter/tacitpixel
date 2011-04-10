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

#ifndef TPMAP_H
#define TPMAP_H


#include "tpArray.h"

/*!
	\class tpPair
	\brief pairs for storage in a tpMap
	
	That class stores pairs of data 
*/

template <class K, class V> class tpPair
{
public:

	tpPair(const K& key,V value);
	tpPair(const tpPair<K,V>&);
	~tpPair();

	const K& getKey();
	V getValue();
	
protected:
	
	K m_key;
	V m_value;
};


/*!
	\class tpMap
	\brief simple container with key=value mapping
*/

template <class K, class V> class tpMap
{
public:
	tpMap();
	~tpMap();
	
	tpPair<K,V>* add(const K& key, V value);

	V operator [] (const K& key) const;

	tpPair<K,V>* find(const K& key) const;


	//V find(const K& key) const;


	tpPair<K,V>* getIndex(tpUInt idx) const;	

	tpUInt getSize() const;

	tpMap& operator = (const tpMap&);

	void empty();


	typedef tpPair<K,V> PairType;

protected:

	tpArray<tpPair<K,V>*> m_pairs;

};


// -------------------------------------------------------------------


template <class K, class V> inline tpPair<K,V>::tpPair(const K& key, V value)
{
	m_key = key;
	m_value = value;
};

template <class K, class V> inline tpPair<K,V>::tpPair(const tpPair<K,V>& pair)
{
	m_key = pair.m_key;
	m_value = pair.m_value;
};

template <class K, class V> inline tpPair<K,V>::~tpPair()
{
};


template <class K, class V> inline const K& tpPair<K,V>::getKey()
{
	return m_key;
};

template <class K, class V> inline V tpPair<K,V>::getValue()
{
	return m_value;
};


// ----------------------------------------------------------------


template <class K, class V> inline tpMap<K,V>::tpMap()
{
};

template <class K, class V> inline tpMap<K,V>::~tpMap()
{
};

template <class K, class V> inline tpPair<K,V>* tpMap<K,V>::add(const K& key, V value)
{
	tpPair<K,V> *_entry = new tpPair<K,V>(key,value);
	m_pairs.add(_entry);
	return _entry;
};


//template <class K, class V> inline V tpMap<K,V>::find(const K& key) const
//{
//
//	register tpULong _size = m_pairs.getSize();
//	register tpULong _id = 0;
//
//	for (_id = 0;_id < _size;++_id)
//	{
//		if (m_pairs[_id]->getKey() == key) return m_pairs[_id]->getValue();
//	}
//
//	return (V)0;
//};

template <class K, class V> inline 
tpPair<K,V>* tpMap<K,V>::find(const K& key) const
{

	register tpULong _size = m_pairs.getSize();
	register tpULong _id = 0;

	for (_id = 0;_id < _size;++_id)
	{
		if (m_pairs[_id]->getKey() == key) return m_pairs[_id];
	}

	return 0;
};



template <class K, class V> inline V tpMap<K,V>::operator [] (const K& key) const
{
	return find(key)->getValue();
};




template <class K, class V> inline tpMap<K,V>& tpMap<K,V>::operator=(const tpMap& map)
{
	m_pairs = map.m_pairs;
	return *this;
};



template <class K, class V> inline void tpMap<K,V>::empty()
{
	m_pairs.empty();
};

template <class K, class V> inline tpPair<K,V>* tpMap<K,V>::getIndex(tpUInt idx) const
{
	return m_pairs[idx];
};

template <class K, class V> inline tpUInt tpMap<K,V>::getSize() const
{
	return m_pairs.getSize();
};








#endif
