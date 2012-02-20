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

#ifndef TP_STRING_H
#define TP_STRING_H


#include <tp/globals.h>
#include <tp/types.h>
#include <tp/chunk.h>
#include <tp/array.h>
#include <tp/stringutils.h>

class tpStringIterator;
class tpStringJoin;

/**
 * @brief Simple wrapper to handle null terminated c-string. Not yet optimized.
 */
class TP_API tpString {
public:

	enum {
		ASCII = 0
	};

	static const tpInt kNotFound = -1;

	//! default c'tor
	tpString();

	//! copy constructor from plain c-string
	tpString(const char* str);

	//! copy constructor from tpString
	tpString(const tpString& str);

	//! default d'tor
	~tpString();

	//! set contents from c-string
	tpString& set(const char* str);

	//! set content from a non-null terminated C string
	tpString& set(const char* buffer, tpSizeT size);


	//! clean contents of the string
	void empty();

	//! get the plain c-string
	const char* c_str() const { return &mBuffer[0]; }

	//! get the plain c-string
	char* c_str() { return &mBuffer[0]; }

	const char& at(tpSizeT pos) const { return mBuffer[pos]; }

	char& at(tpSizeT pos) { return mBuffer[pos]; }

	void toPascal(char** buffer) const;

	inline tpString& operator += (const tpString& rhs) { append(rhs); return *this; }
	inline tpString& operator += (const char* rhs) { append(rhs); return *this; }

	//! operator = to copy from right side value
	tpString& operator = (const char* rs) { set(rs); return *this; }
	//! operator = to copy from right side value
	tpString& operator = (const tpString& rs) { set(rs.c_str()); return *this; }

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
	tpString& append(const char& c);
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

	//! get the string after last occurence of c
	tpString afterLast(const char& c) const;

	//! get the string after first occurence of c
	tpString afterFirst(const char& c) const;

	//! get string before first occurrence of c
	tpString beforeFirst(const char& c) const;

	//! get string before last occurence of c
	tpString beforeLast(const char& c) const;

	//! find a sub string
	int find(const char* substr) const;

	tpString& substitute( const tpChar& c, const tpChar& substc );

	tpString substr( tpSizeT pos, tpSizeT len ) const;

	static tpString format(const char* format, ...);

	template <typename T>
	T to() const { return T(0); }

	template <typename T>
	T& operator [] ( tpSizeT idx ) { return static_cast<T>(&mBuffer[idx]); }

	template <typename T>
	const T& operator [] ( tpSizeT idx ) const { return static_cast<T>(&mBuffer[idx]); }

protected:

	tpString& _assign(const char* str);
	tpString& _append(const char* str);
	tpString& _truncate(tpSizeT pos);

	tpArray<tpChar> mBuffer;
	tpUShort mEncoding;

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


TP_API tpString tpStringFormat(const tpString& format, ... );


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
*/
//#include <tpStringIO.h>

#endif

typedef tpArray<tpString> tpStringArray;

#endif
