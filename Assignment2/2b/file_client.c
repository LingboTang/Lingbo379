#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#define CHUNKLEN 1024

unsigned int IPtoDec(char*IPdot);

int main(int argc, char **argv){
	struct sockaddr_in si_client,si_server;
	int s,slen=sizeof(si_server);

	if((s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))==-1){
		printf("Error in creating socket");
		return 0;
	}

	int dec_ip=IPtoDec(argv[1]);
	int port=atoi(argv[2]);

	si_client.sin_family = AF_INET;
	si_client.sin_port = htons(port+1);
	si_client.sin_addr.s_addr = htonl(dec_ip); 
	
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(port);
	si_server.sin_addr.s_addr = htonl(dec_ip); 


	if(bind(s,(struct sockaddr*)&si_client,sizeof(si_client))==-1){
		printf("Error in binding the socket\n");
		return 0;
	}

	printf("File Client listening to %s:%d\n",inet_ntoa(si_client.sin_addr),ntohs(si_client.sin_port));

	printf("Sending %s to %s:%d\n",argv[3],inet_ntoa(si_server.sin_addr),ntohs(si_server.sin_port));
	sendto(s,argv[3],strlen(argv[3])+1,0,(struct sockaddr*)&si_server, sizeof(si_server));

	char chunk[CHUNKLEN];

	struct timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	
	setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv,sizeof(struct timeval));

	while(1){
		
		if(recvfrom(s,chunk,sizeof(chunk)+1,0,(struct sockaddr*)&si_server,(socklen_t *)&slen)!=-1){
			setsockopt(s,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv,sizeof(struct timeval));
			
			printf("%s",chunk);
		}
		
	}
	
	close(s);
	printf("File is here!\n");
	
 	return 0;
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