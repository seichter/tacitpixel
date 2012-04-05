#include <tp/rendertarget.h>
#include <tp/window.h>
#include <tp/log.h>

int main(int argc, char* argv[])
{
    tpRefPtr<tpRenderBuffer> buffer = tpRenderBuffer::create(tpSize(320,240),tpPixelFormat::kRGBA_8888);

    tpLogNotify("render buffer %x",(tpULong)buffer.get());


    return 0;
}


