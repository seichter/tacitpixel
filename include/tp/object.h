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

#ifndef TPOBJECT_H
#define TPOBJECT_H

//#include "tpField.h"
//#include "tpFieldContainer.h"


/*! \enum tpCopyMode
	\brief controls the depth of a copy

*/
enum tpCopyMode
{
	TP_COPY_SHALLOW,	//!< a shallow copy
	TP_COPY_DEEP,		//!< a deep copy
	TP_COPY_ONE			//!< a shallow copy including a second level
};


/*
	\class tpObject
	\brief the basic object in the scenegraph hierarchy

	A tpObject neither rendered, traversed or otherwise be handles
	it is only capable of holding fields.
*/

#include <tp/globals.h>
#include <tp/rtti.h>
#include <tp/referenced.h>
#include <tp/string.h>
#include <tp/scopeptr.h>
#include <tp/field.h>


class TP_API tpObject : public tpReferenced {
public:

    TP_TYPE_DECLARE

	//! standard c'tor
	tpObject(const tpString& name = "");
	//! copy c'tor
	tpObject(const tpObject& obj);

	//! returns if the type is same or inherited from object
	bool isOfType(const tpRTTI*) const;

	//! returns if the type is same as object
	bool isExactType(const tpRTTI*) const;

	//! clone function
	virtual tpObject* clone(const tpUByte& copymode) const;

	//! set the name
	void setName(const tpString& name) { m_name = name; }

	//! return the name
	const tpString& getName() const { return m_name; }

	//! field system get a copy
	tpFields getFields() { return m_fields; }

	//! const ret field system
	const tpFields& getFields() const { return m_fields; }

protected:

	virtual ~tpObject();

	tpString m_name;
	tpFields m_fields;

};



#endif
