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
#include "pktgen.h"

#define BUFLEN 512
#define SERVERPORT 9930
#define PORT 9931
//#define IP 2130706433  /* 127.0.0.1 */


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

// Randomly gererating the TTL
int randomTTL() {
	int r;
    r = rand();
	r = r % (4 - 1 + 1) + 1;
	return r;
}

// Convert the IP string to decimal
unsigned int IPtoDec(char*IPdot) {
	int a,b,c,d;
	// Scan them to the int type
	sscanf(IPdot,"%d.%d.%d.%d",&a,&b,&c,&d);
	unsigned int ip;
	// Bit shifting to the correct position
	unsigned int thisa = a<<24;
	unsigned int thisb = b<<16;
	unsigned int thisc = c<<8;
	unsigned int sum = thisa+thisb+thisc+d;
	ip = sum;
	return ip;
}

int main( int argc, char ** argv)
{
	// Sockets parameters
	struct sockaddr_in si_me, si_other;
	int s, i, slen=sizeof(si_other);
	char buf[BUFLEN];

	// If the user input the wrong command line
	// just exit with failure
	if ( argc != 3 )
	{
		printf("\n\nusage: %s <int value> <outputfile>\n\n", argv[0]);
		return 3;
	}
	
	// The first arguement is the port number
	int portnum = atoi(argv[1]);

	// The second arguement is the network log
	char * filename;
	FILE *ofp;
	char *mode = "w";
	filename = argv[2];
	ofp = fopen(filename, mode);	
	const char * network1 = "networkA";
	const char * network2 = "networkB";
	const char * network3 = "networkC";
	
	

	/* Seed number for rand() */ 
	srand((unsigned int) time(0)); 
    int Packet_ID=0;
	int thisTTL;
	int countAtoB,countAtoC,countBtoA,countBtoC,countCtoA,countCtoB,countInvalid;
    while (1)
	{
		//handle_signals();

		countAtoB=0;countAtoC=0;countBtoA=0;countBtoC=0;countCtoA=0;countCtoB=0;countInvalid=0;
		struct IP_packet *Package_list;
		Package_list = malloc(sizeof(struct IP_packet) *20);
        for (int i = 0; i<20; i++)
	    {
			// Package ID
			Packet_ID++;
			
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
			else if (randi1>=0 && randi1< 2 && randi2 !=9)
			{
				if (randi2>=0 && randi2 <2)
				{
					countInvalid++;
				}
				else if (randi2 >=2 && randi2 <5)
				{
					countAtoB++;
				}
				else if (randi2 >=5 && randi2 <8)
				{
					countAtoC++;
				}
				
			}
			else if (randi1 >=2 && randi1 < 5 && randi2 != 9)
			{
				if (randi2>=0 && randi2 <2)
				{
					countBtoA++;
				}
				else if (randi2 >=2 && randi2 <5)
				{
					countInvalid++;
				}
				else if (randi2 >=5 && randi2 <8)
				{
					countBtoC++;
				}
			}
			else if (randi1 >=5 && randi1 < 9 && randi2 != 9)
			{	
				if (randi2>=0 && randi2 <2)
				{
					countCtoA++;
				}
				else if (randi2 >=2 && randi2 <5)
				{
					countCtoB++;
				}
				else if (randi2 >=5 && randi2 <8)
				{
					countInvalid++;
				}
			}
			destination_IP = networks[randi2];
			
			// TTL
			thisTTL = randomTTL();

			// Payload
	    	char s[20];    	    
	    	random_payload(s, 20);

			// Package_list infos
		    Package_list[i].packet_id = Packet_ID;
		    Package_list[i].sourceIP = source_IP;
			Package_list[i].destinationIP = destination_IP;
			Package_list[i].TTL = thisTTL;
			Package_list[i].payload = s;
		}

		// Print out the results in the network file
		fprintf(ofp, "%s to %s <%d of packets generated with source host in %s and destination host in %s>\n", network1, network2,countAtoB,network1,network2);
		fprintf(ofp, "%s to %s <%d of packets generated with source host in %s and destination host in %s>\n", network1, network3,countAtoC,network1,network3);
		fprintf(ofp, "%s to %s <%d of packets generated with source host in %s and destination host in %s>\n", network2, network1,countBtoA,network2,network1);
		fprintf(ofp, "%s to %s <%d of packets generated with source host in %s and destination host in %s>\n", network2, network3,countBtoC,network2,network3);
		fprintf(ofp, "%s to %s <%d of packets generated with source host in %s and destination host in %s>\n", network3, network1,countCtoA,network3,network1);
		fprintf(ofp, "%s to %s <%d of packets generated with source host in %s and destination host in %s>\n", network3, network2,countCtoB,network3,network2);
		fprintf(ofp, "Invalid Destination: <%d of packets generated with invalid destination>\n\n",countInvalid);
		free(Package_list);
	}
 	return 0;
 }
