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

#include <tp/log.h>
#include <tp/referenced.h>
#include <tp/version.h>

#include <tp/string.h>
#include <tp/system.h>


#include <stdarg.h>
#include <stdio.h>


struct tpConsoleLog : tpLogCallback
{
	void operator()(const char* cstr)
	{
		::fputs(cstr,stdout);
		::fflush(stdout);
	}
};


struct tpFileLog : tpLogCallback {
	FILE* m_file;

	tpFileLog() : m_file(0)
	{
		m_file = fopen("twistedpair.txt","a+");
	}

	~tpFileLog()
	{
		if (m_file) fclose(m_file);
	}

	void operator()(const char* stuff) {
		if (m_file) ::fputs(stuff,m_file);
	}
};

void tpSetGlobalNotifyLevel(tpUInt level)
{
	tpLog::get().setLevel(level);
}


#if defined(_MSC_VER)
#define		_CRT_SECURE_NO_WARNINGS 1
#endif

#if defined(WIN32) || defined(WINCE)
	#if defined (GNUWIN32)
	#define tpVSNPRINTF vsnprintf
	#else
	#define tpVSNPRINTF _vsnprintf
	#endif
#elif defined(ANDROID)
	#define tpVSNPRINTF snprintf
#elif defined(__unix) || defined(__APPLE__) || defined(__BEOS__) || defined(__HAIKU__)
	#define tpVSNPRINTF vsnprintf
#elif defined(__SYMBIAN32__)
	#define tpVSNPRINTF snprintf
#endif

#define LOG_BUFFER_SIZE   (8192)

static char s_szBuf[LOG_BUFFER_SIZE];


void tpLogNotify(const char* szFormat, ...) {

   va_list argptr;

   va_start(argptr, szFormat);
   tpVSNPRINTF(s_szBuf, sizeof(s_szBuf), szFormat, argptr);

   va_end(argptr);

   tpLog::get().log(tpLog::kLogNotify,s_szBuf);

}


void tpLogMessage(const char* szFormat, ...) {

   va_list argptr;
   va_start(argptr, szFormat);
   tpVSNPRINTF(s_szBuf, sizeof(s_szBuf), szFormat, argptr);

   va_end(argptr);

   tpLog::get().log(tpLog::kLogMessage,s_szBuf);

}


void tpLogError(const char* szFormat, ... ) {

	va_list argptr;
	va_start(argptr, szFormat);
	tpVSNPRINTF(s_szBuf, sizeof(s_szBuf), szFormat, argptr);

	va_end(argptr);

	tpLog::get().log(tpLog::kLogError,s_szBuf);
}


////////////////////////////////////////////////////////////////////////////////

tpLog::tpLog() :
	mLevel(tpLog::kLogNotify)
{
#if defined(_WINCE)
	mLogCallbacks.add(new tpFileLog());
#else
	mLogCallbacks.add(new tpConsoleLog());
#endif

	this->log(tpLog::kLogNotify,tpGetVersionString());
}

tpLog::~tpLog()
{
	for (tpArray<tpLogCallback*>::iterator i = mLogCallbacks.begin();
		 i != mLogCallbacks.end();
		 ++i)
	{
		delete (*i);
	}
}

void tpLog::log(tpUShort logtype,const char* buf)
{
	if (logtype > mLevel) return;

	tpString output = tpSystem::get()->getTime();
	output += " ";
	output += buf;
	output += "\n";
	this->printf(output.c_str());
}

void tpLog::printf(const char* szFormat, ...)
{
   va_list argptr;
   va_start(argptr, szFormat);
   tpVSNPRINTF(s_szBuf, sizeof(s_szBuf), szFormat, argptr);
   va_end(argptr);

   for (tpArray<tpLogCallback*>::iterator i = mLogCallbacks.begin();
		i != mLogCallbacks.end();
		++i)
   {
	   (*(*i))(s_szBuf);
   }
}

/* static */
tpLog& tpLog::get() {
	static tpLog the_log;
	return the_log;
}

