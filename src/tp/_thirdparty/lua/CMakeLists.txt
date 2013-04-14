
set(luasrcdir src)

include_directories(include)

set(lua_srcs
    ${luasrcdir}/lapi.c
    ${luasrcdir}/lcode.c
    ${luasrcdir}/lctype.c
    ${luasrcdir}/ldebug.c
    ${luasrcdir}/ldo.c
    ${luasrcdir}/ldump.c
    ${luasrcdir}/lfunc.c
    ${luasrcdir}/lgc.c
    ${luasrcdir}/llex.c
    ${luasrcdir}/lmem.c
    ${luasrcdir}/lobject.c
    ${luasrcdir}/lopcodes.c
    ${luasrcdir}/lparser.c
    ${luasrcdir}/lstate.c
    ${luasrcdir}/lstring.c
    ${luasrcdir}/ltable.c
    ${luasrcdir}/ltm.c
    ${luasrcdir}/lundump.c
    ${luasrcdir}/lvm.c
    ${luasrcdir}/lzio.c
    )

list(APPEND lua_srcs
    ${luasrcdir}/lauxlib.c
    ${luasrcdir}/lbaselib.c
    ${luasrcdir}/lbitlib.c
    ${luasrcdir}/lcorolib.c
    ${luasrcdir}/ldblib.c
    ${luasrcdir}/liolib.c
    ${luasrcdir}/lmathlib.c
    ${luasrcdir}/loslib.c
    ${luasrcdir}/lstrlib.c
    ${luasrcdir}/ltablib.c
    ${luasrcdir}/loadlib.c
    ${luasrcdir}/linit.c
    )




add_library(lua STATIC ${lua_srcs})
