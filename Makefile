CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -pedantic -std=c99

RES = my_read_iso
SRC = src/my_bash.c src/core_features.c src/tools.c src/get.c \
src/cd.c src/tree.c

all :
	$(CC) $(CFLAGS) $(SRC) -o $(RES)

test : all
	./tests/testsuite.sh ./my_read_iso ./../my_read_iso example.iso

clean :
	$(RM) $(RES)
