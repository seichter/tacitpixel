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

#ifndef TPTIMER_H
#define TPTIMER_H

#include <tp/globals.h>
#include <tp/types.h>

const tpDouble TP_TIME_MICROSEC = 1e6;
const tpDouble TP_TIME_MILLISEC = 1e3;
const tpDouble TP_TIME_SEC      = 1;

typedef tpUInt64 tpTimerTick;


TP_API void tpYield();

TP_API void tpMilliSleep(tpULong milliseconds);



/*!
	\class tpTimer
	\brief a high resolution timer

	This timer provides access to high resolution timing
	on the respective platform. The timing is done like 
	this:
	\code
	tpTimer atimer;

	atimer.start();

	// ... do the measured things


	tpDouble elapsed_in_sec = atimer.getElapsed(TP_TIME_SEC);

	\endcode
*/

class TP_API tpTimer {
public:

	/*! c'tor
	*/
	tpTimer();
	/*! d'tor
	*/
	~tpTimer();
	
	/*! start timer
	*/
	void start();

	/*! Get the elapsed time in the given scale.
		\brief get elapsed time
		\param scale time scale for reporting
		\return time in respective scale (usec,msec or sec ...)		
	*/
	tpDouble getElapsed(tpDouble scale = TP_TIME_SEC) const;
	
	
protected:

	//! static function to get the current time
	static void getCurrentTick(tpTimerTick& val);
		
	tpTimerTick m_start;
	tpDouble m_secondsPerTick;
	
};




#endif
