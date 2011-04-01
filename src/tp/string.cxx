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

#include "tp/string.h"

#include "tp/hash.h"


//#include "tpLog.h"


//#include "pformat.h"

#define TP_NOTFOUND -1

#if 0
wchar_t * locale_to_wchar(const char * str)
{
	wchar_t * ptr;
	size_t s;

	/* first arg == NULL means 'calculate needed space' */
	s = mbstowcs(NULL, str, 0);

	/* a size of -1 is triggered by an error in encoding; never
	happen in ISO-8859-* locales, but possible in UTF-8 */
	if (s == -1)
		return NULL;

	/* malloc the necessary space */
	if ((ptr = (wchar_t *)malloc((s + 1) * sizeof(wchar_t))) == NULL)
		return NULL;

	/* really do it */
	mbstowcs(ptr, str, s);

	/* ensure NULL-termination */
	ptr[s] = L'\0';

	/* remember to free() ptr when done */
	return ptr;
}
#endif

#if 0 //defined(ANDROID)

tpVoid tpLocale2WChar( const char* str, tpArray<tpWChar>& dst )
{
	tpLogMessage("NO WCHAR SUPPORT");
}

tpVoid tpWChar2Locale( const tpWChar* src, tpArray<tpChar>& dst )
{
	tpLogMessage("NO WCHAR SUPPORT");
}

#else

tpVoid tpLocale2WChar( const char* str, tpArray<tpWChar>& dst )
{
	if (str)
	{
		size_t s = mbstowcs(0, str, 0);
		if (s == -1) {
			return;
		}
		
		dst.resize(s+1);
		s = mbstowcs(dst.getData(),str,s);
		dst[s] = 0;
	}
}

tpVoid tpWChar2Locale( const tpWChar* src, tpArray<tpChar>& dst )
{
	if (src)
	{
		size_t s = wcstombs(0, src, 0);
		if ( s == -1 ) return;

		dst.resize(s + 1);
		wcstombs(dst.getData(),src,s);
		dst[s] = L'\0';
	}
}
#endif

//////////////////////////////////////////////////////////////////////////

int __tppformat( int flags , void * dest, int len, const char * fmt, va_list args)
{
//	return __pformat(flags, dest, len, fmt, args);
	return 0;
}

int __tpvsnprintf( char *buf, size_t length, const char *fmt, va_list argv )
{
	
	register int retval;
#if 0
	if( length == (size_t)(0) )
    /*
     * No buffer; simply compute and return the size required,
     * without actually emitting any data.
     */
		return __pformat( 0, buf, 0, fmt, argv );
	
	/* If we get to here, then we have a buffer...
	 * Emit data up to the limit of buffer length less one,
	 * then add the requisite NUL terminator.
	 */
	retval = __pformat( 0, buf, --length, fmt, argv );
	buf[retval < length ? retval : length] = '\0';
#endif
	
	return retval;
}

//////////////////////////////////////////////////////////////////////////

tpString::tpString() : m_stringbuffer(tpArray<tpChar>()), m_wcharcopy(tpArray<tpWChar>())
{
	//setlocale(LC_ALL,"");
}

tpString::tpString(const char* str)
{
	set( str );
}


tpString::tpString(const char* str, unsigned int size)
{
	set( str, size );
}


tpString::tpString(const wchar_t* str)
{
	set( str );
}

tpString::tpString(const tpString& str)
{
	set( str.c_str() );
}

//////////////////////////////////////////////////////////////////////////


tpString& tpString::set( const tpChar* buffer, tpUInt size )
{
	if (size)
	{
		m_stringbuffer.copy(buffer,size);
		m_stringbuffer.add(0);
		syncCStr2WStr();
	}

	return *this;
}

tpString& tpString::set(const char* str)
{
	tpUInt len = tpStrLen(str);
	if (len)
	{
		m_stringbuffer.resize(len+1);
		memcpy(m_stringbuffer.getData(),str,len+1);
	} else {
		m_stringbuffer.clear();
	}
	return *this;
}

tpString& tpString::set( const wchar_t* str )
{
	tpWChar2Locale( str, m_stringbuffer );
	syncCStr2WStr();
	return *this;
}

//////////////////////////////////////////////////////////////////////////

tpBool tpString::isEmpty() const
{
	return ( 0 == m_stringbuffer.getSize() ? true : 0 == getLength() );
}

