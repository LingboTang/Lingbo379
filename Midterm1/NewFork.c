#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <sys/wait.h>   /* Wait for Process Termination */
#include <stdlib.h>     /* General Utilities */

int main()
{
	pid_t childpid;
	int retval;
	int status;
	
	childpid = fork();
	
	if (childpid>=0)
	{
		if (childpid ==0)
		{
			printf("CHILD: I am the child process!\n");
            		printf("CHILD: Here's my PID: %d\n", getpid());
            		printf("CHILD: My parent's PID is: %d\n", getppid());
            		printf("CHILD: The value of my copy of childpid is: %d\n", childpid);
            		printf("CHILD: Sleeping for 1 second...\n");
            		sleep(5); /* sleep for 1 second */
            		printf("CHILD: Enter an exit value (0 to 255): ");
            		scanf(" %d", &retval);
            		printf("CHILD: Goodbye!\n");    
            		exit(retval);
		}
		else
		{
	    		printf("PARENT: I am the parent process!\n");
            		printf("PARENT: Here's my PID: %d\n", getpid());
            		printf("PARENT: The value of my copy of childpid is %d\n", childpid);
            		printf("PARENT: I will now wait for my child to exit.\n");
            		wait(&status); /* wait for child to exit, and store its status */
            		printf("PARENT: Child's exit code is: %d\n", WEXITSTATUS(status) );
            		printf("PARENT: Goodbye!\n");             
            		exit(0);
		}
	}
	else
	{
		perror("fork error");
		exit(0);
	}
}  
