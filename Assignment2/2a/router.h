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
	char* IP_addr;
	int prefix_length;
	char* nexthop;
};

struct ip_pack
{
	int pack_id;
	char* SourceIP;
	char* DestinationIP;
	int TTL;
	char* payload;
};

int fact (int n);
int decrement (int n);
char *decimal_to_binary(int n);
struct ip_pack decode_packet(char* packets);