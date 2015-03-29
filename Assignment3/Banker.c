#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>


int stop_flag = 1;

void sig_handler(int sig);


int main() 
{

	srand((unsigned int) time(0));
	(void) signal(SIGINT, sig_handler);
	while (stop_flag)
	{
		
	 
		// Main procedure goes below to check for unsafe state.
		while (count != 0) {
		    safe = 0;
		    for (i = 0; i < p; i++) {
		        if (running[i]) {
		            exec = 1;
		            for (j = 0; j < r; j++) {
		                if (maxclaim[i][j] - curr[i][j] > avl[j]) {
		                    exec = 0;
		                    break;
		                }
		            }
		            if (exec) {
		                printf("\nProcess%d is executing\n", i + 1);
		                running[i] = 0;
		                count--;
		                safe = 1;
	 
		                for (j = 0; j < r; j++) {
		                    avl[j] += curr[i][j];
		                }
	 
		                break;
		            }
		        }
		    }
		    if (!safe) {
		        printf("\nThe processes are in unsafe state.\n");
		        break;
		    } else {
		        printf("\nThe process is in safe state");
		        printf("\nSafe sequence is:");
	 
		        for (i = 0; i < r; i++) {
		            printf("\t%d", avl[i]);
		        }
	 
		        printf("\n");
		    }
		}
	}
	printf("\nSimulation has been ended.\n");
	return 0;
}

void init_simu()
{
	int number_r;
	printf("\nNumber of different resource types: ");
	scanf("%d",&number_r);
	printf("\nNumber of instances of each resource type: ");

	printf("\nNumber of processes: ");
	printf("Details of P1: 2 2 1");
}

/*
int[] request_generator()
{

	return request;
}
*/


void sig_handler(int sig)
{
	stop_flag = 0;
}
