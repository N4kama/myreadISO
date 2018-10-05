#ifndef FUNCTIONS_H
# define FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include "iso9660.h"
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

struct file {
    struct iso_dir *iso_dir;
    char *name;
};

int str_cut(char *s, char delim);
void *move_to_block(char *map, int offset);

void *checkiso(char *pathname);
void info_func(struct iso_prim_voldesc *pv);
void ls_func(char *map, struct iso_dir *root);
void cat_func(char *map, struct iso_dir *root, char *command);

void *cd_func_alt(char *map, struct iso_dir *root, char *filename);
void *cd_func(char *map, struct iso_dir *root, char *input,
	      struct file *prev_file);

void get_func(char *map, struct iso_dir *root, char *input);

#endif /* !FUNCTIONS_H */