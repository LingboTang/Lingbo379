#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

int pfd1[2],pfd2[2];

void enable_waiting (void)
{
	if (pipe(pfd1)<0||pipe(pfd2)<0)
	{
		printf("pipe creation error");
		exit(1);
	}
}

void notify_parent (void)
{
	if (write(pfd2[1],"c",1)!=1)
	{
		printf("NP:Write error");
		exit(2);
	}
}

void wait_for_parent(void)
{
	char c;
	if(read(pfd1[0],&c,1)!=1)
	{
		printf("\nWFP: read error.\n");
		exit(3);
	}

	if (c!='p')
	{
		printf("\nWFP:incorrect data\n");
		exit(4);
	}
}

void notify_child(void)
{
	if (write(pfd2[1],"p",1)!=1)
	{
		printf("NC:Write error");
		exit(5);
	}
}

void wait_for_child(void)
{
	char c;
	if(read(pfd2[0],&c,1)!=1)
	{
		printf("\nWFC: read error.\n");
		exit(6);
	}

	if (c!='c')
	{
		printf("\nWFC:incorrect data\n");
		exit(7);
	}
}

int main(void)
{
	pid_t chpid;

	enable_waiting();

	chpid = fork();

	if (chpid< 0)
	{
		printf("\nfork error\n");
		return -1;
	}

	if (chpid >0) /* parent */
	{
		printf("P: Msg1");

		notify_child();
		wait_for_child();

		printf("P: Msg2");

		notify_child();
		wait_for_child();

		printf("P: Msg3");

		notify_child();

		wait(NULL);

	}
	else /*child */
	{
		wait_for_parent();
		printf("C: Msg1");

		notify_parent();
		wait_for_parent();

		printf("C: Msg2");

		notify_parent();
		wait_for_parent();

		printf("C: Msg3");

	}
	return 0;
}