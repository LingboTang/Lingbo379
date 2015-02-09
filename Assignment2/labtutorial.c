#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

/*
SIGINT 2 Term Interrupt from keyboard
*/

int count = 0;

void handler1(int signum)
{
	count++;
	printf("got ^C signal fault:%d Number:%d\n ", signum,count);
	if (count == 5)
		exit(0);
}

void main (int argc, char *argv[])
{
	signal (SIGINT, handler1);
	signal (SIGTSTP,handler1);

	while (1) { printf("Idle\n"); sleep(2);};
}
