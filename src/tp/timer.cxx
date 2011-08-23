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
