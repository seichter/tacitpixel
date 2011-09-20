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

#ifndef TPLOG_H
#define TPLOG_H


#include <tp/globals.h>
#include <tp/types.h>

typedef void (*tpLogFunc)(const char* cstr);


/*! \brief wrapper for outputstream for the logs
	\version 0.1
	\date 04.04.2002

	This describes a wrapper for the logging of messages.
	There is a singleton of it accessable through tpGlobalLog();
*/

class TP_API tpLog {
public:

	enum {
		kLogNone,	//!< nothing will be logged, even no errors
		kLogError,	//!< only errors will be logged
		kLogWarning,	//!< log warnings
		kLogMessage,	//!< log messages
		kLogNotify,	//!< log some more information
		kLogInfo,	//!< log verbose information
		kLogAll = 0xFF //!< log absolutely everything
	};

	void log(tpUShort logtype, const char* buf, int endline = 1);

	void printf(tpUShort logtype, const char* szString, ... );

	void setLevel(tpUShort level) {m_level = level;}
	tpUInt getLevel() const { return m_level; }

	static tpLog& get();

	void setBackend(tpLogFunc);

protected:

	tpLog();
	virtual ~tpLog();

	tpUInt	m_level;
	tpLogFunc m_func;


};

//! logs a normal message aka. notification
TP_API void tpLogMessage(const char* szString, ...);
//! logs an error
TP_API void tpLogError(const char* szString, ...);
//! logs notifications
TP_API void tpLogNotify(const char* szFormat, ...);
//! logs progress
TP_API void tpLogProgress(const char* szFormat, ...);

//! set the global notification level
TP_API void tpSetGlobalNotifyLevel(tpUShort level);



#endif
