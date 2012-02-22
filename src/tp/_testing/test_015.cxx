#include <tp/primitive.h>

#include <tp/mutex.h>
#include <tp/scopelock.h>



int main(int argc, char* argv[]) {


	tpMutex mutex;

	tpScopeLock<tpMutex> lock(mutex);


	return 0;
}
