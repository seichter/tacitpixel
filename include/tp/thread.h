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

#ifndef TP_THREAD_H
#define TP_THREAD_H

#include <tp/globals.h>
#include <tp/types.h>
#include <tp/scopeptr.h>


class tpThread;

/**
  * @brief wrapper for running code within threads or timers
  */
struct TP_API tpRunnable {
	/**
	  * @brief needs to be overwritten and should contain the time critical code
	  */
	virtual void run() = 0;

	virtual ~tpRunnable() {};
};

/** structure holding the handle of the threading implementation */
struct tpThreadHandle;

/**
  * @brief wrapper for managing threads
  */
class TP_API tpThread {
public:

	enum {
		kStateNew = 0,
		kStateRun,
		kStateStop
	};

	/**
	  * @brief C'tor
	  * @param runnable object to be executed within the thread
	  */
	tpThread(tpRunnable* runnable);

	/**
	  * @brief D'tor
	  */
	~tpThread();

	/**
	  * @brief starts the execution of the thread
	  */
	void start();

	/**
	  * @brief stops the execution of the thread
	  */
	void stop();

	/**
	  * @brief calls ::run() on the runnable
	  */
	void run();

	/**
	  * @brief joins the thread into the calling thread
	  */
	void join();

	void detach();

	/**
	  * @brief calling thread being suspended
	  * @param milliseconds time in milliseconds
	  */
	static void sleep(tpULong milliseconds);

	/**
	  * @brief returns time to the calling thread
	  */
	static void yield();

protected:

	tpUInt mState;

	tpScopePtr<tpRunnable> mRunnable;
	tpScopePtr<tpThreadHandle> mThreadHandle;

};

#endif
