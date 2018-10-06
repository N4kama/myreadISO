#include "functions.h"

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

int getinput(char *map, struct iso_prim_voldesc *pv)
{
    char input[4095];
    int term = isatty(0);
    struct iso_dir *file = &pv->root_dir;
    struct file last_file;
    struct file last_tmp;

    char pwd[256*9];
    pwd[0] = '/';
    pwd[1] = '\0'; 
    int pwd_index = 1;

    struct input_params param = { input, pwd , &pwd_index};
    
    last_file.iso_dir = file;
    if (term)
    {	
	printf(">");
    }
    while (fgets(param.input, 4095, stdin))
    {
        if (!strcmp(param.input, "quit\n"))
	{
            break;
        }
	if (!strcmp(param.input, "help\n"))
	{
            help_func();
	    if (term)
	    {
		printf(">");
	    }
	    continue;
	}
	if (!strcmp(param.input, "info\n"))
	{
	    info_func(pv);
	    if (term)
	    {
		printf(">");
	    }
	    continue;
        }
	if (!strcmp(param.input, "ls\n"))
	{
	    ls_func(map, file);
	    if (term)
	    {
		printf(">");
	    }
	    continue;
        }
	if (!strncmp(param.input, "cat ", 4))
	{
	    cat_func(map, file, param.input);
	    if (term)
	    {
		printf(">");
	    }
	    continue;
        }
	if (!strncmp(param.input, "cd ", 3))
        {
            last_file.name = last_tmp.name;
            last_tmp = last_file;
            last_file.iso_dir = file;
            file = cd_func(map, file, param, &last_tmp);
            if (file != last_file.iso_dir)
            {
	            void *tmp = file;
		    char *tmpp = tmp;
		    char *filename = tmpp + sizeof(struct iso_dir);
		    for (int i = 0; i < file->idf_len; i++)
		    {
			pwd[256 * pwd_index + i] = filename[i];
		    }
		    pwd[256 * pwd_index + file->idf_len] = '/';
		    pwd[256 * pwd_index + file->idf_len + 1] = '\0';
		    pwd_index += 1;
            }
	    if (term)
	    {
		printf(">");
	    }
	    continue;
        }
        if (!strcmp(param.input, "pwd\n"))
        {
            for (int i = 0; i < pwd_index; i++)
            {
                printf("%s", pwd + 256*i);
            }
            printf("\n");
	    if (term)
	    {
		printf(">");
	    }
	    continue;
        }
        if (!strncmp(param.input, "get ", 4))
        {
            get_func(map, file, param.input);
	    if (term)
	    {
		printf(">");
	    }
	    continue;
        }
        if (!strncmp(param.input, "tree", 4))
	{
	    tree_func(map, file, param);
	    if (term)
	    {
		printf(">");
	    }
	    continue;
	}
        if (!strcmp(param.input, "\n"))
        {
            if (term)
                printf(">");
            continue;
        }
        else
        {
            if (param.input[strlen(input) - 1] == '\n')
            {
		param.input[strlen(input) - 1] = '\0';
            }
	    printf("my_read_iso: %s: unknown command\n", param.input);   
	    if (term)
	    {
		printf(">");
	    }
	}
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        errx(1, "usage: ./my_read_iso FILE");
    }

    char *map = checkiso(argv[1]);
    if (!map)
    {
        return 1;
    }
    
    void *tmp = map;
    struct iso_prim_voldesc *pv = tmp;
    map -= ISO_PRIM_VOLDESC_BLOCK * ISO_BLOCK_SIZE;
    getinput(map, pv);
    return 0;
}
