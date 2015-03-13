#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUFLEN 512

int main(int argc, char**argv)
{
	char buf[BUFLEN];
	if ( argc != 3 )
	{
		printf("\n\nusage: %s <int value>\n\n", argv[0]);
		exit(1);
	}
	
	char * filename;
	FILE *ofp;
	char *mode = "w";
	filename = argv[2];
	ofp = fopen(filename, mode);
	char *outputstream = "\nThis works!\n";
	int portnum =atoi(argv[1]);
	fprintf(ofp, "%s %d\n", outputstream,portnum);
	fclose(ofp);
	return 0;
}
