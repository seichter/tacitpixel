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
#include <tp/config.h>

#if defined (TP_OS_IOS)
#define HAVE_SYS_TIME_H
#define HAVE_GETTIMEOFDAY
#endif

#if defined(HAVE_TIME_H)
#	include <time.h>
#endif

#if defined(HAVE_SYS_TIME_H)
#	include <sys/time.h>
#endif

#if defined(WIN32) || defined(WINCE)
#	include <windows.h>
#endif


tpTimer::tpTimer()
	: mSecondsPerTick(1.0 / tpDouble(1000000))
{
#if defined(_WIN32)
	LARGE_INTEGER _freq;
	if(QueryPerformanceFrequency(&_freq))
	{
		mSecondsPerTick = 1.0/(tpDouble)_freq.QuadPart;
	}
#endif

	start();

}

tpTimer::~tpTimer()
{
}

void tpTimer::start()
{
	getCurrentTick(mStart);
}

tpDouble tpTimer::getElapsed( tpUInt scale ) const
{
	tpTimerTick r;
	getCurrentTick(r);

	tpDouble outputScale = 1.;
	switch (scale) {
		case kTimeMicroSeconds:
			outputScale = 1e6;
			break;
		case kTimeMilliSeconds:
			outputScale = 1e3;
			break;
		case kTimeSeconds:
		default:
			outputScale = 1.;
	}

	return tpDouble((r - mStart) * mSecondsPerTick * outputScale);
}

/* static */
void tpTimer::getCurrentTick(tpTimerTick& val) {

#if defined(_WIN32)
	LARGE_INTEGER _value;

	if ( QueryPerformanceCounter(&_value) )
	{
		val = _value.QuadPart;
	}
#elif defined(HAVE_CLOCK_GETTIME)
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME,&ts);
	val  = ts.tv_sec;
	val *= 1000000;
	val += ts.tv_nsec / 1000;
#elif defined(HAVE_GETTIMEOFDAY)
	timeval tv;
	gettimeofday(&tv, 0);
	val = (tpTimerTick)(tv.tv_sec * 1000000 + tv.tv_usec);
#endif
}
