#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int j;
	for (j = 0; j < argc; j++)
		printf("Argv[%d] == %s\n", j, argv[j]);

	exit(EXIT_SUCCESS);
}
