#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <netinet/in.h>

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <time.h>
#include <arpa/inet.h>

/* we use this structure to keep track of each connection to us */
struct con {
	int sd; 	/* the socket for this connection */
	int state; 	/* the state of the connection */
	struct sockaddr_in sa; /* the sockaddr of the connection */
	size_t  slen;   /* the sockaddr length of the connection */
	char *buf;	/* a buffer to store the characters read in */
	char *bp;	/* where we are in the buffer */
	size_t bs;	/* total size of the buffer */
	size_t bl;	/* how much we have left to read/write */
	int log;
	char rm[1024];
	int head;
	char req[1024];
	char *path;
	char *logpath;
	u_char fp[1024];
	off_t filelength;
	u_char filepath[1024];
	int fd;
};


#define MAXCONN 256
struct con connections[MAXCONN];

#define BUF_ASIZE 256 /* how much buf will we allocate at a time. */

/* states used in struct con. */
#define STATE_UNUSED 0
#define STATE_READING 1
#define STATE_WRITING 2


static void usage()
{
	extern char * __progname;
	fprintf(stderr, "usage: %s portnumber\n", __progname);
	exit(1);
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

void headprint(int log,struct con *inf){
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
	//fprintf(stderr,"%s\n", print);
	//char *p=print;
	ssize_t written=0, w;
	while (written < strlen(print)) {
		w = write(inf->sd, print + written, strlen(print) - written);
		if (w == -1) {
			if (errno != EINTR)
				err(1, "write failed");
		}
		else
			written += w;

        }

}

void writelog(struct con *inf,int log,char *req,off_t filelength,off_t position)
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

        logIP = inet_ntoa(inf->sa.sin_addr);

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
	fclose(logfile);

}

/*
 * get a free connection structure, to save a new connection in
 */
struct con * get_free_conn()
{
	int i;
	for (i = 0; i < MAXCONN; i++) {
		if (connections[i].state == STATE_UNUSED)
			return(&connections[i]);
	}
	return(NULL); /* we're all full - indicate this to our caller */
}



/*
 * close or initialize a connection - resets a connection to the default,
 * unused state.
 */
void closecon (struct con *cp, int initflag)
{
	if (!initflag) {
		if (cp->sd != -1)
			close(cp->sd); /* close the socket */
		free(cp->buf); /* free up our buffer */
	}
	memset(cp, 0, sizeof(struct con)); /* zero out the con struct */
	cp->buf = NULL; /* unnecessary because of memset above, but put here
			 * to remind you NULL is 0.
			 */
	cp->sd = -1;
}


void handlewrite(struct con *cp)
{
	off_t position;
	off_t offset;

	headprint(cp->log,cp);
	if (cp->log==0){
		while(position!=cp->filelength){
			offset=sendfile(cp->sd, cp->fd, &offset, cp->filelength-position);
			if (offset == -1) {
				err(1, "error occures when sendfile");
				break;
			}
			position+=offset;
		
		}
	}
	writelog(cp,cp->log,cp->req,cp->filelength,position);
	close(cp->fd);
	cp->state = STATE_UNUSED;
}

/*
 * handleread - deal with a connection that we want to read stuff
 * from. assumes the caller has checked that cp->sd is writeable
 * by using select(). If a newline is seen at the end of what we
 * are reading, change the state of this connection to the writing
 * state.
 */
void handleread(struct con *cp)
{
	char cha[1024];
	u_char method[1024];
	u_char http[1024];
	struct stat buf;
	off_t filelength;
	int state=0;
	char other[1024];
	char r2[1024];
	int clen=0;
	ssize_t size;

	if (cp->head==0){
		size=read(cp->sd,cp->req, sizeof(cha)-1);
		if (size>1) {
			cp->req[1024]='\0';
			sscanf (cp->req, "%s %s %s", method, cp->filepath, http);
		}
		else{
			cp->log=1;
		}
	
		if (strcmp (http, "HTTP/1.1")) {
			cp->log=1;
		}
		else if (strcmp (method, "GET")) {
			cp->log=1;
		}
		cp->head==1;
	}
	else if (cp->log!=1){
		clen=strlen(cp->rm);
		strncpy(other,other+clen,999);
      		size = read (cp->sd, other, sizeof (other));
		strncpy(cp->rm,other,999);
		if (strcmp (other, "\r\n") != 0 && strcmp (other, "\n") != 0){
			cp->state = STATE_WRITING;
		}
		cp->head+=1;
	}
	if (cp->head==1 && cp->log ==0){	
        	strncpy(cp->fp, cp->path, strlen(cp->path));
	       	char *temp = cp->fp + strlen(cp->path);
       		strncpy(temp, (char *)cp->filepath,sizeof(cp->filepath));
			
		int filevaild=checkfile(cp->fp);
	

		if (filevaild==0){
			cp->log=0;
			stat(cp->fp, &buf);
			cp->fd = open(cp->fp, O_RDONLY);
			cp->filelength=buf.st_size;
		}
		else if (filevaild==2)
			cp->log=2;
		else if (filevaild==1)
			cp->log=3;
		else 
			cp->log=-1;
	}	
}

