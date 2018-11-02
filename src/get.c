#include "functions.h"

void get_func(char *map, struct iso_dir *root, char *input)
{
    input[strlen(input) - 1] = '\0';
    char *name = input + str_cut(input, ' ');
    if (str_cut(name, ' ') != -1)
    {
        printf("my_read_iso: unable to find '%s' directory entry\n", name);
        return;
    }
    char *cur = move_to_block(map, root->data_blk.le);
    while (cur)
    {
        void *tmp = cur;
        struct iso_dir *file = tmp;
        if (!file->data_blk.le)
        {
            break;
        }
        char *filename = cur + sizeof(struct iso_dir);
        char dir = file->type & 2 ? 'd' : '-';
        if (dir == '-')
        {
            if (!strncmp(name, filename, strlen(name)))
            {
                FILE *f;
                if (!(f = fopen(name, "w")))
                {
                    return;
                }
                char *content = move_to_block(map, file->data_blk.le);
                fwrite(content, file->file_size.le, sizeof(char), f);
                fclose(f);
                return;
            }
        }
        cur += file->dir_size;
    }
    printf("my_read_iso: unable to find '%s' entry\n", name);
}