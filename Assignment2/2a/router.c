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
#define IP 2130706433  /* 127.0.0.1 */

int main(int argc, char**argv)
{
	struct sockaddr_in si_me, si_other;
	int s, slen=sizeof(si_other);
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
	
	struct routing* r_table;
	r_table = malloc(sizeof(struct routing)*3);
	int index = 0;
	char line[BUFLEN];
	while(fgets(line,sizeof(line), ifp)) {
		if(line[0]=='\n' || line[0]=='\r' || line[0]=='\0')
		{
			continue;
		}
		else
		{
			sscanf(line,"%s %d %s",r_table[index].IP_addr,&r_table[index].prefix_length,r_table[index].nexthop);
			index++;
		}
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

	strcpy(buf, argv[1]);
	printf("\n\nServer listening to %s:%d\n\n", inet_ntoa(si_me.sin_addr), ntohs(si_me.sin_port));
	//(void) signal(SIGINT, sig_handler_2);
	while (1) 
	{

		int i ;
		struct statistic stat;
		stat.Nexpired = 0; stat.Nunroutable = 0; stat.Ndelivered = 0;
		stat.NrouterB = 0; stat.NrouterC = 0;
		for (i =0; i<20; i++)
		{
			(void) signal(SIGINT, sig_handler_2);
			if ( recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, (socklen_t *)&slen) != -1)
			{
				printf("\nReceived packet from %s:%d  Data: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
				struct ip_pack tmpdecode;
				tmpdecode = decode_packet(buf);
				stat = Make_Decision(tmpdecode,r_table,stat);
			}
		}

		// Print the out put.
		printf("\n\nOut of the loop\n\n");
		fprintf(ofp,"expired packets: %d of packets expired\n",stat.Nexpired);
		fprintf(ofp,"unroutable packets: %d of packets containing invalid detination\n",stat.Nunroutable);
		fprintf(ofp,"delivered direct: %d of packets expired\n",stat.Ndelivered);
		fprintf(ofp,"routerB: %d of packets forward to routerB\n",stat.NrouterB);
		fprintf(ofp,"routerC: %d of packets forward to routerC\n\n",stat.NrouterC);
		fflush(ofp);
	}
	free(r_table);
	fclose(ofp);
	close(s);
 	return 0;
 }

int decrement (int n)
{
	return n-1;
}

// Code from http://www.programmingsimplified.com/c/source-code/c-program-convert-decimal-to-binary

struct ip_pack decode_packet(char* packets)
{
	int PACKET_ID;
	char source_IP[16];
	char destination_IP[16];
	int myTTL;
	char mypayload[21];
	struct ip_pack decode_list;
	sscanf(packets,"%d %s %s %d %s",&PACKET_ID,source_IP,destination_IP,&myTTL,mypayload);
	decode_list.pack_id = PACKET_ID;
	decode_list.SourceIP = source_IP;
	decode_list.DestinationIP = destination_IP;
	decode_list.TTL = myTTL;
	decode_list.payload = mypayload;
	return decode_list;
}


/*
 * Transform the IP string to binary
 * scan the string, and shift the binary bits to the
 * correct position.
 */
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

/*
 * Extract the <prefix_length> number of bits
 * masking them and compare them, if equal, return 
 * true, otherwise false.
 */

int Ip_masking(char*ip,struct routing table)
{
	int cmpsize = table.prefix_length;
	int dec_t_ip = IPtoDec(table.IP_addr);
	int dec_p_ip = IPtoDec(ip);
	int shift = 31-cmpsize;
	int t_cmp = dec_t_ip>>shift;
	int p_cmp = dec_p_ip>>shift;
	if (p_cmp == t_cmp)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/*
 * This funciton is to figure out if a package is expired
 * unroutable, or sending to some specific routers.
 * Once we pass in the recieved and decoded messages and routing table
 * We can figure out this by scanning each line of the routing table.
 * We will use the prefix_length in each line as comparing length to
 * mask the package destination IP address. If it not matching just
 * scan the next line until we find the match or reach to the end of the
 * table. Since the order of the table is from rB->rA->rC, then table[0]
 * is the router B,table[1] is the router A, table[2] is the router C.
 * After we get those, just upgrade the counter, and store them to the stat structure.
 */

struct statistic Make_Decision(struct ip_pack pack,struct routing* tables,struct statistic stats)
{
	int ri = 0;
	if (decrement(pack.TTL) == 0)
	{
		printf("This is expired\n");
		stats.Nexpired++;
	}
	else
	{
		while ((Ip_masking(pack.DestinationIP,tables[ri]) == 0) && ri < 3)
		{
			ri++;
			Ip_masking(pack.DestinationIP,tables[ri]);
		}
		if (ri == 0)
		{
			stats.NrouterB++;
		}
		else if(ri == 1)
		{
			printf("packet_ID = %d, dest = %s\n",pack.pack_id,pack.DestinationIP);
			stats.Ndelivered++;
		}
		else if (ri == 2)
		{
			stats.NrouterC++;
		}
		else if (ri == 3)
		{
			stats.Nunroutable++;
			printf("\n\nyes\n\n");
		}
	}
	return stats;
} 

// sig handler
void sig_handler_2(int sig)
{
	stop_flag = 0;
	exit(0);
}