#ifndef TP_MUTEX_H
#define TP_MUTEX_H


class tpMutexHandle;

/**
	\brief mutex (Mutual Exclusive)

	For mutual execlusive code. Please use with care! Locked
	mutex'es are a common problem in multithreaded code. To
	get automatic mutex handling use the tpAutoLock.

	\see tpAutoLock

*/
class TP_API tpMutex
{
public:

	/**
	Constructor
	*/
	tpMutex();

	/**
	Destructor
	*/
	~tpMutex();


	/**
	\brief locks the mutex

	The mutex will be locked. No execution behind that point.
	*/
	void lock();

	/**
	\brief unlocks the mutex

	The mutex will be unlocked. Execution behind that point
	is allowed again.
	*/
	void unlock();

	/**
	\brief timed locking


	*/
	bool tryLock(tpULong timeout = 0);


private:

/*
	tpMutex(const tpMutex& mutex);


	tpVoid operator = (const tpMutex& mtx);
*/
	tpMutexHandle* m_lock;
};

#endif

