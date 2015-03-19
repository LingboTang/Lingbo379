#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define CHUNKLEN 1024

int main()
{
	char chunk[CHUNKLEN];
	FILE * ifp;
	ifp = fopen("test2.txt","r");
	FILE * ofp;
	ofp = fopen("out.txt","w");
	char dollarSign[2];
	strcpy(dollarSign,"$");
	//memset(chunk,0,CHUNKLEN);
	int counter = 0,outercounter = 0;
	char c;
	while (1)
	{
		c = fgetc(ifp);
		if (c == EOF)
		{
			break;
		}
		//printf("%c",c);
		
		//if (((counter % CHUNKLEN) == 1) && counter >1)
		//{
		//	fgets(chunk,CHUNKLEN,ifp);
			//printf("%d\n",counter);
		//	printf("%s",chunk);
		//}
		counter++;
	}
	outercounter = counter/CHUNKLEN;
	printf("Chunks:%d\n",outercounter);
	int remain = counter % CHUNKLEN;
	printf("remain: %d\n",remain); 
	fclose(ifp);
	ifp = fopen("test2.txt","r");
	//while (fgets(chunk,1024,ifp))
	//{
		//printf("%s",chunk);
		//fprintf(ofp,"%s",chunk);
		//fprintf(ofp,"%s",dollarSign);
		//fflush(ofp);
	//}
	//printf("%d\n",counter);
	//printf("%d\n",outercounter);
	int i=0;
	while (i<outercounter)
	{
		memset(chunk,0,CHUNKLEN+1);
		strcpy(chunk,fgets(chunk,CHUNKLEN,ifp));
		printf("%s\n",chunk);
		printf("%d\n",i);
		i++;
	}
	if (remain !=0)
	{
		memset(chunk,0,remain+1);
		strcpy(chunk,fgets(chunk,remain,ifp));
		printf("%s\n",chunk);
	}
	fclose(ifp);
	fclose(ofp);
	return 0;
}
