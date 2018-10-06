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
    {
        return -1;
    }
    return i;
}

void *move_to_block(char *map, int offset)
{
    void *tmp = map + ISO_BLOCK_SIZE * offset;
    return tmp;
}

void get_name(struct iso_dir *file, char *res)
{
    void *tmp = file;
    char *cur = tmp;
    char *name = cur + sizeof(struct iso_dir);
    char dir = file->type & 2 ? 'd' : '-';
    int name_len = file->idf_len;
    if (dir == '-')
    {
        int len = 0;
        for (; name[len] != ';'; len++);
        name_len = len;
    }
    name[name_len] = '\0';
    strcpy(res, name);
}

int is_last_file(struct iso_dir *file, char *cur)
{
    cur += file->dir_size;
    void *tmp = cur;
    struct iso_dir *next = tmp;
    if (!next->data_blk.le)
    {
        return 1;
    }
    return 0;
}