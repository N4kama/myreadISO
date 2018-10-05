#include "functions.h"

void *cd_func_alt(char *map, struct iso_dir *root, char *filename)
{
    char *cur  = move_to_block(map, root->data_blk.le);
    while (cur)
    {
        void *tmp = cur;
        struct iso_dir *file = tmp;
        if (!file->data_blk.le)
        {
            return NULL;
        }
        char *name = cur + sizeof(struct iso_dir);
        char dir = file->type & 2 ? 'd' : '-';
        int name_len = file->idf_len;
        if (dir == 'd')
        {
            if (!strncmp(name, filename, name_len))
            {
                return file;
            }
        }
        cur += file->dir_size;
    }
    printf("Changing to '%s' directory\n", filename);
    return root;
}

void *cd_func(char *map, struct iso_dir *root, char *input,
    struct file *prev_file)
{
    struct iso_dir *root_save = root;
    char *tmp_filename = input + 3;
    tmp_filename[strlen(tmp_filename) - 1] = '\0';
    char input_save[256];
    strcpy(input_save, tmp_filename);
    char *filename = NULL;
    char *filename_save = NULL;
    while ((filename = strtok(tmp_filename, "/")))
    {
        tmp_filename = NULL;
        filename_save = filename;
        if (!strcmp(filename, "-"))
        {
            root = prev_file->iso_dir;
            break;
        }
        prev_file->name = input_save;
        if (!(root = cd_func_alt(map, root, filename)))
        {
            printf("my_read_iso: unable to find '%s' directory entry\n"
                   , input_save);
            return root_save;
        }  
    }
    if (root == prev_file->iso_dir)
        filename_save = prev_file->name;
    printf("Changing to '%s' directory\n", filename_save);
    return root;
}