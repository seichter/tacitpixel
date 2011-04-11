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

	TP_TYPE_DECLARE;

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
	tpString m_serializerhint;
	
	tpFields m_fields;

};



#endif
