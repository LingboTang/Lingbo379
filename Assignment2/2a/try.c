#include <stdio.h>
#include <stdlib.h>

#define buffersize 48


int main(void)
{
    FILE *fp;
    char *line;
    size_t len = 0;
    ssize_t read;

    fp = fopen("RT_A.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
	
    while ((read = getline(&line, &len, fp)) != -1) 
	{
        //printf("Retrieved line of length %zu :\n", read);
		//int linesize = (int)read;
        //printf("%s", line);
		char *info;
		if (read != 1)
		{
			printf("%s", line);
		}
		
    }

    free(line);
    exit(EXIT_SUCCESS);
}
