

#include <tp/viewer.h>


int main(int argc, char* argv[])
{
    tpRefPtr<tpViewer> viewer = new tpViewer();
    viewer->create();
    return viewer->run();
}
