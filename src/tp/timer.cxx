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

#include <tp/timer.h>

#if defined(WIN32) || defined(WINCE)
#	include <windows.h>
#	include <time.h>
#elif defined(__unix) || defined(__APPLE__)
#	include <time.h>
#	include <sys/time.h>
#endif


/* static */
void tpMilliSleep(tpULong milliseconds) 
{
	
#if defined(__unix) || defined(__APPLE__)
	
	timespec tmReq;
	tmReq.tv_sec = (time_t)(milliseconds / 1000);
	tmReq.tv_nsec = (milliseconds % 1000) * 1000 * 1000;
	
	// we're not interested in remaining time nor in return value
	nanosleep(&tmReq, (timespec *)NULL);
#elif defined(WIN32)
	Sleep(milliseconds);
#endif
	
}

tpVoid tpYield()
{
#if defined(__unix) || defined(__APPLE__)

	timespec tmReq;
	tmReq.tv_sec = 0;
	tmReq.tv_nsec = 0;

	// we just want to give back a minimum to the system
	nanosleep(&tmReq, (timespec *)NULL);
#elif defined(WIN32)
	Sleep(0);
#endif

}

tpTimer::tpTimer() : m_secondsPerTick(1.0 / tpDouble(1000000)) {
#if defined(_WIN32)
	LARGE_INTEGER _freq;
	if(QueryPerformanceFrequency(&_freq))
	{
		m_secondsPerTick = 1.0/(tpDouble)_freq.QuadPart;
	}
#endif

	start();

}

tpTimer::~tpTimer() 
{
}

void tpTimer::start() 
{
	getCurrentTick(m_start);
}

tpDouble tpTimer::getElapsed( tpDouble scale /*= TP_TIME_SEC*/ ) const
{
	tpTimerTick r;
	getCurrentTick(r);
	return tpDouble((r - m_start) * m_secondsPerTick * scale);
}
	
/* static */
void tpTimer::getCurrentTick(tpTimerTick& val) {
#if defined(__unix) || defined(__APPLE__)
	
	timeval tv;
	gettimeofday(&tv, 0);
	val = (tpTimerTick)(tv.tv_sec * 1000000 + tv.tv_usec);

#elif defined(_WIN32)

	LARGE_INTEGER _value;

	if ( QueryPerformanceCounter(&_value) ) 
	{
		val = _value.QuadPart;
	}

#endif
}
