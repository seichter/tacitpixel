
#include <tp/thread.h>
#include <tp/log.h>
#include <tp/timer.h>
#include <tp/stringformater.h>

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

	
	// join thread into main thread
	// thread.join();

	tpString out;

	tpMat44r mat; tpMat44r s;

	mat.all(0).identity();
	mat(0,1) = 3.3; mat(1,0) = 5.5;
	mat.transpose().invert();

	mat.all(0).identity();
	s.all(0).identity();

	mat.setCellIdValue();
	s.setCellIdValue();
	s.transpose();

	out << mat << s;
	tpLog::get().printf("%s",out.c_str());


	out.empty();

	mat *= s;

	out << mat;
	tpLog::get().printf("%s",out.c_str());

	return 0;
}
