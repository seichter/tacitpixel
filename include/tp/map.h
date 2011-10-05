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

#ifndef TPMAP_H
#define TPMAP_H


#include <tp/globals.h>
#include <tp/array.h>
#include <tp/pair.h>


/*!
	\class tpMap
	\brief simple container with key=value mapping
*/

template <typename K, typename V>
class tpMap : public tpArray< tpPair<K,V> >
{
public:

	typedef tpPair<K,V> element_type;
	typedef tpPair<K,V>* iterator;
	typedef const tpPair<K,V>* const_iterator;

	void add(const K& k, const V& v) { tpArray< tpPair<K,V> >::add(tpPair<K,V>(k,v)); }

	bool containsKey(const K& k) const
	{
		for (const_iterator i = this->begin();
			 i != this->end();
			 ++i)
		{
			if (k == (*i).getKey()) return true;
		}

		return false;
	}

	bool containsValue(const V& v) const
	{
		for (const_iterator i = this->begin();
			 i != this->end();
			 ++i)
		{
			if (v == (*i).getValue()) return true;
		}

		return false;
	}

};







#endif
