/* This client sends an integer value encoded as a string to an UDP server, and waits to receive the factorial of that number the Server  */

#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include "pktgen.h"

#define BUFLEN 512
#define IP 2130706433  /* 127.0.0.1 */

int main( int argc, char ** argv)
{
	// Sockets parameters
	struct sockaddr_in si_other;
	int s;
	char buf[BUFLEN];

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
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(SERVERPORT);
	si_other.sin_addr.s_addr = htonl(IP);

	/* Seed number for rand() */ 
	srand((unsigned int) time(0)); 
    int Packet_ID=0;
	int thisTTL;
	int sum;
	int countAtoB,countAtoC,countBtoA,countBtoC,countCtoA,countCtoB,countInvalid;
    (void) signal(SIGINT, sig_handler);
    while (1)
	{
		sum = 0;
		countAtoB=0;countAtoC=0;countBtoA=0;countBtoC=0;countCtoA=0;countCtoB=0;countInvalid=0;
		int i;
		
        for (i = 0; i<20; i++)
	    {
	    	char *package = malloc(sizeof(char*)*20);

			// Package ID
			Packet_ID++;
			if (Packet_ID%2 == 0)
			{
				sleep(1);
			}
			char buffer[32];
			char* packet_ID = itoa(Packet_ID,buffer);
			
			// Source IP and Destination IP
			char* source_IP;
			int randi1 = rand() % (8-0 + 1) + 0;
			int randi2 = rand() % (9-0 + 1) + 1;
			
			source_IP = networks[randi1];
			
			// Using index to specify the source network
			// and the destination network
			// If it is invalid IP or end up with the same source and 
			// destination just set them as invalid
			// otherwise index(0-1) is from networkA, (2-4) is from networkB, (5-8) is from networkC
			char* destination_IP;
			if (randi2 == 9)
			{
				countInvalid++;
			}
			else if (randi1>=0 && randi1< 2 && randi2 <9)
			{
				while (randi2>=0 && randi2 <2)
				{
					randi2 = rand() % 8;
				}
				//printf("%d\n",randi2);
				if (randi2 >=2 && randi2 <5)
				{
					countAtoB++;
				}
				else if (randi2 >=5 && randi2 <9)
				{
					countAtoC++;
				}
				
			}
			else if (randi1 >=2 && randi1 < 5 && randi2 < 9)
			{
				while (randi2 >=2 && randi2 <5)
				{
					randi2 = rand() % 8;
				}
				//printf("%d\n",randi2);
				if (randi2>=0 && randi2 <2)
				{
					countBtoA++;
				}
				else if (randi2 >=5 && randi2 <9)
				{
					countBtoC++;
				}
			}
			else if (randi1 >=5 && randi1 < 9 && randi2 < 9)
			{	
				while (randi2 >=5 && randi2 <9)
				{
					randi2 = rand() % 8;
				}
				//printf("%d\n",randi2);
				if (randi2>=0 && randi2 <2)
				{
					countCtoA++;
				}
				else if (randi2 >=2 && randi2 <5)
				{
					countCtoB++;
				}
			}
			destination_IP = networks[randi2];
			
			// TTL
			char *pktTTL;
			thisTTL = rand() % (3 - 0 + 1) + 0;
			pktTTL = TTL[thisTTL];

			// Payload
	    	char payload[20];    	    
	    	random_payload(payload, 20);

			// Package_list info
			char *space = " ";
			strcpy(package,packet_ID);
			strcat(package,space);
			strcat(package,source_IP);
			strcat(package,space);
			strcat(package,destination_IP);
			strcat(package,space);
			strcat(package,pktTTL);
			strcat(package,space);
			strcat(package,payload);
			strcpy(buf, package);
			printf("\nSending %s to %s:%d\n", buf, inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
			sendto(s, buf, strlen(buf) + 1, 0, (struct sockaddr *)&si_other, sizeof(si_other));
			free(package);
		}
		printf("\n\nOut of the loop\n\n");
		sum = countAtoB+countAtoC+countBtoA+countBtoC+countCtoA+countCtoB+countInvalid;
		// Print out the results in the network file
		fprintf(ofp,"%d\n",sum);
		fprintf(ofp, "%s to %s <%d of packets generated with source host in %s and destination host in %s>\n", network1, network2,countAtoB,network1,network2);
		fprintf(ofp, "%s to %s <%d of packets generated with source host in %s and destination host in %s>\n", network1, network3,countAtoC,network1,network3);
		fprintf(ofp, "%s to %s <%d of packets generated with source host in %s and destination host in %s>\n", network2, network1,countBtoA,network2,network1);
		fprintf(ofp, "%s to %s <%d of packets generated with source host in %s and destination host in %s>\n", network2, network3,countBtoC,network2,network3);
		fprintf(ofp, "%s to %s <%d of packets generated with source host in %s and destination host in %s>\n", network3, network1,countCtoA,network3,network1);
		fprintf(ofp, "%s to %s <%d of packets generated with source host in %s and destination host in %s>\n", network3, network2,countCtoB,network3,network2);
		fprintf(ofp, "Invalid Destination: <%d of packets generated with invalid destination>\n\n",countInvalid);
		fflush(ofp);
	}
	close(s);
 	return 0;
 }

 /* Random generator Referrence
 * http://ubuntuforums.org/showthread.php?t=1979310 
 * author: Buffay
 * Edit date: March 13th, 2012
 */

void random_payload(char * string, size_t length)
{
    /* ASCII characters 33 to 126 */
    unsigned int num_chars = length - 1;
    unsigned int i;
	
	/* Randomly choosing the characters from the
	 * Lower bound to the Upper bound.
	 */
    for (i = 0; i < num_chars; ++i)
    {
        string[i] = rand() % (126 - 33 + 1) + 33;
    }
 
    string[num_chars] = '\0';  
}

/*
 * Referrence of itoa function
 * http://stackoverflow.com/questions/9655202/how-to-convert-integer-to-string-in-c
 * author: bhuwansahni
 * Edit date: March 12,2012
 */
 
char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

// sig handler
void sig_handler(int sig)
{
	stop_flag = 0;
}