void tpString::empty()
{
	m_stringbuffer.empty();
	if (m_stringbuffer.getMaxSize() > 0) m_stringbuffer[0] = '\0';
	m_wcharcopy.empty();
	if (m_wcharcopy.getMaxSize() > 0) m_wcharcopy[0] = L'\0';
}

const char* tpString::c_str() const
{
	return m_stringbuffer.isEmpty() ? 0 : m_stringbuffer.getData();
}


tpInt tpString::getPascal( char** buffer ) const
{
	int _length = this->getLength();

	if (*buffer == 0) 
	{
		*buffer[0] = (_length > 255) ? 255 : _length;
		strncpy(buffer[1],c_str(),_length);	
	}
	
	return _length;
}

    
tpString::~tpString()
{
}


tpSizeT tpString::getLength() const
{
	return (m_stringbuffer.getSize()) ? tpStrLen(m_stringbuffer.getData()) : 0;
}


tpString& tpString::append(const tpString& str)
{
	return append(str.c_str());  	
}

tpString& tpString::removeAfter(const tpChar& end)
{
	if (!isEmpty())
	{
		tpInt idx = find(end,false);
		if (idx > TP_NOTFOUND)
		{
			m_stringbuffer.resize(idx);
		}
	}
	return *this;
}

tpString& tpString::append(const char* rs)
{
	if (rs)
	{
		removeAfter('\0'); // sanity check - in case an intermediate '0' has been added
		const tpChar* cptr = &rs[0];
		while (cptr && *cptr) {
			m_stringbuffer.add(*cptr); cptr++;
		}
		m_stringbuffer.add(0);
	}
	return *this;
}

unsigned long tpString::getHash() const
{
	return (getLength()) ? tpGetHash((tpUChar*)m_stringbuffer.getData(),getLength()) : 0;
}


int tpString::find(char c,tpBool fromright) const
{
	if ( isEmpty() ) return TP_NOTFOUND;
	const char* _pc = (fromright) ? tpStrRChr(c_str(),c) : tpStrChr(c_str(),c);
	return (_pc) ? _pc - (const char*)c_str() : TP_NOTFOUND;
};


tpString tpString::afterLast(const char& c) const
{
	tpString _ret;
	int _pos = find(c,TRUE);

	if (TP_NOTFOUND == _pos) return *this;

	_ret = (const char*)c_str() + _pos + 1;

	return _ret;
};

tpString tpString::afterFirst(const char& c) const
{
	tpString _ret;
	int _pos = find(c);

	if (0 > _pos) return *this;

	_ret = (const char*)c_str() + _pos + 1;

	return _ret;
};

tpString tpString::beforeFirst(const char& c) const
{

	tpString _ret;
	
	const char* _pc = (const char*)c_str();

	while (*_pc != 0 && *_pc != c)
	{
		_ret << *_pc;
		_pc++;
	};

	return _ret;
};


tpString tpString::beforeLast(const char& c) const
{
	tpString _ret;
	int pos = find(c,TRUE);
	
	if (pos != TP_NOTFOUND) _ret = tpString(m_stringbuffer.getData() + pos);

	return _ret;
};


tpString tpString::between(char leftc, char rightc) const
{
	tpString _ret;
	
	int _rpos = find(rightc,TRUE);
	int _lpos = find(leftc,FALSE);
	
	
	// copy if right and left are not the same
	_ret = sub(_lpos + 1,_rpos - _lpos);

	return _ret;
	
};

long tpString::toLong() const
{
	return atol(c_str());
};


float tpString::toFloat() const
{
	return (float)atof(c_str());
}


int tpString::toInteger() const
{
	return (c_str()) ? atoi(c_str()) : 0;
}

void tpString::format(const char* format,...)
{
	va_list argptr;
	va_start(argptr, format);

	char _buf[TP_MAXBUFSIZE];

#ifndef _WIN32
	vsnprintf(&_buf[0],TP_MAXBUFSIZE,format, argptr);
#elif (__SYMBIAN32__)
	snprintf(&_buf[0],TP_MAXBUFSIZE,format, argptr);
#else	
	_vsnprintf(&_buf[0],TP_MAXBUFSIZE,format, argptr);
#endif

	set(_buf);
	
	va_end(argptr);

};


tpString& tpString::operator = (const char* rs)
{
	set(rs);
	return *this;
};

