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

#ifndef TPTHREAD_H
#define TPTHREAD_H

#include <tp/globals.h>
#include <tp/types.h>


/*!
	\class tpThread
	\brief extreme simple threading object

	You need to derive your own class from tpThread. If you
	need more comprehensible threading I suggest you go
	for http://zthread.sourceforge.net which is very nice and
	can be easily integrated. Nevertheless, tpThread is enough
	for most of the tasks. tpThread is an interface as you need
	to overload onRun()

	To implement a thread follow this outline:
	\code

	class myThread : public tpThread
	{
	public:
	myThread() : tpThread()
	{
	};

	virtual void onRun()
	{
		while (isRunning())
		{
			cout << "The thread ..." << endl;
		};
	};
	};

	myThread* t = new myThread();
	t->start();

	// do other important stuff

	t->stop();

	\endcode
*/
class TP_API tpThread {
public:
	/** c'tor
	*/
	tpThread();
	//! d'tor
	virtual ~tpThread();

	//! start thread
	void start();

	//! stop running thread
	void stop();

	//! check if the thread is running
	bool isRunning() const { return m_run; }

	//! the task needs to be put in here!
	virtual void onRun() = 0;

	//! overload this for cleaning up
	virtual void onStop();

	static void doRun(void*);

protected:


	void doStop();

	bool m_run;


private:

	void* m_thread;

};

#endif
