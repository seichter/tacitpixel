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

#include <tp/thread.h>
#include <tp/mutex.h>


#if defined(HAVE_PTHREAD_H)
#include <pthread.h>
#endif

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

/*
	implementation for forward defined
	tpMutexHandle. Encapsulates the
	mutex handle.
*/

class tpMutexHandle
{
public:

	tpMutexHandle(){};

#if defined(HAVE_PTHREAD_H)
	pthread_mutex_t handle;
#else
	void *handle;
#endif
};


// ----------------------------------------------------------

tpMutex::tpMutex()
{
	m_lock = new tpMutexHandle();
#if defined(HAVE_PTHREAD_H)
	pthread_mutex_init(&m_lock->handle,NULL);
#elif defined(_WIN32)
	m_lock->handle = ::CreateMutex(0, 0, 0);
#else
	#error Unsupported platform
#endif
};


tpMutex::~tpMutex()
{
#if defined(HAVE_PTHREAD_H)
	pthread_mutex_destroy(&m_lock->handle);
#elif defined(_WIN32)
	::CloseHandle(m_lock->handle);
#else
	#error Unsupported platform
#endif
};

void tpMutex::lock()
{
#if defined(HAVE_PTHREAD_H)
	if (-1 == pthread_mutex_lock(&m_lock->handle))
#elif defined(_WIN32)
	if(::WaitForSingleObject(m_lock->handle, INFINITE) != WAIT_OBJECT_0)
#endif
	{
		//tpLogError("tpMutex::lock() : Error unlocking!");
	}
}

void tpMutex::unlock()
{

#if defined(_WIN32)
	if(::ReleaseMutex(m_lock->handle) == 0)
#elif defined(HAVE_PTHREAD_H)
	if (-1 == pthread_mutex_unlock(&m_lock->handle))
#endif
	{
		//tpLogError("tpMutex::unlock() : Error unlocking!");
    }
}

bool tpMutex::tryLock(tpULong timeout)
{
#if defined(_WIN32)
	switch(::WaitForSingleObject(m_lock->handle,timeout))
	{
      case WAIT_OBJECT_0:
		  return true;
	  case WAIT_TIMEOUT:
		  return false;
	  default:
		  break;
	};
#elif defined(HAVE_PTHREAD_H)
	if (0 == pthread_mutex_trylock(&m_lock->handle)) return true;
#endif
	//tpLogError("tpMutex::tryLock() : Error!");

	return false;
}

