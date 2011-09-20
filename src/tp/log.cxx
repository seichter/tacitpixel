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


class tpFileLog : public tpReferenced {
	FILE* m_file;
public:
	tpFileLog() : m_file(0) {
		m_file = fopen("twistedpair.txt","a+");
	}

	~tpFileLog() {
		if (m_file) fclose(m_file);
	}

	void log(const char* stuff) {
		if (m_file) fprintf(m_file,"%s",stuff);
	}

	static tpFileLog& get() {
		static tpFileLog thefilelog;
		return thefilelog;
	}

};

void tpFileLogFunction(const char* stuff) {
	tpFileLog::get().log(stuff);
}



void tpDefaultLogFunction(const char* stuff) {
	fprintf(stdout,"%s",stuff);
}

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
#elif defined(__unix) || defined(__APPLE__)
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


void tpLogProgress(const char* szFormat, ... ) {

	va_list argptr;
	va_start(argptr, szFormat);
	tpVSNPRINTF(s_szBuf, sizeof(s_szBuf), szFormat, argptr);

	va_end(argptr);

	tpLog::get().log(tpLog::kLogInfo,s_szBuf,0);
}

////////////////////////////////////////////////////////////////////////////////

tpLog::tpLog() :
	m_level(tpLog::kLogNotify),
	m_func(0)
{
	m_func =
#if defined(WINCE)
		&tpFileLogFunction;
#else
		&tpDefaultLogFunction;
#endif

	this->log(tpLog::kLogNotify,tpGetVersionString(), 1);

}

tpLog::~tpLog() {
}

void tpLog::log(tpUShort logtype, const char* buf, int endline)
{
	if (logtype > m_level) return;

	if (m_func) {
		tpString t = tpSystem::get()->getTime();
		m_func(t.c_str());
		m_func(" ");
		m_func(buf);
		if (endline) m_func("\n");
	}
}

void tpLog::printf(tpUShort logtype, const char* szFormat, ...)
{

	if (logtype >= m_level) return;

   va_list argptr;
   va_start(argptr, szFormat);
   tpVSNPRINTF(s_szBuf, sizeof(s_szBuf), szFormat, argptr);

   va_end(argptr);

   tpLog::get().log(logtype,s_szBuf);

}



/* static */
tpLog& tpLog::get() {
	static tpLog the_log;
	return the_log;
}

void tpLog::setBackend( tpLogFunc func) {
	m_func = func;
}

