#include "functions.h"

int str_cut(char *s, char delim)
{
    int i = 0;
    for (; s[i] && s[i] != delim; i++)
    {
        continue;
    }
    for (; s[i] && s[i] == delim; i++)
    {
        continue;
    }
    int len = strlen(s);
    if (i == len)
        return -1;
    return i;
}

void *move_to_block(char *map, int offset)
{
    void *tmp = map + ISO_BLOCK_SIZE * offset;
    return tmp;
}