#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <string.h>
#include "iso9660.h"
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <err.h>

struct file
{
    struct iso_dir *iso_dir;
    char *name;
};

struct input_params
{
    char *input;
    char *path;
    int *index;
};

int str_cut(char *s, char delim);
void *move_to_block(char *map, int offset);
void get_name(struct iso_dir *file, char *res);
int is_last_file(struct iso_dir *file, char *cur);

void *checkiso(char *pathname);
void info_func(struct iso_prim_voldesc *pv);
void ls_func(char *map, struct iso_dir *root);
void cat_func(char *map, struct iso_dir *root, char *command);

void *goto_file(char *map, struct iso_dir *root, int index);
void *cd_func(char *map, struct iso_dir *root, struct input_params param,
              struct file *prev_file);

void get_func(char *map, struct iso_dir *root, char *input);

void tree_func(char *map, struct iso_dir *root, struct input_params param);

#endif /* !FUNCTIONS_H */