/* This client sends an integer value encoded as a string to an UDP server, and waits to receive the factorial of that number the Server  */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFLEN 1024
#define IP 2130706433  /* 127.0.0.1 */
#define CHUNKLEN 1024
#define BUFLEN2 32

int main( int argc, char ** argv)
{
	struct sockaddr_in si_client, si_server;
	int s, slen=sizeof(si_server);
	char buf[BUFLEN];
	char buf2[BUFLEN2];

	
	// If the user input the wrong command line
	// just exit with failure
	if ( argc != 3 )
	{
		printf("\n\nusage: %s <int value> <outputfile>\n\n", argv[0]);
		return 3;
	}
	
	// The first arguement is the port number
	int SERVERPORT = atoi(argv[1]);

	// The second arguement is the network log
	char * filename;
	FILE *ofp;
	char *mode = "w";
	filename = argv[2];
	ofp = fopen(filename, mode);	
	const char * network1 = "networkA";
	const char * network2 = "networkB";
	const char * network3 = "networkC";

	if ( ( s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ) == -1 )
	{
		printf("Error in creating socket");
		return 1;
	}

	memset((char *) &si_client, 0, sizeof(si_client));
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(SERVERPORT);
	si_server.sin_addr.s_addr = htonl(IP); 

	if ( bind(s, (struct sockaddr *)&si_client, sizeof(si_client)) == -1 )
	{
		printf("Error in binding the socket");
		return 2;
	}

	printf("\n\nClient listening to %s:%d\n\n", inet_ntoa(si_client.sin_addr), ntohs(si_client.sin_port));

	if ( recvfrom(s, buf, BUFLEN+1, 0, (struct sockaddr *)&si_server, (socklen_t *)&slen) != -1)
	{
		printf("\nReceived packet from %s:%d %s\n\n", inet_ntoa(si_server.sin_addr), ntohs(si_server.sin_port), buf);
	}
	else if ( recvfrom(s,buf2,remain+1,0,(struct sockaddr *)&si_server,(socklen_t *)&slen) != -1)
	{
		printf("\nReceived packet from %s:%d %s\n\n", inet_ntoa(si_server.sin_addr),ntohs(si_server.sin_port),buf2);
	}

	close(s);
 	return 0;
 }
