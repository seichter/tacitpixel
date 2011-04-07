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
