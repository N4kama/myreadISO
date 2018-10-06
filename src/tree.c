#include "functions.h"

struct record
{
    int dir;
    int files;
};

struct treestruct
{
    int prof;
    char *name;
    int name_len;
    char dir;
};

void tree_rec_alt(struct iso_dir *file, char *cur, struct treestruct tree,
                  struct record *rec)
{
    if (tree.dir == '-')
    {
        rec->files += 1;
        int len = 0;
        for (; tree.name[len] != ';'; len++)
        {
            continue;
        }
        tree.name_len = len;
    }
    else
    {
        rec->dir += 1;
    }
    if (tree.prof || !is_last_file(file, cur))
        printf("|");
    for (int i = 0; i < tree.prof; i++)
    {
        printf("   ");
    }
    if (is_last_file(file, cur))
    {
        for (int i = 0; i < tree.prof; i++)
        {
            printf(" ");
        }
        printf("+-- %.*s/", tree.name_len, tree.name);
    }
    else
    {
        if (tree.prof && !is_last_file(file, cur))
        {
            printf("|");
        }
        printf("-- %.*s", tree.name_len, tree.name);
    }
}

void tree_rec(char *map, struct iso_dir *root, int prof, struct record *rec)
{
    int count = 0;
    char *cur  = goto_file(map, root, 3);
    struct treestruct treestruct;
    treestruct.prof = prof;
    while (cur)
    {
        void *tmp = cur;
        struct iso_dir *file = tmp;
        if (!file->data_blk.le)
        {
            return;
        }
        treestruct.name = cur + sizeof(struct iso_dir);
        treestruct.dir = file->type & 2 ? 'd' : '-';
        treestruct.name_len = file->idf_len;
        tree_rec_alt(file, cur, treestruct, rec);
        printf("\n");
        if (treestruct.dir == 'd')
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
    struct record rec =
        {
            0, 0
        };
    tree_rec(map, root, 0, &rec);
    printf("\n%d directories, %d files\n", rec.dir, rec.files);
}