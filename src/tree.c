#include "functions.h"

struct record {
    int dir;
    int files;
};

void tree_rec(char *map, struct iso_dir *root, int prof, struct record *rec)
{
    int count = 0;
    char *cur  = goto_file(map, root, 3);
    while (cur)
    {
        void *tmp = cur;
        struct iso_dir *file = tmp;
        if (!file->data_blk.le)
	{
            return;
	}
	char *name = cur + sizeof(struct iso_dir);
        char dir = file->type & 2 ? 'd' : '-';
        int name_len = file->idf_len;
        if (dir == '-')
        {
	    rec->files += 1;
            int len = 0;
            for (; name[len] != ';'; len++)
	    {
		continue;
	    }
            name_len = len;
        }
	else
	{
	    rec->dir += 1;
	}
	if (prof && !is_last_file(file, cur))
	{
	    printf("|");
	}
	for (int i = 0; i < prof; i++)
	{
	    printf("   ");
	}
	if (is_last_file(file, cur))
	{
	    printf("+-- %.*s/", name_len, name);
	}
	else
	{
	    printf("|-- %.*s", name_len, name);
	}
	printf("\n");
	if (dir == 'd')
	{
	    tree_rec(map, file, prof + 1, rec);
        }
	cur += file->dir_size;
        count++;
    }
}

void tree_func(char *map, struct iso_dir *root, struct input_params param)
{
    char *tmp_filename = param.input + 4;
    int len = strlen(tmp_filename);
    tmp_filename[len - 1] = '\0';
    char *cmd = strtok(tmp_filename, " ");
    if (!cmd)
    {
	printf(".\n");
    }
    else
    {
	printf("%s\n", cmd);
	char new[4095];
	strcpy(new, "cp ");
	strcpy(new + 3, cmd);
	new[len + 1] = '\n'; 
	param.input = new;
	root = cd_func(map, root, param, NULL);
    }
    struct record rec = { 0, 0};
    tree_rec(map, root, 0, &rec);
    printf("\n%d directories, %d files\n", rec.dir, rec.files);
}