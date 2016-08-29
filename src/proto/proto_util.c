#include <system.h>

extern int randnum(int limit)
{
    return (rand() & 0x7fffffff) % limit;
}
