#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("RT_A.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
		if (line[0] != '\n')
		{
			char IP[16];
			char router[8];
			int length;
			sscanf(line,"%s %d %s",IP,&length,router);
		}	
    }

    free(line);
    exit(EXIT_SUCCESS);
}
