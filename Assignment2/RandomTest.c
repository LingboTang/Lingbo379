#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "pktgen.h"

/* Random generator Referrence
 * http://ubuntuforums.org/showthread.php?t=1979310 
 * author: Buffay
 * Edit date: March 13th, 2012
 */


void random_payload(char * string, size_t length)
{
    /* Seed number for rand() */
    //srand((unsigned int) time(0));  
     
    /* ASCII characters 33 to 126 */
    unsigned int num_chars = length - 1;
    unsigned int i;
    for (i = 0; i < num_chars; ++i)
    {
        string[i] = rand() % (126 - 33 + 1) + 33;
    }
 
    string[num_chars] = '\0';  
}

/*
 * Referrence of itoa function
 * http://stackoverflow.com/questions/9655202/how-to-convert-integer-to-string-in-c
 * author: bhuwansahni
 * Edit date: March 12,2012
 */
 
char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

char* random_IP(char * string,size_t length)
{	
	//srand(time(NULL));
	struct IP_address *address_list;
	address_list = malloc(sizeof(struct IP_address) * 4);
	int i;
	char string2[4],string3[4],string4[4];
	for (i=0; i<5; i++)
	{
		address_list[i].ip = rand()% (255-0+1)+0;
	}
    char* token1 = itoa(address_list[1].ip,string);
	char* token2 = itoa(address_list[2].ip,string2);
	char* token3 = itoa(address_list[3].ip,string3);
	char* token4 = itoa(address_list[4].ip,string4);
	char* token5 = ".";
	char* IP = (char*)malloc(16);
	strcpy(IP,token1);
	strcat(IP,token5);
	strcat(IP,token2);
	strcat(IP,token5);
	strcat(IP,token3);
	strcat(IP,token5);
	strcat(IP,token4);
	//free(IP);
	free(address_list);
	return IP;
}

int randomTTL() {
	//srand(time(NULL));
	int r;
    r = rand();
	r = r % (4 - 1 + 1) + 1;
	return r;
}


int main(void)
{
	srand((unsigned int) time(0)); 
    int Packet_ID=0;
	int thisTTL;
    while (1)
	{
	    Packet_ID++;
	    char s[20]; 
	    thisTTL = randomTTL();    
	    random_payload(s, 20);  	    
	    char bufferIP[4];
	    char* source_IP;
	    source_IP = random_IP(bufferIP, 4);		
		char bufferIP2[4];
		char* destination_IP;
		destination_IP = random_IP(bufferIP2,4);
		printf("\nPacket_ID: %d\n",Packet_ID);
		printf("\nsource_IP: %s\n",source_IP);
	    printf("\ndestination_IP: %s\n", destination_IP);
		printf("\nTTL: %d\n",thisTTL);
		printf("\npayload: %s\n", s);
		struct IP_packet *Package_list;
		Package_list = malloc(sizeof(struct IP_packet) *20);
        for (int i = 0; i<20; i++)
	    {
		    Package_list[i].packet_id = Packet_ID;
		    Package_list[i].sourceIP = source_IP;
			Package_list[i].destinationIP = destination_IP;
			Package_list[i].TTL = thisTTL;
			Package_list[i].payload = s;
		}
		free(Package_list);
	}  
    return 0;
}
