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

#ifndef TP_PAIR_H
#define TP_PAIR_H


/**
 * @brief class to store pairs of values
 */
template <typename K, typename V>
class tpPair {
public:

	typedef K key_type;
	typedef V value_type;

	tpPair() {}

	tpPair(const K& key, const V& value) : mKey(key), mValue(value) {}

	tpPair(const tpPair<K,V>& p) : mKey(p.mKey), mValue(p.mValue) {}

	tpPair& operator = (const tpPair<K,V>& in) { mKey = in.mKey; mValue = in.mValue; return *this; }

	const K& getKey() const { return mKey; }
	K& getKey() { return mKey; }

	const V& getValue() const { return mValue; }
	V& getValue() { return mValue; }

	void setValue(const V& v) { mValue=v;}


protected:

	K mKey;
	V mValue;
};


// operators
template <typename K, typename V>
bool operator == (const tpPair<K,V>& x,const tpPair<K,V>& y)
{
    return ((x.getKey() == y.getKey()) &&
            (x.getValue()== y.getValue()));
}


template <typename K, typename V>
bool operator < (const tpPair<K,V>& x,const tpPair<K,V>& y)
{
	return (x.mKey < y.mKey) ? true : (x.mKey > y.mKey) ? false : (x.mValue < y.mValue);
}


#endif
