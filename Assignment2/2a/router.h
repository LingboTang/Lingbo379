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

struct statistic
{
	int Nexpired;
	int Nunroutable;
	int Ndelivered;
	int NrouterB;
	int NrouterC;
};

int fact (int n);
int decrement (int n);
char *decimal_to_binary(int n);
struct ip_pack decode_packet(char* packets);
unsigned int IPtoDec(char*IPdot);
int Ip_masking(char*ip,struct routing table);
struct statistic Make_Decision(char*ip,struct routing* tables);

/*
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
}*/