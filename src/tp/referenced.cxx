/*
 * Twisted Pair Visualization Engine
 *
 * Copyright (c) 1999-2009 Hartmut Seichter 
 * 
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the Twisted Pair License (TPL) version 1.0 or (at your option) 
 * any later version. The full license text is available in the LICENSE file 
 * included with this distribution, and on the technotecture.com website.
 *
 */

#include <tp/referenced.h>


tpReferenced::tpReferenced()
	: m_refcount(0)
{
}

tpReferenced::~tpReferenced() 
{
	if (m_refcount > 0)
	{
		//tpLogError("%s goes out of scope with %d references dangling",__FUNCTION__,m_refcount);
	}	
}


tpSizeT tpReferenced::getReferenceCount() const
{
	return m_refcount;
}

tpVoid
tpReferenced::popRef() const
{	
	if (0 == --m_refcount) {
	    delete this;
	}
}

tpVoid 
tpReferenced::pushRef() const
{
	++m_refcount;
}


TP_TYPE_REGISTER_BASE(tpReferenced,Reference);


