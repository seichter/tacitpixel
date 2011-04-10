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

#ifndef TPLOG_H
#define TPLOG_H


#include "tp/globals.h"
#include "tp/types.h"


//! different type of notifivation levels
enum tpLogLevelType {

    TP_LOG_NONE,		//!< nothing will be logged, even no errors
    TP_LOG_ERROR,		//!< only errors will be logged
    TP_LOG_WARNING,		//!< log warnings
    TP_LOG_MESSAGE,		//!< log messages
    TP_LOG_NOTIFY,		//!< log some more information
    TP_LOG_ALL			//!< just for the sake of completeness

};

typedef void (*tpLogFunc)(const char* cstr);


/*! \brief wrapper for outputstream for the logs
    \version 0.1
    \date 04.04.2002

    This describes a wrapper for the logging of messages.
    There is a singleton of it accessable through tpGlobalLog();
*/

class TP_API tpLog {
public:

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
