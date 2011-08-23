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

#ifndef TPFIELD_H
#define TPFIELD_H


#include <tp/globals.h>
#include <tp/types.h>
#include <tp/string.h>
#include <tp/array.h>
#include <tp/referenced.h>
#include <tp/refptr.h>


enum tpFieldState
{
    TP_FIELD_UNCHANGED = 0,
    TP_FIELD_CHANGED
};



class tpFieldConnector;


/*!
	\class tpField
	\brief is a generic property of scenegraph nodes

	
*/

class tpField : public tpReferenced {
public:

	//! standard c'tor
	tpField(const tpString& name = "unnamed") : tpReferenced(), m_name(name) {}

	//! get a name
	const tpString& getName() const { return m_name; }

protected:
	
	//! d'tor
	virtual ~tpField() {}
	
	tpString	m_name;
};


typedef tpArray< tpRefPtr<tpField> > tpFields;


/*!
	\brief A single property of a node
*/


template <class T> class tpRefField : public tpField 
{
public:

	//! c'tor
	tpRefField(T& val,const tpString& name) : tpField(name), m_value(val) {}

	//! copy c'tor
	tpRefField(const tpRefField& field) { *this = field; }

	//! assignment operator
	tpRefField<T>& operator = (const tpRefField<T>& rhs) { m_name = rhs.getName(); m_value = rhs.getValue(); }


	//! return stored value
	const T& getValue() const;
	
	//! set value
	void setValue(const T& val);

protected:
	
	T& m_value;
};



template <class T> class tpRefPtrField : public tpField 
{
	//! c'tor
	tpRefPtrField() {};

	//! copy c'tor
	tpRefPtrField(const tpRefPtrField& field) {}

public:

	//! initial c'tor
	tpRefPtrField(T* val,const tpString& name = "noname") : tpField(name),  m_value(val) {}

	//! return stored value
	const T& getValue() const;

	//! set value
	void setValue(const T& val);

	//! assign values of other fields an dirty this one
	tpRefPtrField& operator = (const tpRefPtrField<T>&);

protected:

	tpRefPtr<T> m_value;
};

#if 0

/*!
	\brief an array of properties
*/
template <class T> class tpMultiField : public tpField
{
public:
	//! standard c'tor
	tpMultiField();
	
	//! copy c'tor
	tpMultiField(const tpMultiField& field);

	//! return of the values 
	const tpArray<T>& getValue() const;

	//! set the whole value set
	void setValue(const tpArray<T>& val);

	//! add a single value
	void addValue(const T& val);

	//! remove a value 
	void removeValue(const T& val);

	//! clean the whole value set
	void empty();

	//! return the sieze of the value set
	tpULong getSize() const;

	//! get a certain value at a position
	const T& operator[] (tpSizeT index);


protected:
	tpArray<T> m_value;
};





// -----------------------------------------------------------------------------------------------

template <class T> 
inline tpSingleField<T>::tpSingleField() : tpField()
{};

template <class T>
inline tpSingleField<T>::tpSingleField(const tpSingleField& field)
: tpField(), m_value(field.m_value)
{
};

template <class T>
inline tpSingleField<T>::tpSingleField(const T& val,
									   const tpString& name)
	: tpField(name), m_value(val)

{
};


template <class T> 
inline const T& tpSingleField<T>::getValue() const
{
	return m_value;
};

template <class T> inline void tpSingleField<T>::setValue(const T& val)
{
	m_value = val;
	evaluate();
};


template <class T> 
inline tpSingleField<T>& tpSingleField<T>::operator = (const tpSingleField<T>& v)
{
	m_value = v.m_value;
	evaluate();
	return *this;
};



// -----------------------------------------------------------------------------------------------

template <class T> inline tpMultiField<T>::tpMultiField() : tpField()
{};

template <class T> inline tpMultiField<T>::tpMultiField(const tpMultiField& field) 
: tpField(),
m_value(field.m_value)
{
};

template <class T> 
inline const tpArray<T>& tpMultiField<T>::getValue() const
{
	return m_value;
};


template <class T> 
inline void tpMultiField<T>::setValue(const tpArray<T>& val)
{
	m_value = val;
	evaluate();
};

template <class T> 
inline void tpMultiField<T>::addValue(const T& val)
{
	m_value += val;
	evaluate();
};

template <class T> 
inline void tpMultiField<T>::removeValue(const T& val)
{
	tpLong idx = m_value.find(val);
	if (idx > -1)
	{
		m_value.remove(idx);
		evaluate();
	};
};

template <class T> 
inline void tpMultiField<T>::empty()
{
	m_value.empty();
	evaluate();
};

template <class T> 
inline tpULong tpMultiField<T>::getSize() const
{
	return m_value.getSize();
};


template <class T> 
inline const T& tpMultiField<T>::operator[] (tpSizeT index)
{
	return m_value[index];
};




template <class T> 
inline tpBool operator==(const tpSingleField<T>& l, const tpSingleField<T>& r)
{
  return r.getValue() == l.getValue();
}

template <class T> 
inline tpBool operator!=(const tpSingleField<T>& l, const tpSingleField<T>& r)
{
  return !operator == (l,r);
}


template <class T> 
inline tpBool operator==(const tpMultiField<T>& l, const tpMultiField<T>& r)
{
  return r.getValue() == l.getValue();
}

template <class T> 
inline tpBool operator!=(const tpMultiField<T>& l, const tpMultiField<T>& r)
{
  return !operator == (l,r);
}




class TP_SG_API tpSFFloat : public tpSingleField<tpFloat>
{
public:		
	tpSFFloat();
};


class TP_SG_API tpSFDouble : public tpSingleField<tpDouble>
{
public:		
	tpSFDouble();
};

class TP_SG_API tpSFInt : public tpSingleField<tpInt>
{
public:		
	tpSFInt();
};

class TP_SG_API tpSFUInt : public tpSingleField<tpUInt>
{
public:		
	tpSFUInt();
};



class TP_SG_API tpSFBool : public tpSingleField<tpBool>
{
public:
	tpSFBool();
};

class TP_SG_API tpSFString : public tpSingleField<tpString>
{
public:
	tpSFString();
};


class TP_SG_API tpMFInt : public tpMultiField<tpInt>
{
public:
	tpMFInt();
};


// ----------------------------------------------------------------------------



class TP_SG_API tpSFVec3f : public tpSingleField<tpVec3f>
{
public:			
	tpSFVec3f();
};

class TP_SG_API tpSFVec3d : public tpSingleField<tpVec3d>
{
public:		
	tpSFVec3d();
};

class TP_SG_API tpSFRotation : public tpSingleField<tpVec4d>
{
public:			
	tpSFRotation();
};


class TP_SG_API tpMFVec3f : public tpMultiField<tpVec3f>
{
public:
	tpMFVec3f();
};


class TP_SG_API tpMFVec3d : public tpMultiField<tpVec3d>
{
public:
	tpMFVec3d();
};

typedef tpSingleField<tpVec4f> tpSFColor;

class TP_SG_API tpSFVec4f : public tpSingleField<tpVec4f>
{
public:			
	tpSFVec4f(){};
};

#endif

#endif



