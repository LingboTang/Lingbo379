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

struct IP_address
{
	int ip;
};

int stop_flag = 1;
char* networkA[3] = {"192.168.128.7","192.168.128.1","networkA"};
char* networkB[4] = {"192.168.192.10","192.168.192.6","192.168.192.4","networkB"};
char* networkC[5] = {"192.224.0.5","192.224.0.7","192.224.10.5","192.224.15.6","networkC"};
char* networks[10] = {"192.168.128.7","192.168.128.1","192.168.192.10","192.168.192.6","192.168.192.4","192.224.0.5","192.224.0.7","192.224.10.5","192.224.15.6","168.130.192.01"};
char* TTL[4] = {"1","2","3","4"};

void random_payload(char * string, size_t length);
char* itoa(int i, char b[]);
void sig_handler(int sig);
