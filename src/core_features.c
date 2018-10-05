#include "functions.h"

void  *checkiso(char *pathname)
{
    int fd = open(pathname, O_RDONLY);
    if (fd < 0)
    {
	printf("my_read_iso: %s: No such file or directory\n", pathname);
	close(fd);
	return NULL;
    }
    struct stat st;
    if (fstat(fd, &st) || (st.st_size < (ISO_BLOCK_SIZE * 17)))
    {
	printf("my_read_iso: %s: invalid ISO9660 file\n", pathname);
	close(fd);
	return NULL;
    }
    char *map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    map += ISO_PRIM_VOLDESC_BLOCK * ISO_BLOCK_SIZE;
    void *tmp = map;
    struct iso_prim_voldesc *prim_vol = tmp;
    if (strncmp(prim_vol->std_identifier, "CD001", 5))
    {
	printf("my_read_iso: %s: invalid ISO9660 file\n", pathname);
	close(fd);
	return NULL;
    }
    close(fd);
    return map;
}

void info_func(struct iso_prim_voldesc *pv)
{
    printf("System Identifier: ");
    fflush(stdout);
    write(0, pv->syidf, ISO_SYSIDF_LEN * sizeof(char));
    printf("\n");
    printf("Volume Identifier: ");
    fflush(stdout);
    write(0, pv->vol_idf, ISO_VOLIDF_LEN * sizeof(char));
    printf("\n");
    printf("Block count:  %u\n", pv->vol_blk_count.le);
    printf("Block size: %u\n", pv->vol_blk_size.le);
    printf("Creation date: ");
    fflush(stdout);
    write(0, pv->date_creat, ISO_LDATE_LEN * sizeof(char));
    printf("\n");
    printf("Application Identifier: ");
    fflush(stdout);
    write(0, pv->app_idf, ISO_APP_LEN * sizeof(char));
    printf("\n");
}

void ls_func(char *map, struct iso_dir *root)
{
    int count = 0;
    char *cur  = move_to_block(map, root->data_blk.le);
    while (cur)
    {
        void *tmp = cur;
        struct iso_dir *file = tmp;
        if (!file->data_blk.le)
            return;
        char *name = cur + sizeof(struct iso_dir);
        char dir = file->type & 2 ? 'd' : '-';
        char hidden = file->type & 1 ? 'h' : '-';
        int name_len = file->idf_len;
        if (dir == '-')
        {
            int len = 0;
            for (; name[len] != ';'; len++);
            name_len = len;
        }
        if (count < 2)
        {
            name = !count ? "." : "..";
            name_len = !count ? 1 : 2;
        }       
        printf("%c%c %9u %04d/%02d/%02d %02d:%02d %.*s\n",
               dir, hidden, file->file_size.le, 1900 + file->date[0],
               file->date[1], file->date[2], file->date[3],
               file->date[4], name_len, name);
        cur += file->dir_size;
        count++;
    }
}

void cat_func(char *map, struct iso_dir *root, char *command)
{
    command[strlen(command) - 1] = '\0';
    char save_cmd[4095];
    strcpy(save_cmd, command);
    char *name = strtok(command, " ");
    while ((name = strtok(NULL, " ")))
    {
        char *cur  = move_to_block(map, root->data_blk.le);
        while (cur)
        {
            void *tmp = cur;
            struct iso_dir *file = tmp;
            if (!file->data_blk.le)
                break;
            char *filename = cur + sizeof(struct iso_dir);
            char dir = file->type & 2 ? 'd' : '-';
            if (dir == '-')
            {
                if (!strncmp(name, filename, strlen(name)))
                {
                    char *content = move_to_block(map, file->data_blk.le);
                    fwrite(content, file->file_size.le, sizeof(char), stdout);
                    return;
                }
            }
            cur += file->dir_size;
        }
    }
    printf("my_read_iso: unable to find '%s' entry\n",
           command + str_cut(save_cmd, ' '));
}
