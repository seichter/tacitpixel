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
#include <tp/array.h>


/**
  * @brief callback used for the logger
  */
struct tpLogCallback {
	virtual void operator()(const char* cstr) = 0;
	virtual ~tpLogCallback() {}
};

/**
  * @brief multipurpose logger used internally
  */
class TP_API tpLog {
public:

	enum {
		kLogNone,		//!< nothing will be logged, even no errors
		kLogError,		//!< only errors will be logged
		kLogWarning,		//!< log warnings (non-critical errors)
		kLogMessage,		//!< log messages (something to note but not an error)
		kLogNotify,		//!< log some more information
		kLogInfo,		//!< log verbose information
		kLogAll = 0xFF	//!< log absolutely everything
	};

	/**
	  * @brief logs (adds time and a carriage return to the message)
	  * @param logtype provides a level this message belongs to
	  * @param buf null terminated cstring that is being logged
	  */
	void log(tpUShort logtype, const char* buf);

	/**
	  * @brief does the usual printf with a message
	  * @param szString formating string
	  * @param ... parameters that should correspond to the formating
	  */
	void printf(const char* szString, ... );

	/** sets the logging level */
	void setLevel(tpUShort level) {mLevel = level;}

	/** gets the logging level */
	tpUShort getLevel() const { return mLevel; }

	/**
	  * @brief getter for the singleton
	  * @return returns a reference to the singleton
	  */
	static tpLog& get();

	void operator << (const char* input) { this->printf("%s",input); }

protected:

	tpLog();
	virtual ~tpLog();

	tpArray<tpLogCallback*> mLogCallbacks;
	tpUShort	mLevel;
};

//! logs a normal message aka. notification
TP_API void tpLogMessage(const char* szString, ...);
//! logs an error
TP_API void tpLogError(const char* szString, ...);
//! logs notifications
TP_API void tpLogNotify(const char* szFormat, ...);

//! set the global notification level
TP_API void tpSetGlobalNotifyLevel(tpUShort level);


/**
  * @brief helper for changing the log level in a scope
  */
struct tpScopeLogLevel
{
	tpUShort levelOutside;
	explicit tpScopeLogLevel(tpUShort level)
	{
		levelOutside = tpLog::get().getLevel();
		tpLog::get().setLevel(level);
	}
	~tpScopeLogLevel()
	{
		tpLog::get().setLevel(levelOutside);
	}
};

template <tpUShort level>
struct tpScopeLog {
	const char* info;
	explicit tpScopeLog(const char* i) : info(i) {
		tpLog::get().log(level,info);
	}

	~tpScopeLog() {
		tpLog::get().log(level,info);
	}
};

#endif
