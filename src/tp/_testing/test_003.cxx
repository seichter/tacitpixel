
#include <tp/thread.h>
#include <tp/log.h>
#include <tp/timer.h>

#include <cstdio>

struct MyRunnable : tpRunnable
{
	void run()
	{
		for (int i = 0; i < 100; ++i)
		{
			tpLog::get().printf("From worker thread %d\n",i);
			tpThread::sleep(10);

		}
	}

	~MyRunnable()
	{
		tpLog::get().printf("From worker thread %s\n",__FUNCTION__);
	}
};

int main(int argc, char* argv[])
{
	tpThread thread(new MyRunnable());

	thread.start();

	for (int i = 0; i < 100; ++i)
	{
		tpLog::get().printf("From calling thread %d\n",i);
		tpThread::sleep(5);
	}

	//thread.join();

	return 0;
}
