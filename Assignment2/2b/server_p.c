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



struct info{
	int clientsd;
        struct sockaddr_in client;
	char *path;
        socklen_t clientlen;
	char *logpath;
};
void request(struct info *i);
void headprint(int log,struct info *i);
void writelog(struct info *inf,int log,char *req,off_t filelength,off_t position);
void *threaduse(void *c);



static void usage()
{
	extern char * __progname;
	fprintf(stderr, "usage: %s portnumber\n", __progname);
	exit(1);
}

static void kidhandler(int signum) {
	/* signal handler for SIGCHLD */
	waitpid(WAIT_ANY, NULL, WNOHANG);
}

int directory(char *path)
{
	struct stat buf;
	memset(&buf,0, sizeof(buf));
	char *path1=path+1;
        stat(path1, &buf);

	if (errno==EACCES){
		return -1;
	}
        if (errno==ENOENT){
	        return -1;
	}
	else{
	        return 0;
	}
}

int checkfile(char *path)
{
	struct stat buf;
	memset(&buf,0, sizeof(buf));
        stat(path, &buf);

	if (errno==EACCES){
		return 2;
	}
        if (errno==ENOENT){
	        return 1;
	}
        if (S_ISREG(buf.st_mode)){
		return 0;
	}
	else{
	        return -1;
	}
}


int main(int argc,  char *argv[])
{
	struct sockaddr_in sockname, client;
	char buffer[80], *ep;
	struct sigaction sa;
	socklen_t clientlen;
        int sd;
	u_short port;
	pid_t pid;
	u_long p;
	pthread_t thread;



	if (argc != 4)
		usage();
		errno = 0;
        p = strtoul(argv[1], &ep, 10);
        if (*argv[1] == '\0' || *ep != '\0') {
		fprintf(stderr, "%s - not a number\n", argv[1]);
		usage();
	}
        if ((errno == ERANGE && p == ULONG_MAX) || (p > USHRT_MAX)) {
		fprintf(stderr, "%s - value out of range\n", argv[1]);
		usage();
	}
	if (directory((char *)argv[2])==-1){
		usage();
	}
	

	/* now safe to do this */
	port = p;

	memset(&sockname, 0, sizeof(sockname));
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(port);
	sockname.sin_addr.s_addr = htonl(INADDR_ANY);
	sd=socket(AF_INET,SOCK_STREAM,0);

	if ( sd == -1)
		err(1, "socket failed");

	if (bind(sd, (struct sockaddr *) &sockname, sizeof(sockname)) == -1)
		err(1, "bind failed");
	
	if (listen(sd,3) == -1)
		err(1, "listen failed");

#ifndef DDEBUG
        if (daemon(1,0) == -1) {
                fprintf(stderr, "ERROR: daemon() failed!!!!!! \n");
                exit(EXIT_FAILURE);
        }
#endif

	sa.sa_handler = kidhandler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        if (sigaction(SIGCHLD, &sa, NULL) == -1)
                err(1, "sigaction failed");

	fprintf(stderr,"Server up and listening for connections on port %u\n", port);
	for(;;) {
		struct info i,*j;
		i.clientlen = sizeof(&client);
		i.clientsd = accept(sd, (struct sockaddr *)&i.client, &i.clientlen);
		if (i.clientsd == -1)
			err(1, "accept failed");

		i.path=(char *)argv[2]+1;
		i.logpath=(char *)argv[3]+1;
		pthread_create(&thread, 0, (void *) threaduse, &i);
	}
}

void *threaduse(void *i){
	request((struct info *)i);
	pthread_exit(NULL);
}
	

void request(struct info *i){
	char cha[1024];
	u_char method[1024];
	u_char filepath[1024];
	u_char fp[1024];
	u_char http[1024];
	struct stat buf;
	int fd;
	off_t filelength;
	int log=0;
	int state=0;
	off_t position;
	off_t offset;
	char other[1024];
	char r2[1024];
	int clen=0;
	
	ssize_t size=read(i->clientsd,cha, sizeof(cha)-1);
	if (size>1) {
		cha[size]='\0';
		sscanf (cha, "%s %s %s", method, filepath, http);
	}
	else{
		log=1;
	}
	
	if (strcmp (http, "HTTP/1.1")) {
		log=1;
	}
	else if (strcmp (method, "GET")) {
		log=1;
	}
	if (log!=1){
		fprintf(stderr,"%d", strcmp (other, "\r\n"));
		while (strcmp (other, "\r\n") != 0 && strcmp (other, "\n") != 0){
			clen=strlen(r2);
			strncpy(other,other+clen,999);
      			size = read (i->clientsd, other, sizeof (other));
			strncpy(r2,other,999);
		}
	}

        strncpy(fp, i->path, strlen(i->path));

       	char *temp = fp + strlen(i->path);
       	strncpy(temp, (char *)filepath,sizeof(filepath));
       	
		
	int filevaild=checkfile(fp);

	if (log==1) log=1;
	else if (filevaild==0){
		log=0;
		stat(fp, &buf);
		fd = open(fp, O_RDONLY);
		filelength=buf.st_size;
	}
	else if (filevaild==2)
		log=2;
	else if (filevaild==1)
		log=3;
	else 
		log=-1;
	headprint(log,i);
		


	while(position!=filelength){
		offset=sendfile(i->clientsd, fd, &offset, filelength-position);
		if (offset == -1) {
			err(1, "error occures when sendfile");
			break;
		}
		position+=offset;
		
	}
	
	writelog(i,log,cha,filelength,position);

	close(fd);

	close(i->clientsd);

	return;
}


