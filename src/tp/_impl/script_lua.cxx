#include "script_lua.h"

tpScriptLua::tpScriptLua()
	: state(luaL_newstate())
{
}

tpUInt tpScriptLua::init()
{
	luaopen_base(state);
	luaopen_bit32(state);
	luaopen_coroutine(state);
	luaopen_debug(state);
	luaopen_io(state);
	luaopen_package(state);
	luaopen_string(state);
	luaopen_table(state);
	luaopen_math(state);

	return 0;
}

tpUInt tpScriptLua::run(const tpString &script)
{
	luaL_loadstring(state, script.c_str());
	lua_pcall(state, 0, LUA_MULTRET, 0);
	return 0;
}

tpScriptLua::~tpScriptLua()
{
	lua_close(state);
}
