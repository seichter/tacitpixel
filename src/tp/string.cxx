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

#include <tp/string.h>

#include <tp/hash.h>

#include <string.h>
#include <stdio.h>

tpString::tpString()
{
	this->empty();
}

tpString::tpString(const char* str, tpUShort encoding /* tpString::ASCII */) : m_encoding(encoding)
{
	set( str );
}


tpString::tpString(const char* str, unsigned int size,  tpUShort encoding /* tpString::ASCII */)
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


tpString& tpString::set( const char* buffer, tpSizeT size )
{
	return *this;
}

tpString& tpString::set(const char* str)
{
	tpUInt len = tpStrLen(str);
	
	m_buffer.reserve<char>(len+1);
	memcpy(m_buffer.getData(),str,len);
	m_buffer.at<char>(len) = '\0';

	return *this;
}

tpString& tpString::set( const wchar_t* str )
{
	tpSizeT len = tpStrLen(str);
	m_buffer.reserve<wchar_t>(len + 1);
	memcpy(m_buffer.getData(),str,len * sizeof(wchar_t));
	m_buffer.at<wchar_t>(len) = L'\0';

	return *this;
}

//////////////////////////////////////////////////////////////////////////

bool tpString::isEmpty() const
{
	return ( 0 == getLength() );
}

void tpString::empty()
{
	m_buffer.setSize(0);
	m_buffer.at<long>(0) = 0L;
}

tpInt tpString::getPascal( char** buffer ) const
{
	
	int _length = this->getLength();
	
	#if 0
	if (*buffer == 0) 
	{
		*buffer[0] = (_length > 255) ? 255 : _length;
		strncpy(buffer[1],c_str(),_length);	
	}
	#endif
	
	return _length;
}

    
tpString::~tpString()
{
}


tpSizeT 
tpString::getLength() const
{
	tpSizeT lw = tpStrLen(m_buffer.ptr<wchar_t>());
	tpSizeT lc = tpStrLen(m_buffer.ptr<char>());
	return (lw > lc) ? lw : lc;
}

tpString&
tpString::append(const tpString& other)
{
	this->append(other.c_str());
}

tpString&
tpString::append(const char* other)
{
	tpSizeT input_length = tpStrLen(other);
	if (input_length)
	{
		tpSizeT local_length = getLength();

		input_length++;
		
		m_buffer.reserve<char>(local_length + input_length);

		for (tpSizeT i = 0; i < input_length; ++i)
		{
			m_buffer.at<char>(i + local_length) = other[i];
		}
	}
	
	return *this;
}

tpString&
tpString::prepend(const char* other)
{
	tpString result(other);
	result.append(*this);
	*this = result;
	return *this;
}



#if 0


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


int tpString::find(char c,bool fromright) const
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


bool tpString::contains(const tpString& str) const
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

//bool tpString::operator==( const tpString& rs )
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

bool tpStringTokenizer::hasTokens() const
{
	return !m_string.isEmpty();
}


#endif

#if defined(__SYMBIAN32__) && defined(__GCCE__)
#	include <staticlibinit_gcce.h>
#endif

