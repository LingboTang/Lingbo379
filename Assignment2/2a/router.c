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
#include <math.h>
#include "pktgen.h"
#include "router.h"

#define BUFLEN 512
#define PORT 9930
#define IP 2130706433  /* 127.0.0.1 */

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
	//int portnumber = atoi(argv[1]);
	
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
	
	char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if (ifp == NULL)
	{
        exit(EXIT_FAILURE);
	}
	
	struct routing *r_table;
	r_table = malloc(sizeof(routing*)*3);
	int index = 0;
    while ((read = getline(&line, &len, ifp)) != -1) {
		if (line[0] != '\n')
		{
			char IP_ad[16];
			char router[8];
			int length;
			sscanf(line,"%s %d %s",IP_ad,&length,router);
			//printf("%s\n",IP_ad);
			//printf("%d\n",length);
			//printf("%s\n",router);
			r_table[index].IP_addr = IP_ad;
			r_table[index].prefix_length = length;
			r_table[index].nexthop = router;
			index++;
		}

    }
    free(line);
	
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

	strcpy(buf, argv[1]);
	printf("\n\nServer listening to %s:%d\n\n", inet_ntoa(si_me.sin_addr), ntohs(si_me.sin_port));
	while (1) 
	{
		int i ;
		struct statistic;
		for (i =0; i<20; i++)
		{
			if ( recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen) != -1)
			{
				printf("\nReceived packet from %s:%d  Data: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
				struct ip_pack tmpdecode;
				tmpdecode = decode_packet(buf);
				if (decrement(tmpdecode.TTL) == 0)
				{
					printf("This is expired\n");
					statistic.Nexpired++;
				}
			}
		}
	}

	printf("%s\n",buf);

	close(s);
 	return 0;
 }

int fact (int n)
{
	if (n <= 1)
		return 1;
	else
		return n * fact (n-1);
}

int decrement (int n)
{
	return n-1;
}

// Code from http://www.programmingsimplified.com/c/source-code/c-program-convert-decimal-to-binary

char *decimal_to_binary(int n)
{
   int c, d, count;
   char *pointer;
 
   count = 0;
   pointer = (char*)malloc(32+1);
 
   if ( pointer == NULL )
      exit(EXIT_FAILURE);
 
   for ( c = 31 ; c >= 0 ; c-- )
   {
      d = n >> c;
 
      if ( d & 1 )
         *(pointer+count) = 1 + '0';
      else
         *(pointer+count) = 0 + '0';
 
      count++;
   }
   *(pointer+count) = '\0';
 
   return  pointer;
}

struct ip_pack decode_packet(char* packets)
{
	int PACKET_ID;
	char* source_IP;
	char* destination_IP;
	int myTTL;
	char* mypayload;
	struct ip_pack decode_list;
	sscanf(packets,"%d %s %s %d %s",&PACKET_ID,source_IP,destination_IP,&myTTL,mypayload);
	decode_list.pack_id = PACKET_ID;
	decode_list.SourceIP = source_IP;
	decode_list.DestinationIP = destination_IP;
	decode_list.TTL = myTTL;
	decode_list.payload = mypayload;
	return decode_list;
}

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

void Making_Decision(char*ip,struct routing table)
{
	
}