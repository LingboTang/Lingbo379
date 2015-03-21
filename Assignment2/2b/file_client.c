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

unsigned long ip_aton(const char *cp);

int main(int argc, char **argv){
	struct sockaddr_in si_client,si_server;
	int s,slen=sizeof(si_server);

	if((s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP))==-1){
		printf("Error in creating socket");
		return 0;
	}

	unsigned long dec_ip=ip_aton(argv[1]);
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

	printf("%s\n",argv[3]);
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
			
			if(strncmp(chunk,"404 Not Found!\n",19)==0){
				printf("Server cannot find the file,exit the program.\n");
				break;
			}
			printf("%s",chunk);
		}
		
	}
	
	close(s);
	printf("File is here!\n");
	
 	return 0;
 }


/* http://courses.cs.vt.edu/cs4254/spring06/unpv13e/libfree/inet_aton.c */
unsigned long ip_aton(const char *cp)
{

    int dots = 0;
    register u_long acc = 0, addr = 0;

    do {
	register char cc = *cp;

	switch (cc) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	    acc = acc * 10 + (cc - '0');
	    break;

	case '.':
	    if (++dots > 3) {
		return 0;
	    }
	    /* Fall through */

	case '\0':
	    if (acc > 255) {
		return 0;
	    }
	    addr = addr << 8 | acc;
	    acc = 0;
	    break;

	default:
	    return 0;
	}
    } while (*cp++) ;

    /* Normalize the address */
    if (dots < 3) {
	addr <<= 8 * (3 - dots) ;
    }

	//printf("%ld\n",(long)addr);

    return 0;    
}
