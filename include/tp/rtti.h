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

#ifndef TP_RTTI_H
#define TP_RTTI_H 1

#include <tp/types.h>
#include <tp/stringutils.h>
#include <tp/hash.h>

/*!
	\class tpRTTI
	\brief runtime type information (RTTI)

	This class circumvents compilers with a weak or not working RTTI 
	implementation. If you derive a class for rendering 
	into the scenegraph you have to provide type information.
*/

class TP_API tpRTTI
{
public:
	/*! Default c'tor
		\param name name of the class
		\param parent parent class of this one, NULL if it is a base class
	 */
	explicit tpRTTI(const tpChar* name,tpRTTI* parent) : m_hash(tpAdler32((tpUChar*)name,tpStrLen(name))), m_parent(parent) {}
	
	/*! Check if this class has been inherited from another class
		\param parent check if this is a child (inherited) class
	 */
	inline bool isOfType(tpRTTI* query) const
	{
		const tpRTTI* iter = this;
		while (iter)
		{
			if (query == iter) return true;
			iter = iter->m_parent;
		}
		return false;
	}

	/*! Check if it is exacty the same type.
		\param parent checks if aclass is of exact type
	 */
	bool operator == (tpRTTI* query) const
	{
		return query->m_hash = this->m_hash;
	}

protected:

	tpRTTI*  m_parent;
	tpULong  m_hash;
};

#define TP_TYPE_DECLARE \
	static tpRTTI ms_type; \
	static tpRTTI *getTypeInfo() { return &ms_type;} \
	virtual tpRTTI *getType() const { return this->getTypeInfo(); } 



#define TP_TYPE_REGISTER(type,parent_type,class_name) \
	tpRTTI type::ms_type(#class_name,&parent_type::ms_type)

#define TP_TYPE_REGISTER_BASE(type,class_name) \
	tpRTTI type::ms_type(#class_name,0L)




#endif
