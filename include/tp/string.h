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


#include <tp/globals.h>
#include <tp/types.h>
#include <tp/chunk.h>
#include <tp/array.h>
#include <tp/stringutils.h>


const tpInt TP_NOTFOUND = -1;


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
	
	enum {
		ASCII = 0
	};

	//! default c'tor
	tpString();
	
	//! copy constructor from plain c-string
	tpString(const char* str, tpUShort encoding = ASCII);
	
	//! copy constructor from buffer without terminating '\0'
	tpString(const char* str, unsigned int size, tpUShort encoding = ASCII);
	
	//! copy constructor from tpString
	tpString(const tpString& str);
	
	//! default d'tor
	~tpString();
	
	//! clean contents of the string
    void empty();   
	
	//! get the plain c-string
	const char* c_str() const { return m_buffer.ptr<const char>(); }
	
	//! get the plain c-string
	char* c_str() { return m_buffer.ptr<char>(); }
	
	//! set contents from c-string
    tpString& set(const char* str, tpUByte encoding = ASCII);
	
	//! set content from a non-null terminated C string
	tpString& set(const char* buffer, tpSizeT size, tpUByte encoding = ASCII);
	
	tpInt getPascal(char** buffer) const;
	
	inline tpString& operator += (const tpString& rhs) { append(rhs); return *this; }
	inline tpString& operator += (const char* rhs) { append(rhs); return *this; }
	
	//! operator = to copy from right side value
	tpString& operator = (const char* rs) { set(rs); return *this; }
	//! operator = to copy from right side value
	tpString& operator = (const tpString& rs) { set(rs.c_str()); return *this; }
	
	/*
	//! operator = to copy from right side value
	tpString& operator = (const tpWChar* rs);
	*/
	
	//! operator + to add a string
	tpString& operator + (const tpString& rs) { append(rs); return *this; }
	
	//! operator + to add a string
	tpString& operator + (const char* rs) { append(rs); return *this; }
	
	//! check if the string is empty or not initialized
	bool isEmpty() const;
	
	//! append another string
	tpString& append(const tpString&);
	
	//! append another cstr
	tpString& append(const char* rs);

	//! prepend another cstr
	tpString& prepend(const char* rs);

	//! truncate the string at position pos
	tpString& truncate(tpSizeT pos);
	
	//! return string size (not buffer size!)
	tpSizeT getLength() const;

	//! find a single character
	int find(const char& c, bool fromright = false) const;

	//! sets this character to 0 in order to remove everything behind
	tpString& removeFrom(const char& c, bool fromright = false);
	
	//! for guessing encoding
	static bool isUTF8(const char* str);
	
	
	template <typename T>
	T& operator [] ( tpSizeT idx ) { return m_buffer.ptr<T>[idx]; }
	
	template <typename T>
	const T& operator [] ( tpSizeT idx ) const { return m_buffer.ptr<T>[idx]; }
	
	
	//! get the string after last occurence 
	tpString afterLast(const char& c) const;
	
	//! find a sub string
	int find(const char* sub) const;
	
	
	
#if 0
	
	//! returns a hash-value for the string
	unsigned long getHash() const;
	
	
	
	
	//! 
	tpString sub(int pos, int length) const;
	
	bool contains(const tpString& str) const;
	
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
	
	
	tpChar& operator [] ( tpSizeT idx ) { return m_buffer.ptr<tpChar>[idx]; }
	
	//const tpChar& operator [] const ( tpInt idx ) { return m_buffer.ptr<tpChar>[idx]; }
	
	
	//tpStringIterator *getBegin(); 
	
	tpString& operator << (const tpString& str);
	tpString& operator << (int i);
	tpString& operator << (long i);
	tpString& operator << (float r);
	tpString& operator << (double r);
	tpString& operator << (const char c);
	
	tpString& subst( tpChar c, const tpChar& substc );
	

	tpVoid __verbose_dump() const;
	
#endif


	template <typename T>
	T to() const { return T(0); }
	
protected:

	tpString& _assign(const char* str, tpUByte encoding = ASCII);
	tpString& _append(const char* str, tpUByte encoding = ASCII);
	tpString& _truncate(tpSizeT pos);
	
	tpChunk m_buffer;
	tpUShort m_encoding;
	
};


//////////////////////////////////////////////////////////////////////////


template<> inline int tpString::to<int>() const
{ 
	return atoi(this->c_str()); 
}

template<> inline long tpString::to<long>() const
{ 
	return atol(this->c_str()); 
}

template<> inline float tpString::to<float>() const
{ 
	return static_cast<float>(atof(this->c_str())); 
}



//////////////////////////////////////////////////////////////////////////


inline tpString operator + (const tpString& ls,const tpString& rs) 
{
	tpString res = ls;
	res.append(rs);
	return res;
}

inline bool operator == (const tpString& l, const tpString& r)
{
	return (0 == tpStrCmp(l.c_str(),r.c_str()));
}


inline bool operator != (const tpString& l, const tpString& r) 
{
	return (0 != tpStrCmp(r.c_str(),l.c_str()));
}




#if 0

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
	
	bool hasTokens() const;
	
	
	
protected:
	tpString m_string;
	tpString m_delim;
};


//////////////////////////////////////////////////////////////////////////

//const char* tpStrStr(const char* hay, char* needle);





// -----------------------------------------------------------------


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



/*

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
	
	bool finished() const 
	{
		return (0 == local.getLength());
	}
};
*/
//#include <tpStringIO.h>

#endif

#endif
