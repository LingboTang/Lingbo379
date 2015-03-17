/* This server receives an integer value encoded as a string from a client through UDP socket, and sends back the factorial of that number to the client  */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFLEN 512
#define IP 2130706433  /* 127.0.0.1 */

void writelog(ofp);

int fact (int n)
{
	if (n <= 1)
		return 1;
	else
		return n * fact (n-1);
}

int main(int argc,char** argv)
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
	int PORT = atoi(argv[1]);
	
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
	

    if (ifp == NULL)
	{
        exit(EXIT_FAILURE);
	}	

	if ( ( s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ) == -1 )
	{
		printf("Error in creating socket");
		return 1;
	}

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(IP); /* htonl(INADDR_ANY) for any interface on this machine */

	if ( bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) == -1 )
	{
		printf("Error in binding the socket");
		return 2;
	}

	printf("\n\nServer listening to %s:%d\n\n", inet_ntoa(si_me.sin_addr), ntohs(si_me.sin_port));
	while (1) 
	{
		if ( recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, (socklen_t *)&slen) != -1)
		{
			printf("\nReceived packet from %s:%d  Data: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
			i = atoi(buf);
			sprintf(buf, "%d", fact(i));
			printf("\nSending Fact(%d): %s to %s:%d\n", i, buf, inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
			sendto(s, buf, strlen(buf) + 1, 0, (struct sockaddr *)&si_other, sizeof(si_other));
		}
	}

	close(s);
 	return 0;
 }



void writelog(ofp)
{
	char* clientIP;
	int clientPORT;
	char* Filename_sent;
	int request_time;
	int transmission_time;
	char* flag_notfound = "file not found!";
	char* flag_notcompleted = "transmission not completed!";
	fprintf(ofp,"%s %d %s %d %d\n",clientIP,clientPORT,Filename_sent,request_time, transmission_time);
	fprintf(ofp,"%s %d %s %d %d\n",clientIP,clientPORT,Filename_sent,request_time, flag_notfound);
	fprintf(ofp,"%s %d %s %d %d\n\n",clientIP,clientPORT,Filename_sent,request_time, flag_notcompleted);
} 
