#include "tp/script.h"

#include "lua/src/lua.h"

#include "_impl/script_lua.h"


tpScript *tpScript::create(tpUInt scriptType)
{
    switch (scriptType) {
    case tpScript::kLua :
        return new tpScriptLua();
        break;

    }
    return 0L;
}

tpScript::tpScript()
{
}

tpScript::~tpScript()
{
}

tpScript::tpScript(const tpScript &s)
{
}

tpScript &tpScript::operator =(const tpScript &s)
{
	return (*this);
}



