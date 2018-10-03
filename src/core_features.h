#ifndef CORE_FEATURES_H
# define CORE_FEATURES_H

#include <stdio.h>
#include <string.h>
#include "iso9660.h"
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

void *checkiso(char *pathname);

#endif /* !CORE_FEATURES_H */