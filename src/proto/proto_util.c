#include <system.h>

extern int randnum(int limit)
{
    return (rand() & 0x7fffffff) % limit;
}

extern int initLua()
{
    L1 = lua_open();
    lua_baselibopen(L1);

    return 0;
}

extern int execLua(char *lua)
{
    lua_dofile(L1, lua);

    return 0;
}

extern void closeLua()
{
    lua_close(L1);
    lua_close(L2);
}
