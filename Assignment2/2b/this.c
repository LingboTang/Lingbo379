#include <sys/types.h>
#include <sys/stat.h>

#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <netinet/in.h>

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <arpa/inet.h>


#define CHUNKLEN 1024

int main()
{
	char *filename = "test2.txt";
	FILE *ifp;
	ifp = fopen(filename,"r");
	char chunk[CHUNKLEN];

	while(!feof(ifp)){
		memset(chunk,0,CHUNKLEN+1);
		int c;
		int index;
		for(index=0;index<1024;index++){
			c=fgetc(ifp);
			if(c!=EOF){
				strcat(chunk,(char*)&c);
			}
			if(feof(ifp)){
				if(index==0){
					strcat(chunk,"$");
				}
				break;
			}
		}
		printf("\n\n=========%d================\n\n",strlen(chunk));
		printf("%s",chunk);
	}
	return 0;
}
