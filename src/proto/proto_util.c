#include <system.h>

extern int randnum(int limit)
{
    return (rand() & 0x7fffffff) % limit;
}

extern int grabScreen(char *shots)
{
    if (!vid_screen_shot(shots))
    {
        dbglog(DBG_DEBUG, "* Failed to Capture Screen!\n");
        return 1;
    }
    return 0;
}
