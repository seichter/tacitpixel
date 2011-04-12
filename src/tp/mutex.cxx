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

