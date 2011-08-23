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
