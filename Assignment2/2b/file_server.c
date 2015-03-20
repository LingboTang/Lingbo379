#include <sys/types.h>
#include <sys/stat.h>

#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <netinet/in.h>

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <arpa/inet.h>

#define BUFLEN 1024
#define IP 2130706433  /* 127.0.0.1 */
#define CHUNKLEN 1024
#define BUFLEN2 32

void writelog(char *clientIP,int clientPORT,char* Filename_sent,FILE *ofp,int flag);
int get_file_size(FILE*ifp);
void split_file(char chunk[CHUNKLEN],FILE*ifp,int turn);

static void kidhandler(int signum);



int main(int argc,char** argv)
{
	struct sockaddr_in si_server, si_client;
	int s, slen=sizeof(si_client);

	/* If the user input the wrong command line */
	/* just exit with failure */
	if ( argc != 4 )
	{
		printf("\n\nusage: %s <int value>\n\n", argv[0]);
		exit(1);
	}

	/* The first argument is the port number */
	int PORT = atoi(argv[1]);
	/* The second argument is the requesting file path */
	char * filepath1;
	filepath1 = argv[2];
	FILE *ifp;
	/* The third argument is the log file path */
	char * filepath2;
	filepath2 = argv[3];
	FILE *ofp;

	if ( ( s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP) ) == -1 )
	{
		printf("Error in creating socket");
		return 1;
	}

	memset((char *) &si_server, 0, sizeof(si_server));
	si_server.sin_family = AF_INET;
	si_server.sin_port = htons(PORT);
	si_server.sin_addr.s_addr = htonl(IP); /* htonl(INADDR_ANY) for any interface on this machine */

	if ( bind(s, (struct sockaddr *)&si_server, sizeof(si_server)) == -1 )
	{
		printf("Error in binding the socket");
		return 2;
	}

	printf("daemon\n");
    //if (daemon(1,0) == -1) {
    //    printf("ERROR: daemon() failed!!!!!! \n");
    //   exit(EXIT_FAILURE);
    //}
	printf("daemon passed\n");

	struct sigaction sa;
	/*
	 * first, let's make sure we can have children without leaving
	 * zombies around when they die - we can do this by catching
	 * SIGCHLD.
	 */
	sa.sa_handler = kidhandler;
    sigemptyset(&sa.sa_mask);
	/*
	 * we want to allow system calls like accept to be restarted if they
	 * get interrupted by a SIGCHLD
	 */
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1){
        err(1, "sigaction failed");
	}

	char buffer[100];
	memset(buffer,0,101);

	printf("\n\nServer listening to %s:%d\n\n", inet_ntoa(si_server.sin_addr), ntohs(si_server.sin_port));
	while (1) 
	{	
		if(recvfrom(s,buffer,100,0,(struct sockaddr *)&si_client,(socklen_t *)&slen)!=-1)
		{	
			pid_t pid = fork();

			if (pid == 0)
			{

				/* 
				 * Get client ID and send the package from
				 * the setup filepath to the client.
				 */

				char input_filepath[200];
				memset(input_filepath,0,201);
				strcpy(input_filepath,filepath1);
				strcat(input_filepath,"/");
				strcat(input_filepath,buffer);
				printf("File path found: %s\n",input_filepath);

				/*
				 * Set up the file path for
				 * our server log.
				 */
				char logger[8] = "log.txt";
				char output_filepath[256];
				memset(output_filepath,0,256);
				char client_port[5];
				memset(client_port,0,6);
				sprintf(client_port,"%d",ntohs(si_client.sin_port));
				strcpy(output_filepath,filepath2);
				strcat(output_filepath,"/");
				strcat(output_filepath,logger);
				printf("%s\n",logger);
				
				
				ifp = fopen(filepath1, "r");
				ofp = fopen(filepath2, "w");
				int flag;

				if(ifp==NULL)
				{
					fprintf(stderr,"File not found.\n");
					char error_mssg[20];
					strcpy(error_mssg,"404 Not found!\n");
					sendto(s,error_mssg,20,0,(struct sockaddr*)&si_client,sizeof(si_client));
					flag = 1;
					writelog(buffer,PORT,input_filepath,ofp,flag);
					break;
				}

				char chunk[CHUNKLEN];

				int turn=0;
				while(!feof(ifp))
				{	
					split_file(chunk,ifp,turn);
						
					if(sendto(s,chunk,sizeof(chunk)+1,0,(struct sockaddr*)&si_client,sizeof(si_client))<0){
						printf("Transmission failed at this time, stop transmission.\n");
						flag = 2;
						writelog(buffer,PORT,input_filepath,ofp,flag);
						break;
					}

					turn++;
				}
				
				flag = 0;	
				writelog(buffer,PORT,input_filepath,ofp,flag);
				fclose(ifp);
				exit(0);
			}
			else if (pid == -1)
			{
				fprintf(stderr, "Fork failed.\n");
			}
		}
	}
	
	fclose(ofp);
	close(s);
 	return 0;
 }

int get_file_size(FILE*ifp)
{
	int counter = 0;
	while(1)
	{
		if(fgetc(ifp) == EOF)
		{
			break;
		}
		counter++;
	}
	return counter;
}


void split_file(char chunk[CHUNKLEN],FILE*ifp,int turn)
{
	memset(chunk,0,CHUNKLEN+1);
	int c;
	int index;
	for(index = 0; index< CHUNKLEN; index++)
	{
		c = fgetc(ifp);
		if (c!=EOF)
		{
			strcat(chunk,(char*)&c);
		}
		if(feof(ifp))
		{
			if(index == 0)
			{
				strcat(chunk,"$");
			}
			break;
		}
	}
	printf("chunk with ID: %d has been created and will be send.\n",turn);
}

void writelog(char *clientIP,int clientPORT,char* Filename_sent,FILE *ofp,int flag)
{
	int request_time=2;
	int transmission_time=3;
	char* flag_notfound = "file not found!";
	char* flag_notcompleted = "transmission not completed!";
	if (flag == 0)
	{
		fprintf(ofp,"%s %d %s %d %d\n",clientIP,clientPORT,Filename_sent,request_time, transmission_time);
	}
	else if(flag == 1)
	{
		fprintf(ofp,"%s %d %s %d %s\n",clientIP,clientPORT,Filename_sent,request_time, flag_notfound);
	}
	else if(flag == 2)
	{
		fprintf(ofp,"%s %d %s %d %s\n\n",clientIP,clientPORT,Filename_sent,request_time, flag_notcompleted);
	}
	fflush(ofp);
}

static void kidhandler(int signum) {
	/* signal handler for SIGCHLD */
	waitpid(WAIT_ANY, NULL, WNOHANG);
}