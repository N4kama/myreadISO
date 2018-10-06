#include "functions.h"

struct cd_params {
    char **filename;
    char *filename_save;
    char *input_save;
    struct file *prev_file;
};

void *goto_file(char *map, struct iso_dir *root, int index)
{
    char *cur  = move_to_block(map, root->data_blk.le);
    while (cur)
    {
        index--;
        void *tmp = cur;
        struct iso_dir *file = tmp;
        if (!file->data_blk.le)
        {
            return NULL;
        }
        if (!index)
        {
            return file;
        }
        cur += file->dir_size;
    }
    return root;
}

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

int cd_ext(char *map, struct iso_dir *root, struct input_params *param,
           struct cd_params *cd_p)
{
    if (!strcmp(*cd_p->filename, ".."))
    {
        struct iso_dir *back = goto_file(map, root, 2);
        if (*param->index <= 2)
        {
            strcpy(cd_p->filename_save, "/");
        }
        else
        {
            strcpy(cd_p->filename_save,
                   &param->path[(*param->index - 2) * 256]);
            cd_p->filename_save[strlen(cd_p->filename_save) - 1] = '\0';
        }
        root = back;
        *param->index -= 1;
        return 1;
    }
    if (!strcmp(*cd_p->filename, "-"))
    {
        root = cd_p->prev_file->iso_dir;
        return 1;
    }
    if (cd_p->prev_file)
    {
        cd_p->prev_file->name = cd_p->input_save;
    }
    return 0;
}

void *cd_func(char *map, struct iso_dir *root, struct input_params param,
    struct file *prev_file)
{
    struct iso_dir *root_save = root;
    char *tmp_filename = param.input + 3;
    tmp_filename[strlen(tmp_filename) - 1] = '\0';
    char input_save[256];
    strcpy(input_save, tmp_filename);
    char *filename = NULL;
    char filename_save[256];
    struct cd_params cd_p = {
        &filename, filename_save, input_save, prev_file
    };
    while ((filename = strtok(tmp_filename, "/")))
    {
        tmp_filename = NULL;
        strcpy(filename_save, filename);
        if (cd_ext(map, root, &param, &cd_p))
            break;
        if (!(root = cd_func_alt(map, root, filename)))
        {
            printf("my_read_iso: unable to find '%s' directory entry\n"
                   , input_save);
            return root_save;
        }
    }
    if (prev_file && root == prev_file->iso_dir)
    {
        strcpy(filename_save, prev_file->name);
    }
    if (prev_file)
    {
        printf("Changing to '%s' directory\n", filename_save);
    }
    return root;
}