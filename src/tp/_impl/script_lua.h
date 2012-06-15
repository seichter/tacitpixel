#include "tp/script.h"


#include "luawrapper/luawrapper.h"

class tpScriptLua : public tpScript {
public:
    tpScriptLua();

    tpUInt init();
    tpUInt run(const tpString &script);

protected:

    ~tpScriptLua();

    lua_State* state;
};
