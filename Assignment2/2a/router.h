#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

struct routing
{
	char IP_addr[16];
	int prefix_length;
	char nexthop[8];
};

struct ip_pack
{
	int pack_id;
	char* SourceIP;
	char* DestinationIP;
	int TTL;
	char* payload;
};

struct statistic
{
	int Nexpired;
	int Nunroutable;
	int Ndelivered;
	int NrouterB;
	int NrouterC;
};

int decrement (int n);
char *decimal_to_binary(int n);
struct ip_pack decode_packet(char* packets);
unsigned int IPtoDec(char*IPdot);
int Ip_masking(char*ip,struct routing table);
struct statistic Make_Decision(struct ip_pack pack,struct routing* tables,struct statistic stats);