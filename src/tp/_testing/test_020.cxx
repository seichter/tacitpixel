#include <tp/rendertarget.h>
#include <tp/window.h>
#include <tp/log.h>

#include <tp/script.h>


int main(int argc, char* argv[])
{
    tpString script;

    script = "s = 'hello world'; print(s); io.stdout:write(tostring(math.pi));";

    tpRefPtr<tpScript> luas = tpScript::create(tpScript::kLua);

    if (luas.isValid()) {

        luas->run(script);
    }


    return 0;
}


