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
	
	void add(const K& k, const V& v) { tpArray< tpPair<K,V> >::add(tpPair<K,V>(k,v)); }
	
};







#endif
