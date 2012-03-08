
#include <tp/window.h>


int main(int argc, char* argv[])
{

    tpRefPtr<tpRenderBuffer> framebuffer;
    tpRefPtr<tpImage> image;


    framebuffer = tpRenderBuffer::create(tpSize(320,240),tpPixelFormat::kBGR_888);
    framebuffer->setContext(0);
    framebuffer->copy(*image);

    return 0;
}
