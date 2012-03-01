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

#ifndef TP_TIMER_H
#define TP_TIMER_H

#include <tp/globals.h>
#include <tp/types.h>

typedef tpUInt64 tpTimerTick;

/**
  * @brief a high resolution timer
  * @code
  * tpTimer timer;
  * timer.start();
  * // do some stuff
  * tpDouble elapsed_time_in_seconds = timer.getElapsed(tpTimer::kTimeSeconds);
  * @endcode
  */
class TP_API tpTimer {
public:

	enum {
		kTimeMicroSeconds,
		kTimeMilliSeconds,
		kTimeSeconds
	};


	/**
	  * @brief c'tor - will reset the timer (call ::start())
	  */
	explicit tpTimer();

	/**
	  * @brief d'tor
	  */
	~tpTimer();

	/**
	  * @brief starts the timer
	  */
	void start();

	/**
	  * @brief returns the time elapsed since last call of start()
	  * @param scale is the scale of the returned value
	  * @return time in units
	  */
	tpDouble getElapsed(tpUInt scale) const;

	//! static function to get the current time
	static void getCurrentTick(tpTimerTick& val);


protected:

	tpTimerTick mStart;
	tpDouble mSecondsPerTick;

};




#endif
