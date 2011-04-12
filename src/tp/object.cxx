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


#include <tp/object.h>
#include <tp/log.h>

tpObject::tpObject(const tpString& name) : tpReferenced(), m_name(name)
{
	m_fields.add(new tpRefField<tpString>(m_name,"name"));
}

tpObject::tpObject(const tpObject& obj) : tpReferenced()
{
	m_fields.add(new tpRefField<tpString>(m_name,"name"));

	m_name = obj.m_name;
}

tpObject::~tpObject()
{
}

tpObject* 
tpObject::clone(const tpUByte& clonemode) const
{
	tpObject* ret = new tpObject(m_name + "_copy");
	return ret;
}

bool 
tpObject::isOfType(const tpRTTI* type) const
{
	return getType()->isOfType(type);
}

bool 
tpObject::isExactType(const tpRTTI* type) const
{
	return getType() == type;
}

TP_TYPE_REGISTER(tpObject,tpReferenced,Object);
