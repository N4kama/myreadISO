#include "functions.h"

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
    while ((filename = strtok(tmp_filename, "/")))
    {
        tmp_filename = NULL;
        strcpy(filename_save, filename);
	if (!strcmp(filename, ".."))
	{
	    struct iso_dir *back = goto_file(map, root, 2);
	    if (*param.index <= 2)
	    {
		strcpy(filename_save, "/");
	    }
	    else
	    {
		strcpy(filename_save, &param.path[(*param.index - 2) * 256]);
		filename_save[strlen(filename_save) - 1] = '\0';
	    }
	    root = back;
	    *param.index -= 1;
	    break;
	}
        if (!strcmp(filename, "-"))
        {
            root = prev_file->iso_dir;
            break;
        }
	if (prev_file)
	{
	    prev_file->name = input_save;
	}
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