void headprint(int log,struct info *inf){
	char print[1000],*pp;
	char h200[1024]="HTTP/1.1 200 OK\r\nDATA: ";
	char h400a[1024]="HTTP/1.1 400 Bad Request\r\nDATA: ";
	char h404a[1024]="HTTP/1.1 404 Not Found\r\nDATA: ";
	char h403a[1024]="HTTP/1.1 403 Forbidden\r\nDATA: ";
	char h500a[1024]="HTTP/1.1 500 Internal Server Error\r\nDATA: ";
	char type[100]="Content-Type: text/html\r\n\r\n";
	char h400b[1024]="<html><body>\r\n<h2>Malformed Request</h2>\r\nYour browser sent a request I could not understand.\r\n</body></html>\r\n";
	char h404b[1024]="<html><body>\r\n<h2>Document not found</h2>\r\nYou asked for a document that doesn't exist. That is so sad.\r\n</body></html>\r\n";
	char h403b[1024]="<html><body>\r\n<h2>Permission Denied</h2>\r\nYou asked for a document you are not permitted to see. It sucks to be you.\r\n</body></html>\r\n";
	char h500b[1024]="<html><body>\r\n<h2>Oops. That Didn't work</h2>\r\nI had some sort of problem dealing with your request. Sorry, I'm lame.\r\n</body></html>\r\n";
	char s[50];
	size_t i;
	struct tm *tim;
	time_t now;
	time(&now);
	tim = gmtime(&now);
	i = strftime(s,50,"%a %d %b %Y %T %Z\n",tim);
	if (log==0){
        	strncpy(print, h200, strlen(h200));
        	pp = print + strlen(h200);
        	strncpy(pp, s, strlen(s));
		pp = pp + strlen(s);
		strncpy(pp, type, strlen(type));
	}
	else if (log==1){
        	strncpy(print, h400a, strlen(h400a));
        	pp = print + strlen(h400a);
        	strncpy(pp, s, strlen(s));
		pp = pp + strlen(s);
		strncpy(pp, type, strlen(type));
		pp = pp + strlen(type);
		strncpy(pp, h400b, strlen(h400b));
	}
	else if (log==2){
        	strncpy(print, h403a, strlen(h403a));
        	pp = print + strlen(h403a);
        	strncpy(pp, s, strlen(s));
		pp = pp + strlen(s);
		strncpy(pp, type, strlen(type));
		pp = pp + strlen(type);
		strncpy(pp, h403b, strlen(h403b));
	}
	else if (log==3){
        	strncpy(print, h404a, strlen(h404a));
        	pp = print + strlen(h404a);
        	strncpy(pp, s, strlen(s));
		pp = pp + strlen(s);
		strncpy(pp, type, strlen(type));
		pp = pp + strlen(type);
		strncpy(pp, h404b, strlen(h404b));
	}
	else if (log==-1){
        	strncpy(print, h500a, strlen(h500a));
        	pp = print + strlen(h500a);
        	strncpy(pp, s, strlen(s));
		pp = pp + strlen(s);
		strncpy(pp, type, strlen(type));
		pp = pp + strlen(type);
		strncpy(pp, h500b, strlen(h500b));
	}
	ssize_t written=0, w;
	while (written < strlen(print)) {
		w = write(inf->clientsd, print + written, strlen(print) - written);
		if (w == -1) {
			if (errno != EINTR)
				err(1, "write failed");
		}
		else
			written += w;

        }

}

void writelog(struct info *inf,int log,char *req,off_t filelength,off_t position)
{
	size_t i;
	char s[50],http[50];
	char *logIP;
	struct tm *tim;
	time_t now;
	time(&now);
	tim = gmtime(&now);
	FILE * logfile;
	i = strftime(s,50,"%a %d %b %Y %T %Z",tim);
	char h200[1024]="200 OK";
	char h400[1024]="400 Bad Request";
	char h404[1024]="404 Not Found";
	char h403[1024]="403 Forbidden";
	char h500[1024]="500 Internal Server Error";
        
	logfile = fopen(inf->logpath,"a+");
        logIP = inet_ntoa(inf->client.sin_addr);

	if (log==0){
        	fprintf(logfile, "%s   %s   %s   %s   %jd/%jd\n",s,logIP,req, h200, (intmax_t) position,(intmax_t) filelength);
	}
	else if (log==1){
		fprintf(logfile, "%s   %s   %s   %s\n",s,logIP, req, h400);

	}
	else if (log==2){
        	fprintf(logfile, "%s   %s   %s   %s\n",s,logIP, req, h403);
	}
	else if (log==3){
        	fprintf(logfile, "%s   %s   %s   %s\n",s,logIP, req, h404);
	}
	else if (log==-1){
        	fprintf(logfile, "%s   %s   %s   %s\n",s,logIP, req, h500);
	}
        fflush(logfile);

}
