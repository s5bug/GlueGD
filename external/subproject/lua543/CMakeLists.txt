cmake_minimum_required(VERSION 3.15)

project(Lua)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(luaobj
    lapi.c
    lcode.c
    lctype.c
    ldebug.c
    ldo.c
    ldump.c
    lfunc.c
    lgc.c
    llex.c
    lmem.c
    lobject.c
    lopcodes.c
    lparser.c
    lstate.c
    lstring.c
    ltable.c
    ltm.c
    lundump.c
    lvm.c
    lzio.c
    lauxlib.c
    lbaselib.c
    lcorolib.c
    ldblib.c
    liolib.c
    lmathlib.c
    loadlib.c
    loslib.c
    lstrlib.c
    ltablib.c
    lutf8lib.c
    linit.c
)

add_library(lualib STATIC ${luaobj})
target_compile_definitions(lualib PRIVATE LUA_BUILD_AS_DLL)

add_library(lua54 SHARED ${luaobj})
target_compile_definitions(lua54 PRIVATE LUA_BUILD_AS_DLL)

add_executable(luac
    luac.c
)
target_link_libraries(luac lualib)

add_executable(lua
    lua.c
)
target_link_libraries(lua lua54)
