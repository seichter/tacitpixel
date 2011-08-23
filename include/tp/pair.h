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

#ifndef TP_PAIR_H
#define TP_PAIR_H

/*!
	\class tpPair
	\brief pairs for storage in a tpMap
	
	That class stores pairs of data 
*/

template <typename K, typename V> class tpPair {
public:
	
	typedef K key_type;
	typedef V value_type;
	
	tpPair() {}

	tpPair(const K& key, const V& value) : m_key(key), m_value(value) {}
	
	tpPair(const tpPair<K,V>& p) : m_key(p.m_key), m_value(p.m_value) {}

	const K& getKey() const { return m_key; }
	K& getKey() { return m_key; }
	
	const V& getValue() const { return m_value; }
	V& getValue() { return m_value; }
	
protected:
	
	K m_key;
	V m_value;
};


// operators
template <typename K, typename V>
bool operator == (const tpPair<K,V>& x,const tpPair<K,V>& y) 
{ 
	return ((x.m_key == y.m_key) && 
			(x.m_value == y.m_value)); 
}

template <typename K, typename V>
bool operator < (const tpPair<K,V>& x,const tpPair<K,V>& y) 
{ 
	return (x.m_key < y.m_key) ? true : (x.m_key > y.m_key) ? false : (x.m_value < y.m_value); 
}


#endif