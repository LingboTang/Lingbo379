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
#define CHUNKLEN 1024
#define BUFLEN2 32

void writelog(char *clientIP,int clientPORT,char* Filename_sent,char *request_time,char *transmission_time,FILE *ofp,int flag);
int get_file_size(FILE*ifp);
char* get_time();

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
	si_server.sin_addr.s_addr = htonl(INADDR_ANY); /* htonl(INADDR_ANY) for any interface on this machine */
	

    //if (daemon(1,0) == -1) 
    //    printf("ERROR: daemon() failed!!!!!! \n");

	if ( bind(s, (struct sockaddr *)&si_server, sizeof(si_server)) == -1 )
	{
		printf("Error in binding the socket");
		return 2;
	}
	
		

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

	char input_filepath[200];
	memset(input_filepath,0,201);

	char buffer[100];
	memset(buffer,0,101);
	char logger[8] = "log.txt";
	char output_filepath[256];
	memset(output_filepath,0,256);

	char*req_time;
	char* tras_time = NULL;
	

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
				req_time = get_time();
				strcpy(input_filepath,filepath1);
				strcat(input_filepath,"/");
				strcat(input_filepath,buffer);
				printf("File path found: %s\n",input_filepath);

				/*
				 * Set up the file path for
				 * our server log.
				 */
				
				
				strcpy(output_filepath,filepath2);
				strcat(output_filepath,"/");
				strcat(output_filepath,logger);
				printf("%s\n",output_filepath);
				
				
				ifp = fopen(input_filepath, "r");
				ofp = fopen(output_filepath, "w");
				int flag;
				if(ifp==NULL)
				{
					fprintf(stderr,"File not found.\n");
					char error_mssg[20];
					strcpy(error_mssg,"404 Not found!\n");
					sendto(s,error_mssg,20,0,(struct sockaddr*)&si_client,sizeof(si_client));
					flag = 1;
					writelog(buffer,PORT,input_filepath,req_time,tras_time,ofp,flag);
					break;
				}

				char chunk[CHUNKLEN];
				while(!feof(ifp)){
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
					if(sendto(s,chunk,sizeof(chunk)+1,0,(struct sockaddr*)&si_client,sizeof(si_client))<0){
						printf("Transmission failed at this time, stop transmission.\n");
						flag = 2;
						writelog(buffer,PORT,input_filepath,req_time,tras_time,ofp,flag);
						break;
					}
				}
				
				tras_time = get_time();
				printf("%s\n",tras_time);
				flag = 0;	
				writelog(buffer,PORT,input_filepath,req_time,tras_time,ofp,flag);
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


char* get_time()
{
	char s[80];
	memset(s,0,81);
	struct tm *tim;
	time_t now;
	time(&now);
	tim = localtime(&now);
	strftime(s,80,"%a %d %b %Y %T %Z",tim);
	return strdup(s);
}

void writelog(char *clientIP,int clientPORT,char* Filename_sent,char *request_time,char *transmission_time,FILE *ofp,int flag)
{
	char* flag_notfound = "file not found!";
	char* flag_notcompleted = "transmission not completed!";
	if (flag == 0)
	{
		fprintf(ofp,"%s %d %s %s %s\n",clientIP,clientPORT,Filename_sent,request_time, transmission_time);
	}
	else if(flag == 1)
	{
		fprintf(ofp,"%s %d %s %s %s\n",clientIP,clientPORT,Filename_sent,request_time, flag_notfound);
	}
	else if(flag == 2)
	{
		fprintf(ofp,"%s %d %s %s %s\n\n",clientIP,clientPORT,Filename_sent,request_time, flag_notcompleted);
	}
	fflush(ofp);
}

static void kidhandler(int signum) {
	/* signal handler for SIGCHLD */
	waitpid(WAIT_ANY, NULL, WNOHANG);
}
