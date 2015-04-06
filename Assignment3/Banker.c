#include "Banker.h"

int main()
{
	/* Setup the random seed */
	srand((unsigned int) time(0));
	int number_r;
	int number_p;
	int i,j;
	/* Number of different Resources
	* eg. A B C D is 4
	*/
	printf("\nNumber of different resource types: ");
	scanf("%d",&number_r);
	/* Number of Available Resouces */
	int Availres[number_r];
	printf("\nNumber of instances of each resource type: ");
	for (i=0;i<number_r; i++)
	{
		scanf("%d",&Availres[i]);
	}
	/* Number of different Process
	* eg. P1 P2 P3 P4 is 4
	*/
	printf("\nNumber of processes: ");
	scanf("%d",&number_p);
	/* Number of different max Res that different Process will request
	* eg. P1 2 2 1
	*/
	int processes[number_p][number_r];
	for (i = 0;i<number_p;i++)
	{
		printf("Detail of process P%d: ",i+1);
		for (j = 0; j< number_r; j++)
		{
			scanf("%d",&processes[i][j]);
		}
		printf("\n");
	}

	/* Generate the current allocation */
	int allocation[number_p][number_r];
	allocation_generator(number_r,number_p,processes,allocation);

	/* Initialize the current request */
	int request[number_r];

	/* Initialize the current available */
	int current_Avail[number_r];

	/* Initialize the current Need */
	int current_Need[number_p][number_r];

	/* Initialize the finish vector */
	int finished[number_p];

	/* Initialize the request table */
	int request_Table[number_p][number_r];
	for (i = 0; i<number_p;i++)
	{
		for (j = 0; j<number_r;j++)
		{
			request_Table[i][j] = 0;
		}
	}

	/* Initialize the current available with value */
	int p_ind;
	current_Avilable(number_p,number_r,Availres,allocation,current_Avail);
	for (p_ind = 0; p_ind<number_p;p_ind++)
	{
		finished[p_ind] = 0;
	}

	/* Termination signal */
	(void) signal(SIGINT, sig_handler);

	/* Program counter */
	int counter = 0;

	/*******************************************
     * Main algorithm
     * It's a combination of Safety algorithm
	 * and Request-Release algorithm
     * First we run the safety algorithm to check
	 * if the whole system is safe or not.
	 * If there exist one allocation which
	 * allocation[i] > work[i], then just break
	 * and display the system is not safe.
	 * If safety algorithm works, we need to
	 * run the request-release algorithm
	 * If the request > Need, break the loop and
	 * display the request over bound.
	 * If the request <= Need,
	 * but request > available,let it wait, until
	 * the next time step.
	 * If request <= available, do the release
     * process.
     *******************************************/
	while (stop_flag)
	{
		if (((counter % 5) == 0) && counter >0)
		{
			sleep(2);
		}
		curr_Need(number_r,number_p,processes,allocation,current_Need);
		/* Safety algorithm */
		if (safety_Checker(number_p, number_r,allocation,current_Avail,current_Need,finished) == 0)
		{
			printf("\n");
			fprintf(stderr,"error: Allocation Overflow!\n");
			exit(EXIT_FAILURE);
		} else {
			printf("\nPassed!\n");
		}
		
		/* Request-Release algorithm */
		int k = rand()%(number_p-0);
		request_generator(number_r,k,number_p,processes,allocation,request);
		printf("Request: (");
		for (j = 0; j<number_r; j++)
		{
			printf("%d ",request[j]);
		}
		printf(") from P%d\n", k+1);
		
		for (i = 0; i < number_p; i++)
		{
			
		}
		counter++;
	}
	printf("\n\nSimulation has been ended.\n\n");
	return 0;
}


/* Random number generator */
int rdm_num(const int min, const int max)
{
	return min + ( rand() % (max-min+1) );
}


/************************************
 * Allocation generator
 * Allocations shouldn't be more
 * than the maximum possible resources
 *************************************/

void allocation_generator(int r,int p,int proc[p][r],int allocation[p][r])
{
	int j;
	int i;
	for (i=0;i<p;i++)
	{
		for (j = 0;j<r; j++)
		{
			allocation[i][j] = rdm_num(0,proc[i][j]);
		}
	}
}

/*******************************************************
 * Request generator
 * Request should be withing the range [0,max-allocation]
 ********************************************************/

void request_generator(int r,int which,int p,int proc[p][r],int allocation[p][r],int request[r])
{
	int j;
	for (j=0; j<r; j++)
	{
		request[j] = rdm_num(0,proc[which][j]-allocation[which][j]);
	}
}

/************************************
 * Current Need Table
 * Need[i] = Max[i] - Allocation[i]
 *************************************/

void curr_Need(int r, int p, int proc[p][r],int allocation[p][r],int Need[p][r])
{
	int i,j;
	for (i = 0;i<p;i++)
	{
		for (j =0; j < r; j++)
		{
			Need[i][j] = proc[i][j]-allocation[i][j];
		}
	}
}

/******************************************
 * Current Available Table at the beginning
 * Available[i] = SysMax[i] - Allocation[i]
 ******************************************/

void current_Avilable(int p,int r,int Availres[r],int allocation[p][r],int current_Avail[r])
{
	int i,j;
	int counter;
	for (j=0;j<r;j++)
	{
		counter = 0;
		for (i = 0; i<p; i++)
		{
			counter = counter+allocation[i][j];
		}
		current_Avail[j] = Availres[j]-counter;
	}
}

/******************************************
 * Release vector are randomly generated
 * When we release,we should leave at least
 * one instance in the process. And
 * allocation = allocation -release.
 * available = availabe + release.
 ******************************************/

void release(int p, int r,int which,int current_Avail[r],int allocation[p][r])
{
	int m;
	m = rdm_num(0,r);
	int release;
	release = rand()%(allocation[which][m]-1);
	allocation[which][m] = allocation[which][m] - release;
	current_Avail[m] = current_Avail[m] + release;
	int j;
	for (j=0; j<r; j++)
	{
		if (j == m)
		{
			continue;
		}
		else
		{
			release = rdm_num(0,allocation[which][j]);
			allocation[which][j] = allocation[which][j] - release;
			current_Avail[j] = current_Avail[j] + release;
		}
	}
}

/*********************************************
 * Safety Algorithm Checker
 * First we run the safety algorithm to check
 * if the whole system is safe or not.
 * If there exist one allocation which
 * allocation[i] > work[i], then just break
 * and display the system is not safe.
 *********************************************/

int safety_Checker(int p, int r, int allocation[p][r],int current_Avail[r],int Need[p][r],int finish[p])
{
	int i,j;
	int avail_checker[r];
	int safety =0;
	int checked = r;
	int executed = 1;
	int running[p];
	for (i = 0; i < p; i++) {
        running[i] = 1;
    }
	for (j=0;j<r;j++)
	{
		avail_checker[j] = current_Avail[j];
	}
	while (checked != 0) {
        safety = 0;
        for (i = 0; i < p; i++) {
            if (running[i]) {
                executed = 1;
                for (j = 0; j < r; j++) {
                    if (Need[i][j] > avail_checker[j]) {
                        executed = 0;
                        break;
                    }
                }
                if (executed) {
                    running[i] = 0;
                    checked--;
                    safety = 1;
                    for (j = 0; j < r; j++) {
                        avail_checker[j] += allocation[i][j];
                    }
                    break;
                }
            }
        }
        if (!safety) {
            break;
        }
    }
	return safety;
}


/************************************
 * Keyboard signal (Ctrl+C) handler
 *************************************/

void sig_handler(int sig)
{
	stop_flag = 0;
}
