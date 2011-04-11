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