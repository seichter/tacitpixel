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

#ifndef TPSTRING_H
#define TPSTRING_H


#include "tp/types.h"
#include "tp/array.h"
#include "tp/stringutils.h"


typedef tpArray<tpChar> tpCharArray;

const tpInt TP_NOTFOUND = -1;


//tpVoid TP_API tpLocale2WChar( const char* str, tpArray<tpWChar>& dst );
//tpVoid TP_API tpWChar2Locale( const tpWChar* src, tpArray<tpChar>& dst );

class tpStringIterator;
class tpStringJoin;

/**
 * \class tpString
 * \module tpCore
 * \brief a string wrapper 
 *
 * A simple wrapper to handle a string. Not yet optimized.
 */
class TP_API tpString {
public:
	
	//! default c'tor
	tpString();
	
	//! copy constructor from plain c-string
	tpString(const char* str);
	
	//! copy constructor from wide string
	tpString(const wchar_t* str);
	
	//! copy constructor from buffer without terminating '\0'
	tpString(const char* str,unsigned int size);
	
	//! copy constructor from tpString
	tpString(const tpString& str);
	
	//! default d'tor
	~tpString();
	
	//! clean contents of the string
    void empty();   
	
	//! get the plain c-string
	const char* c_str() const;


#if defined(_UNICODE)
	
	const wchar_t* wc_str() const;
	
	const wchar_t* mb_str() const { return wc_str(); }
#else
	const char* mb_str() const { return c_str(); }
#endif
	
	//! set contents from c-string
    tpString& set(const char* str);
	
	//! set contents from c-string
	tpString& set(const wchar_t* str);
	
	//! set content from a non-null terminated C string
	tpString& set(const tpChar* buffer, tpUInt size);
	
	tpInt getPascal(char** buffer) const;
	
	tpString& operator += (const tpString& rs);
	tpString& operator += (const char* rs);
	
	//! operator = to copy from right side value
	tpString& operator = (const char* rs);
	//! operator = to copy from right side value
	tpString& operator = (const tpString& rs);
	//! operator = to copy from right side value
	tpString& operator = (const tpWChar* rs);
	
	
	//! operator + to add a string
	tpString& operator + (const tpString& rs);
	
	//! operator + to add a string
	tpString& operator + (const char* rs);
	
	//! check if the string is empty or not initialized
	tpBool isEmpty() const;
	
	//! append another string
	tpString& append(const tpString&);
	
	//! append another cstr
	tpString& append(const char* rs);
	
	
	//! return string size (not buffer size!)
	tpSizeT getLength() const;
	
	//! returns a hash-value for the string
	unsigned long getHash() const;
	
	
	int find(char c, tpBool fromright = false) const;
	
	int find(const char* sub) const;
	
	
	//! 
	tpString sub(int pos, int length) const;
	
	tpBool contains(const tpString& str) const;
	
	//! get the string after last occurence 
	tpString afterLast(const char& c) const;
	
	//! get the string before last occurrence
	tpString beforeLast(const char& c) const;
	
	
	//! get the string after first occurence 
	tpString afterFirst(const char& c) const;
	
	//! get the string before first occurrence
	tpString beforeFirst(const char& c) const;
	
	//! get what is between tagging characters
	tpString between(char left,char right) const;
	
	//! get as integer value
	long toLong() const;
	
	//! get as integer value
	int toInteger() const;
	
	//! get floating point value
	float toFloat() const;
	
	/*! Formats a string 
	 \param format formating string
	 \param ... parameters for insertion
	 */
	void format(const char* format, ...);
	
	
	/*! Split a string from separators
	 \param separator Separator to search in the string
	 \return array of strings excluding the separators
	 */
	tpArray<tpString> split(const tpString& separator) const;
	
	
	tpChar* operator [] ( tpInt offset ) { return m_stringbuffer.getData() + offset; }
	const tpChar* operator [] ( tpInt offset ) const { return m_stringbuffer.getData() + offset; }
	
	tpStringIterator *getBegin(); 
	
	tpString& operator << (const tpString& str);
	tpString& operator << (int i);
	tpString& operator << (long i);
	tpString& operator << (float r);
	tpString& operator << (double r);
	tpString& operator << (const char c);
	
	tpString& subst( tpChar c, const tpChar& substc );
	
	tpString& removeAfter(const tpChar& end);
	
	//tpBool operator == (const tpString& rs);
	
	
	tpVoid __verbose_dump() const;
	
protected:
	
	
	void syncCStr2WStr();
	void syncWStr2CStr();
	
	tpArray<tpChar> m_stringbuffer;
	
};


//////////////////////////////////////////////////////////////////////////

template <typename T> inline
tpString tpArrayToString(const tpArray<T>& arr, const tpString& separator = ", ")
{
	tpString res;
	for ( tpSizeT i = 0; i < arr.getSize(); i++ ) { res << arr[i]; if (i < arr.getSize() - 1) res << separator; }
	return res;
}


// -------------------------------------------------------


class TP_CORE_API tpStringTokenizer
{
public:
	
	tpStringTokenizer(const tpString& str, 
					  const tpString& delim);
	
	
	tpString nextToken();
	
	tpBool hasTokens() const;
	
	
	
protected:
	tpString m_string;
	tpString m_delim;
};


//////////////////////////////////////////////////////////////////////////

//const char* tpStrStr(const char* hay, char* needle);





// -----------------------------------------------------------------

inline tpBool operator == (const tpString& l, const tpString& r)
{
	return (0 == tpStrCmp(l.c_str(),r.c_str()));
}


inline tpBool operator != (const tpString& l, const tpString& r) 
{
	return (0 != tpStrCmp(r.c_str(),l.c_str()));
}

inline tpString& tpString::operator + (const tpString& rs)
{
	return this->append(rs);
};


inline tpString& tpString::operator + (const char* rs)
{
	return this->append(tpString(rs));
};


inline tpString& tpString::operator += (const tpString& rs)
{
	append(rs);
	return *this;
};


inline tpString& tpString::operator += (const char* rs)
{
	append(rs);
	return *this;
};


//////////////////////////////////////////////////////////////////////////

inline tpString operator + (const tpString& ls,const tpString& rs) {
	tpString res = ls;
	res.append(rs);
	return res;
}

template <typename T> 
class tpStringTokenizerNX
{
	tpStringBase<T> local;
	tpStringBase<T> delim;
	
public:
	
	tpStringTokenizerNX(const tpStringBase<T>& str, const tpStringBase<T>& dlm) : local(str.mb_str()), delim(dlm.mb_str())
	{
	}
	
	tpStringBase<T> next() 
	{
		tpStringBase<T> res;
		tpInt pos = local.find(*delim.mb_str(),false);
		
		if (pos > -1)
		{
			res = local.substr(0,pos);
			local = local.substr( pos + delim.getLength(), local.getLength() - pos - delim.getLength() ); 
		} else {
			res = local;
			local.empty();
		}
		
		return res;
	}
	
	tpBool finished() const 
	{
		return (0 == local.getLength());
	}
};

//#include <tpStringIO.h>

#endif
