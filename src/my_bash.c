#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "core_features.h"

void help_func(void)
{
    printf("help: display command help\n");
    printf("info: display volume info\n");
    printf("ls: display the content of a directory\n");
    printf("cd: change current directory\n");
    printf("tree: display the tree of a directory\n");
    printf("get: copy file to local directory\n");
    printf("cat: display file content\n");
    printf("pwd: print current path\n");
    printf("quit: program exit\n");
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

void *move_to_block(char *map, int offset)
{
    void *tmp = map + ISO_BLOCK_SIZE * offset;
    return tmp;
}

void ls_func(char *map, struct iso_prim_voldesc *pv)
{
    struct iso_dir *root = &pv->root_dir;
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
	printf("%c%c %9u %04d/%02d/%02d %02d:%02d %s\n",
	       dir, hidden, file->file_size.le, file->date[0], file->date[1],
	       file->date[2], file->date[3], file->date[4], name);
	cur += file->dir_size;
    }
}

int getinput(char *map, struct iso_prim_voldesc *pv)
{
    char input[4095];
    int term = isatty(0);
    if (term)
	printf(">");
    while (fgets(input, 4095, stdin))
    {
	if (!strcmp(input, "quit\n"))
	    break;
	else if (!strcmp(input, "help\n"))
	    help_func();
	else if (!strcmp(input, "info\n"))
	    info_func(pv);
	else if (!strcmp(input, "ls\n"))
	    ls_func(map, pv);
	else if (!strcmp(input, "\n"))
	{
	    if (term)
		printf(">");
	    continue;
	}
	else
	    printf("my_read_iso: %s: unknown command", input);
	if (term)
	    printf(">");
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
	printf("usage: ./my_read_iso FILE\n");
	return 1;
    }

    char *map = checkiso(argv[1]);
    if (!map)
	return 1;

    void *tmp = map;
    struct iso_prim_voldesc *pv = tmp;
    map -= ISO_PRIM_VOLDESC_BLOCK * ISO_BLOCK_SIZE;
    getinput(map, pv);
    return 0;
}
