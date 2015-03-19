#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define CHUNKLEN 1024

int main()
{
	char chunk[CHUNKLEN];
	FILE * ifp;
	ifp = fopen("test.txt","r");
	FILE * ofp;
	ofp = fopen("out.txt","w");
	char dollarSign[2];
	strcpy(dollarSign,"$");
	int counter = 0,outercounter = 0;
	char c;
	while (1)
	{
		c = fgetc(ifp);
		if (c == EOF)
		{
			break;
		}
		counter++;
	}
	printf("Chars: %d\n",counter);
	outercounter = counter/CHUNKLEN;
	printf("Chunks:%d\n",outercounter);
	int remain = counter % CHUNKLEN;
	printf("remain: %d\n",remain);
	fclose(ifp);
	ifp = fopen("test.txt","r");
	counter = 0;
	int newcounter = 0;
	if (outercounter == 0)
	{
		memset(chunk,0,remain+1);
		while(1)
		{
			c = fgetc(ifp);
			if (c == EOF)
			{
				break;
			}
			chunk[counter] = c;
		}
	}
	else
	{
		memset(chunk,0,CHUNKLEN+1);
		while (1)
		{
			c = fgetc(ifp);
			if (c == EOF)
			{
				break;
			}
			printf("%c",c);
			chunk[counter] =c;
			if (((counter % CHUNKLEN) == 0) && (counter >0))
			{
				if (newcounter < outercounter)
				{
					//printf("%s",chunk);
					memset(chunk,0,CHUNKLEN+1);
					newcounter++;
				}
				else
				{
					//printf("%s",chunk);
					memset(chunk,0,remain+1);
				}
			}
			counter++;
		}
	}
	fclose(ifp);
	fclose(ofp);
	return 0;
}