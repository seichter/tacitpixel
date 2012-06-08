#include <tp/array.h>
#include <tp/timer.h>
#include <tp/map.h>
#include <tp/log.h>

#include <vector>


int container_add(const int loop_count)
{

    tpArray<int> tp_vec;
    std::vector<int> stl_vec;

    tpTimer t;

    tpArray<size_t> sizes;


    // generate array with random size requests
    for (int l = 0; l < loop_count; l++)
        sizes.add( rand() * rand() );

    tpLog::get().printf("add_%d,",loop_count);


    // now fill a single vec consequently with the sizes
    t.start();
    for (size_t s = 0; s < sizes.getSize(); ++s)
    {
        for (int j = 0; j < sizes[s]; ++s)
        {
            tp_vec.add(rand());
        }
    }

    tpLog::get().printf("%3.3f,",t.getElapsed(tpTimer::kTimeMilliSeconds));


    t.start();
    for (size_t s = 0; s < sizes.getSize(); ++s)
    {
        for (int j = 0; j < sizes[s]; ++s)
        {
            stl_vec.push_back(rand());
        }
    }

    tpLog::get().printf("%3.3f\n",t.getElapsed(tpTimer::kTimeMilliSeconds));

    return 0;

}


int main(int argc, char* argv[])
{
    tpLog::get().printf("test,\ttacit,\tstl\n");

    container_add(100);
    container_add(1000);
    container_add(10000);
    container_add(100000);

    return 0;
}
