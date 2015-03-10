#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>

struct IP_packet
{
	int packet_id;
	char* sourceIP;
	char* destinationIP;
	int TTL;
	char* payload;
};
