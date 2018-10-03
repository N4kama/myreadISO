#include "core_features.h"

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