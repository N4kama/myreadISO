#include <stdio.h>
#include <string.h>
#include <unistd.h>

int getinput(void)
{
    char input[4095];
    int term = isatty(0);
    if (term)
	printf(">");
    while (fgets(input, 4095, stdin))
    {
	printf("%s", input);
	if (term)
	    printf(">");
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
	printf("usage: ./myreadiso FILE\n");
	return 1;
    }
    argv[1] = 0;
    //checkisofilefunction (open and check)
    //Leave if bad iso, continue if good iso

    getinput();
    return 0;
}