tpString& tpString::operator = (const tpString& rs)
{
	return set(rs.c_str());
}

tpString& tpString::operator=( const tpWChar* rs )
{
	return set(rs);
}

tpString& tpString::operator << (const tpString& str)
{
	append(str);
	return *this;
};

tpString& tpString::operator << (int i)
{
	tpString _str;
	_str.format("%d",i);
	append(_str);
	return *this;
};


tpString& tpString::operator << (long i)
{
	tpString _str;
	_str.format("%d",i);
	append(_str);
	return *this;
};


tpString& tpString::operator << (float r)
{
	tpString _str;
	_str.format("%f",r);
	append(_str);
	return *this;
};

tpString& tpString::operator << (double r)
{
	tpString _str;
	_str.format("%lf",r);
	append(_str);
	return *this;
};


tpString& tpString::operator << (const char c)
{
	tpString _str;
	_str.format("%c",c);
	append(_str);
	return *this;
};


int tpString::find(const char* sub) const
{
	const char *pf = tpStrStr(c_str(),(char*)sub);

	return (pf) ? pf - (const char*)c_str() : TP_NOTFOUND;

}


tpBool tpString::contains(const tpString& str) const
{
	return find(str.c_str()) > TP_NOTFOUND;
};


tpString tpString::sub(int pos, 
					   int length) const
{

	tpString _res = c_str() + pos;
	_res.m_stringbuffer[length] = '\0';
	
	return _res;
};

tpArray<tpString> tpString::split(const tpString& separator) const
{

	tpArray<tpString> _res;
	
	tpStringTokenizer _tok(*this,separator);

	while (_tok.hasTokens()) _res += _tok.nextToken();

	return _res;
}

const wchar_t* tpString::wc_str() const
{
	return m_wcharcopy.getData();
	//return 0;
}

tpString& tpString::subst( tpChar c, const tpChar& substc )
{
	tpChar* cptr = m_stringbuffer.getData();
	for (tpSizeT i = 0; i < getLength();i++)
	{
		if (*cptr == c) *cptr = substc; cptr++;
	}

	return *this;
}

tpVoid tpString::__verbose_dump() const
{
	tpLogMessage("%s - length: %d, size: %d, max_size: %d",
		__FUNCTION__,getLength(),m_stringbuffer.getSize(),m_stringbuffer.getMaxSize());

	const tpChar* cptr = m_stringbuffer.getData();
	for (tpSizeT i = 0; i < getLength() + 1; i++ )
	{
		tpLogMessage("%s - 0x%x '%c'",__FUNCTION__,(long)*cptr,*cptr ? *cptr : '@'); cptr++;
	}
}

void tpString::syncCStr2WStr()
{
	tpLocale2WChar( m_stringbuffer.getData(), m_wcharcopy );
}

void tpString::syncWStr2CStr()
{
	tpWChar2Locale( m_wcharcopy.getData(), m_stringbuffer );
}

//tpBool tpString::operator==( const tpString& rs )
//{
//	if (isEmpty() && rs.isEmpty()) return true;
//	return (0 == tpStrCmp(c_str(),rs.c_str()));
//}
// Utils //

//tpString& tpString::join(int unused,va_arg arg)
//{
//	va_list vl;
//	va_start(vl,unused);
//
//}
//

// ---------------------------------------------------------------------

/*
const char* tpStrStr(const char* hay, char* needle)
{
	return strstr(hay, needle);
};
*/



tpStringTokenizer::tpStringTokenizer(const tpString& str,
									 const tpString& delim):
	m_string(str),
	m_delim(delim)
{
};

tpString tpStringTokenizer::nextToken()
{

	tpString _res;

	int _pos = m_string.find(m_delim.c_str());
	
	if (_pos > TP_NOTFOUND)
	{
		_res = m_string.sub(0,_pos);

		m_string = m_string.sub(_pos + m_delim.getLength(),
			m_string.getLength() - _pos - m_delim.getLength());

	} else
	{
		_res = m_string.sub(0,m_string.getLength());
		m_string = "";
	}
	
	return _res;
};

tpBool tpStringTokenizer::hasTokens() const
{
	return !m_string.isEmpty();
}




#if defined(__SYMBIAN32__) && defined(__GCCE__)
#	include <staticlibinit_gcce.h>
#endif

