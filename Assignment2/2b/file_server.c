/* This server receives an integer value encoded as a string from a client through UDP socket, and sends back the factorial of that number to the client  */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFLEN 1024
#define IP 2130706433  /* 127.0.0.1 */
#define CHUNKLEN 1024
#define BUFLEN2 32

void writelog(FILE *ofp);
int get_file_size(FILE*ifp);
//void kidhandler(int signum);

static void usage()
{
	extern char * __progname;
	fprintf(stderr, "usage: %s portnumber\n", __progname);
	exit(1);
}


int main(int argc,char** argv)
{
	struct sockaddr_in si_me, si_other;
	int s, i, slen=sizeof(si_other);
	char buf[BUFLEN];
	char buf2[BUFLEN2];

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
	int filesize;
	char chunk[CHUNKLEN];

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
		filesize = get_file_size(ifp);
		//send_split_file(ifp,filesize,chunk);
		int outer_counter = filesize/CHUNKLEN;
		int remain = filesize % CHUNKLEN;
		int counter =0;
		char dollarSign[2] = "$";
		memset(chunk,0,CHUNKLEN);
		while (counter < filesize)
		{
			chunk[counter] = fgetc(ifp);
			if (((counter % CHUNKLEN) == 0) && (counter >0))
			{
				strcpy(buf,chunk);
				sendto(s, buf, strlen(buf) + 1, 0, (struct sockaddr *)&si_other, sizeof(si_other));
				strcpy(buf2,dollarSign);
				sendto(s, buf2, strlen(buf2) + 1, 0, (struct sockaddr *)&si_other, sizeof(si_other));
				memset(chunk,0,CHUNKLEN);
				memset(buf,0,CHUNKLEN);	
			}
			counter++;
		}
		if (remain > 0)
		{
			fgets(chunk,remain,ifp);
			strcpy(buf,chunk);
			sendto(s, buf, strlen(buf) + 1, 0, (struct sockaddr *)&si_other, sizeof(si_other));
		}
	}
	
	fclose(ifp);
	fclose(ofp);
	close(s);
 	return 0;
 }

int get_file_size(FILE*ifp)
{
	int counter = 0;
	while(1)
	{
		if(fgetc(ifp) == EOF)
		{
			break;
		}
		counter++;
	}
	return counter;
}

void writelog(FILE *ofp)
{
	char* clientIP;
	int clientPORT;
	char* Filename_sent;
	int request_time;
	int transmission_time;
	char* flag_notfound = "file not found!";
	char* flag_notcompleted = "transmission not completed!";
	fprintf(ofp,"%s %d %s %d %d\n",clientIP,clientPORT,Filename_sent,request_time, transmission_time);
	fprintf(ofp,"%s %d %s %d %s\n",clientIP,clientPORT,Filename_sent,request_time, flag_notfound);
	fprintf(ofp,"%s %d %s %d %s\n\n",clientIP,clientPORT,Filename_sent,request_time, flag_notcompleted);
} 

//void kidhandler(int signum) {
//	waitpid(WAIT_ANY, NULL, WNOHANG);
//}