int main(int argc,  char *argv[])
{
	struct sockaddr_in sockname;
	int max = -1, omax;	     /* the biggest value sd. for select */
	int sd;			     /* our listen socket */
	fd_set *readable = NULL , *writable = NULL; /* fd_sets for select */
	u_short port;
	u_long p;
	char *ep;
	int i;

	/*
	 * first, figure out what port we will listen on - it should
	 * be our first parameter.
	 */
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

	/* now before we get going, decide if we want to daemonize, that
	 * is, run in the background like a real system process
	 */
#ifndef DEBUG
	/* don't daemonize if we compile with -DDEBUG */
	if (daemon(1, 0) == -1)
		err(1, "daemon() failed");
#endif

	/* now off to the races - let's set up our listening socket */
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

	/* 
	 * We're now bound, and listening for connections on "sd".
	 * Each call to "accept" will return us a descriptor talking to
	 * a connected client.
	 */

	/*
	 * finally - the main loop.  accept connections and deal with 'em
	 */
	/*
	 * since we'll be running as a daemon if we're not compiled with
	 * -DDEBUG, we better not be using printf - since stdout will be
	 * unusable
	 */
	fprintf(stderr,"Server up and listening for connections on port %u\n", port);

	/* initialize all our connection structures */
	for (i = 0; i < MAXCONN; i++)
		closecon(&connections[i], 1);

	for(;;) {
		int i;
		int maxfd = -1; /* the biggest value sd we are interested in.*/

		/*
		 * first we have to initialize the fd_sets to keep;
		 * track of readable and writable sockets. we have
		 * to make sure we have fd_sets that are big enough
		 * to hold our largest valued socket descriptor.
		 * so first, we find the max value by iterating through
		 * all the connections, and then we allocate fd sets
		 * that are big enough, if they aren't already.
		 */
		omax = max;
		max = sd; /* the listen socket */

		for (i = 0; i < MAXCONN; i++) {
			if (connections[i].sd > max)
				max = connections[i].sd;
		}
		if (max > omax) {
			/* we need bigger fd_sets allocated */

			/* free the old ones - does nothing if they are NULL */
			free(readable);
			free(writable);

			/*
			 * this is how to allocate fd_sets for select
			 */
			readable = (fd_set *)calloc(howmany(max + 1, NFDBITS),
			    sizeof(fd_mask));
			if (readable == NULL)
				err(1, "out of memory");
			writable = (fd_set *)calloc(howmany(max + 1, NFDBITS),
			    sizeof(fd_mask));
			if (writable == NULL)
				err(1, "out of memory");
			omax = max;
			/*
			 * note that calloc always returns 0'ed memory,
			 * (unlike malloc) so these sets are all set to 0
			 * and ready to go
			 */
		} else {
			/*
			 * our allocated sets are big enough, just make
			 * sure they are cleared to 0. 
			 */
			memset(readable, 0, howmany(max+1, NFDBITS) *
			    sizeof(fd_mask));
			memset(writable, 0, howmany(max+1, NFDBITS) *
			    sizeof(fd_mask));
		}

		/*
		 * Now, we decide which sockets we are interested
		 * in reading and writing, by setting the corresponding
		 * bit in the readable and writable fd_sets.
		 */

		/*
		 * we are always interesting in reading from the
		 * listening socket. so put it in the read set.
		 */

		FD_SET(sd, readable);
		if (maxfd < sd)
			maxfd = sd;

		/*
		 * now go through the list of connections, and if we
		 * are interested in reading from, or writing to, the
		 * connection's socket, put it in the readable, or
		 * writable fd_set - in preparation to call select
		 * to tell us which ones we can read and write to.
		 */
		for (i = 0; i<MAXCONN; i++) {
			if (connections[i].state == STATE_READING) {
				FD_SET(connections[i].sd, readable);
				if (maxfd < connections[i].sd)
					maxfd = connections[i].sd;
			}
			if (connections[i].state == STATE_WRITING) {
				FD_SET(connections[i].sd, writable);
				if (maxfd < connections[i].sd)
					maxfd = connections[i].sd;
			}
		}

		/*
		 * finally, we can call select. we have filled in "readable"
		 * and "writable" with everything we are interested in, and
		 * when select returns, it will indicate in each fd_set
		 * which sockets are readable and writable
		 */
		i = select(maxfd + 1, readable, writable, NULL,NULL);
		if (i == -1  && errno != EINTR)
			err(1, "select failed");
		if (i > 0) {

			/* something is readable or writable... */

			/*
			 * First things first.  check the listen socket.
			 * If it was readable - we have a new connection
			 * to accept.
			 */

			if (FD_ISSET(sd, readable)) {
				struct con *cp;
				int newsd;
				socklen_t slen;
				struct sockaddr_in sa;
				slen = sizeof(sa);
				newsd = accept(sd, (struct sockaddr *)&sa,
				    &slen);
				if (newsd == -1)
					err(1, "accept failed");

				cp->path=(char *)argv[2]+1;
				cp->logpath=(char *)argv[3]+1;


				cp = get_free_conn();
				if (cp == NULL) {
					/*
					 * we have no connection structures
					 * so we close connection to our
					 * client to not leave him hanging
					 * because we are too busy to
					 * service his request
					 */
					close(newsd);
				} else {
					/*
					 * ok, if this worked, we now have a
					 * new connection. set him up to be
					 * READING so we do something with him
					 */
					cp->state = STATE_READING;
					cp->sd = newsd;
					cp->slen = slen;
					memcpy(&cp->sa, &sa, sizeof(sa));
				}
			}
			/*
			 * now, iterate through all of our connections,
			 * check to see if they are readble or writable,
			 * and if so, do a read or write accordingly 
			 */
			for (i = 0; i<MAXCONN; i++) {
				if ((connections[i].state == STATE_READING) &&
				    FD_ISSET(connections[i].sd, readable))
					handleread(&connections[i]);
				if ((connections[i].state == STATE_WRITING) &&
				    FD_ISSET(connections[i].sd, writable))
					handlewrite(&connections[i]);
			}
		}
	}
}

