#include "functions.h"

struct iso_dir_param {
    struct iso_prim_voldesc *pv;
    struct file *last_file;
    struct file *last_tmp;
    struct iso_dir *file;
};

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

void getinput5(int term, struct input_params *param)
{
    if (!strcmp(param->input, "\n"))
    {
	if (term)
	    printf(">");
	return;
    }
    else
    {
	if (param->input[strlen(param->input) - 1] == '\n')
	{
	    param->input[strlen(param->input) - 1] = '\0';
	}
	printf("my_read_iso: %s: unknown command\n", param->input);   
	if (term)
	{
	    printf(">");
	}
    }
}

void getinput4(char *map, struct iso_dir_param *dir_p, int term,
	       struct input_params *param)
{
    if (!strcmp(param->input, "pwd\n"))
    {
	for (int i = 0; i < *param->index; i++)
	{
	    printf("%s", param->path + 256*i);
	}
	printf("\n");
	if (term)
	{
	    printf(">");
	}
	return;
    }
    if (!strncmp(param->input, "get ", 4))
    {
	get_func(map, dir_p->file, param->input);
	if (term)
	{
	    printf(">");
	}
	return;
    }
    if (!strncmp(param->input, "tree", 4))
    {
	tree_func(map, dir_p->file, *param);
	if (term)
	{
	    printf(">");
	}
	return;
    }
    getinput5 (term, param);
}

void getinput3(char *map, struct iso_dir_param *dir_p, int term,
	       struct input_params *param)
{
    if (!strncmp(param->input, "cd ", 3))
    {
	if (!strncmp(param->input, "cd ..", 5))
	{
	    dir_p->file = dir_p->last_file->iso_dir;
	}
	else
	{
	    dir_p->last_file->name = dir_p->last_tmp->name;
	    dir_p->last_tmp = dir_p->last_file;
	    dir_p->last_file->iso_dir = dir_p->file;
	    dir_p->file = cd_func(map, dir_p->file, *param, dir_p->last_tmp);
	    if (dir_p->file != dir_p->last_file->iso_dir)
	    {
		void *tmp = dir_p->file;
		char *tmpp = tmp;
		char *filename = tmpp + sizeof(struct iso_dir);
		for (int i = 0; i < dir_p->file->idf_len; i++)
		{
		    param->path[256 * *param->index + i] = filename[i];
		}
		param->path[256 * *param->index + dir_p->file->idf_len] = '/';
		param->path[256 * *param->index +
			    dir_p->file->idf_len + 1] = '\0';
		*param->index += 1;
	    }
	}
	if (term)
	{
	    printf(">");
	}
	return;
    }
    getinput4(map, dir_p, term, param);
}

void getinput2(char *map, struct iso_dir_param *dir_p, int term,
	      struct input_params *param)
{
    while (fgets(param->input, 4095, stdin))
    {
        if (!strcmp(param->input, "quit\n"))
	{
            break;
        }
	if (!strcmp(param->input, "help\n"))
	{
            help_func();
	    if (term)
	    {
		printf(">");
	    }
	    continue;
	}
	if (!strcmp(param->input, "info\n"))
	{
	    info_func(dir_p->pv);
	    if (term)
	    {
		printf(">");
	    }
	    continue;
        }
	if (!strcmp(param->input, "ls\n"))
	{
	    ls_func(map, dir_p->file);
	    if (term)
	    {
		printf(">");
	    }
	    continue;
        }
	if (!strncmp(param->input, "cat ", 4))
	{
	    cat_func(map, dir_p->file, param->input);
	    if (term)
	    {
		printf(">");
	    }
	    continue;
        }
	getinput3(map, dir_p, term, param);
    }
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
    last_tmp.iso_dir = file;
    if (term)
    {	
	printf(">");
    }
    struct  iso_dir_param dir_p = {
	pv, &last_file, &last_tmp, file
    };
    getinput2(map, &dir_p, term, &param);
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
