/* This server receives an integer value encoded as a string from a client through UDP socket, and sends back the factorial of that number to the client  */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512
#define maxaline 64
#define initial 10
#define PORT 9930
#define IP 2130706433  /* 127.0.0.1 */

int fact (int n)
{
	if (n <= 1)
		return 1;
	else
		return n * fact (n-1);
}

int main(int argc, char**argv)
{
	struct sockaddr_in si_me, si_other;
	int s, i, slen=sizeof(si_other);
	char buf[BUFLEN];


	// If the user input the wrong command line
	// just exit with failure
	if ( argc != 4 )
	{
		printf("\n\nusage: %s <int value>\n\n", argv[0]);
		exit(1);
	}

	// The first argument is the port number 
	int portnumber = atoi(argv[1]);
	
	// The second argument is the routing table
	char * filename1;
	FILE *ifp;
	char *mode1 = "r";
	filename1 = argv[2];
	ifp = fopen(filename1, mode1);

	// The third argument is the statistic file name
	char * filename2;
	FILE *ofp;
	char *mode2 = "w";
	filename2 = argv[3];
	ofp = fopen(filename2, mode2);
		
	char line[maxaline+1];
	int j=0;
	char* filetext;
	int onthego = initial;
	filetext = malloc(onthego*sizeof(line));
	memset(line, 0, maxaline + 1);
	while((fgets(line,maxaline+1,ifp))!=NULL)
	{
		strncpy(&filetext[j],line,maxaline+1);
		j += maxaline+1;
		if (j == onthego)
		{
			onthego *= 2;
			filetext = realloc(filetext,onthego*sizeof(line));
		}
		memset(line, 0, maxaline + 1);
	}
	for (j = j-maxaline-1; j >=0; j -= maxaline+1)
	{
		fprintf(stdout,"%s",&filetext[j-maxaline-1]);
	}	
	free(filetext);
 	return 0;
 }
