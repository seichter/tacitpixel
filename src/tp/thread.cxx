
#include <tp/thread.h>


// Includes for Windows and *nic'es
#if defined(_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#if !defined(WINCE)
	#include <process.h>
#else
	//#include <atlbase.h>
	// Windows Mobile doesn't provide _beginthreadex or _beginthread. Note that
	// unlike on the desktop, CreateThread for WinCE does support the CRT.
/*
	uintptr_t _beginthreadex(void *security,
			unsigned stack_size,
			unsigned (*start_address)(void*),
			void *arglist,
			unsigned initflag,
			unsigned *thrdaddr) 
	{
				return reinterpret_cast<uintptr_t>(CreateThread(
					reinterpret_cast<SECURITY_ATTRIBUTES*>(security),
					stack_size,
					reinterpret_cast<DWORD (*)(void*)>(start_address),
					arglist,
					initflag,
					reinterpret_cast<DWORD*>(thrdaddr)));
	}
*/


#endif

unsigned int __stdcall tpWin32ThreadDispatch(void *arg)
{
    tpThread* t = (tpThread*)arg;
	if (t) t->doRun(arg);

	return 0;
}

#elif defined(HAVE_PTHREAD_H)

#include <pthread.h>

// Dispatching function for pthreads
extern "C" void *tpPThreadDispatch(void *arg)
{

	tpThread* t = (tpThread*)arg;

	if (t) t->doRun(arg);
	/*
	if (t) t->onStop();

	pthread_exit((void**)0);
	*/
	return (void*)0;

}

#endif

tpThread::tpThread()
: m_run(false), m_thread(0L)
{
}

tpThread::~tpThread()
{
	if (m_run) stop();
};


void tpThread::start()
{
    
	m_run = true;
#if defined(_WIN32)
	//_beginthreadex(0, 0, tpWin32ThreadDispatch, this, 0, &_threadid);
	// _beginthread(tpThreadDispatch, 0, this);
	CreateThread( 0, 0, 
		reinterpret_cast<LPTHREAD_START_ROUTINE>(tpWin32ThreadDispatch), 
		this,0,static_cast<LPDWORD>(m_thread));

#elif defined(HAVE_PTHREAD_H)
	pthread_t _thread;
	pthread_create(&_thread,NULL,tpPThreadDispatch,(void*)this);
#endif

};


void tpThread::stop()
{
	m_run = false;
}


void tpThread::onStop()
{
}


/* static */
void tpThread::doRun(void* userdata)
{
	static_cast<tpThread*>(userdata)->onRun();
}

void tpThread::doStop()
{
 	this->onStop();
#if defined(WIN32)
	//_endthreadex(0);
	ExitThread(0);
#elif defined(HAVE_PTHREAD_H)
	pthread_exit(NULL);
#endif


};
