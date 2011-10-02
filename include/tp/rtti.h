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
	explicit tpRTTI(const tpChar* name,tpRTTI* parent) : m_hash(tpAdler32((tpUChar*)name,tpStrLen(name))), m_parent(parent), m_name(name) {}

	/*! Check if this class has been inherited from another class
		\param parent check if this is a child (inherited) class
	 */
	inline bool isOfType(const tpRTTI* query) const
	{
		const tpRTTI* iter = this;
		while (iter)
		{
			if (query == iter) return true;
			iter = iter->m_parent;
		}
		return false;
	}

	template <typename T>
	bool isOfType(const T* q) const { this->isOfType(q->getTypeInfo()); }

	template <typename T,typename Tin>
	T* self_cast(Tin* q) const
	{
		return (q->getType()->isOfType(T::getTypeInfo())) ? static_cast<T*>(q) : 0L;
	}

	/*! Check if it is exacty the same type.
		\param parent checks if aclass is of exact type
	 */
	bool operator == (tpRTTI* query) const
	{
		return (query->m_hash == this->m_hash);
	}

	const char* getName() const { return m_name; }

	const tpULong& getID() const { return m_hash; }

protected:
	tpULong  m_hash;
	tpRTTI*  m_parent;
	const char* m_name;
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
