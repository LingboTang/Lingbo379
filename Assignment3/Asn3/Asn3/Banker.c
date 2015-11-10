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
	printf("\n");
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
	int locking[number_p];

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
	current_Available(number_p,number_r,Availres,allocation,current_Avail);
	for (p_ind = 0; p_ind<number_p;p_ind++)
	{
		locking[p_ind] = 0;
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
		if ((counter % 5) == 0)
		{
			printf("\nTimestep %d\n",counter/5+1);
			sleep(2);
		}

		
		/* Request-Release algorithm */

		int k = rdm_num(0,number_p-1);
		curr_Need(number_r,number_p,processes,allocation,current_Need);
		/* Safety algorithm */

		while (locking[k] == 1)
		{
			k = rdm_num(0,number_p-1);
		}
		request_generator(number_r,k,number_p,processes,allocation,request);
		for (j = 0; j<number_r;j++)
		{
			request_Table[k][j] = request_Table[k][j]+request[j];
		}
		printf("Request: ( ");
		for (j = 0; j<number_r; j++)
		{
			printf("%d ",request[j]);
		}
		printf(") from P%d\n", k+1);
		int current_cmp[number_r];
		for (j = 0; j <number_r; j++)
		{
			current_cmp[j] = request_Table[k][j];
		}

		if (veccmp(number_r,current_cmp,current_Avail) == 0)
		{
			printf("Request: ( ");
			for (j = 0; j<number_r; j++)
			{
				printf("%d ",request[j]);
			}
			printf(") from P%d can not be satisfied, P%d is in waiting state.\n", k+1,k+1);
			locking[k] = 1;
		}
		else if (veccmp(number_r,current_cmp,current_Avail) == 1)
		{
			printf("Request: ( ");
			for (j = 0; j<number_r; j++)
			{
				printf("%d ",request_Table[k][j]);
			}
			printf(") from P%d has been granted\n", k+1);
			print_snapshot(number_p,number_r,allocation,request_Table,current_Avail,processes,Availres);
			for (j = 0; j<number_r;j++)
			{
				current_Avail[j]=current_Avail[j]-current_cmp[j];
			}
			for (j = 0; j<number_r;j++)
			{
				request_Table[k][j] = request_Table[k][j]-current_cmp[j];
			}
			for (j = 0; j<number_r;j++)
			{
				allocation[k][j] = allocation[k][j]+current_cmp[j];
			}
			print_snapshot(number_p,number_r,allocation,request_Table,current_Avail,processes,Availres);
		}
		if (locking[k] == 0)
		{
			release(number_p, number_r,k,current_Avail,allocation);
			print_snapshot(number_p,number_r,allocation,request_Table,current_Avail,processes,Availres);
			for(i=0;i<number_p;i++)
			{	
				for (j = 0; j <number_r; j++)
				{
					current_cmp[j] = request_Table[i][j];
				}
				if ((veccmp(number_r,current_cmp,current_Avail) == 1)&&(locking[i] == 1))
				{
					printf("Previous request of ( ");
					for (j=0;j<number_r;j++)
					{
						printf("%d ",request_Table[i][j]);
					}
					printf(") from P%d has been satisfied.\n",i+1);
					for (j = 0; j<number_r;j++)
					{
						current_Avail[j]=current_Avail[j]-current_cmp[j];
					}
					for (j = 0; j<number_r;j++)
					{
						request_Table[i][j] = request_Table[i][j]-current_cmp[j];
					}
					for (j = 0; j<number_r;j++)
					{
						allocation[i][j] = allocation[i][j]+current_cmp[j];
					}
					locking[i] = 0;
				}
				else if ((veccmp(number_r,current_cmp,current_Avail) == 1)&&(locking[i] == 0))
				{
					locking[i] = 0;
				}
			}
			print_snapshot(number_p,number_r,allocation,request_Table,current_Avail,processes,Availres);
		}
		counter++;
	}
	printf("\n\nSimulation has been ended.\n\n");
	return 0;
}


/* Random number generator */
int rdm_num(int min, int max)
{
	if (max <= 0)
	{
		return 0;
	}
	else 
	{
		return min+(rand()%(max-min+1));
	}
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
			allocation[i][j] = 0;
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

void current_Available(int p,int r,int Availres[r],int allocation[p][r],int current_Avail[r])
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
 * request_Table - request.
 * available - request
 ******************************************/

void release(int p, int r,int which,int current_Avail[r],int allocation[p][r])
{
	int m;
	m = rand()%r;
	int release;
	int release_vec[r];
	int j;
	for (j=0; j<r; j++)
	{
		if (j == m)
		{
			release = rdm_num(0,allocation[which][j]-1);
			allocation[which][j] = allocation[which][j] - release;
			current_Avail[j] = current_Avail[j] + release;
		}
		else 
		{
			release = rdm_num(0,allocation[which][j]); 
			allocation[which][j] = allocation[which][j] - release;
			current_Avail[j] = current_Avail[j] + release;
		}
		release_vec[j] = release;
	}
	printf("P%d has released ( ",which+1);
	for(j = 0;j<r;j++)
	{
		printf("%d ",release_vec[j]);
	}
	printf(") resources.\n");
}



/*********************************************
 * Safety Algorithm Checker
 * First we run the safety algorithm to check
 * if the whole system is safe or not.
 * If there exist one allocation which
 * allocation[i] > work[i], then just break
 * and display the system is not safe.
 *********************************************/

int safety_Checker(int p, int r, int allocation[p][r],int current_Avail[r],int Need[p][r])
{
	int i,j;
	int avail_checker[r];
	int safety =0;
	int checked = p;
	int executed = 1;
	int running[p];
	for(j = 0;j<r;j++)
	{
		avail_checker[j] = current_Avail[j];
	}
	for (i = 0; i < p; i++) {
        running[i] = 1;
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
	if (checked == 0)
	{
		safety = 1;
		return safety;
	}
	else
	{
		safety = 0;
		return safety;
	}
}

/************************************************
 * Snapshot
 ************************************************/
void print_snapshot(int p,int r,int allocation[p][r],int request[p][r],int Avail[r],int proc[p][r],int Max[r])
{
	int i,j,k;
	printf("Current snapshot: \n\n");
	for (k = 0;k<5;k++)
	{
		if (k == 0)
		{
			printf("\tCurrent Allocation");
		}
		if (k == 1)
		{
			printf("\t\tCurrent Request");
		}
		if (k == 2)
		{
			printf("\t\t    Currently Available Resource");
		}
		if (k == 3)
		{
			printf("\t\tMaximum Possible Request");
		}
		if (k == 4)
		{
			printf("\t\tMaximum Available Resources");
		}
	}
	printf("\n");
	for (k = 0;k<5;k++)
	{
		printf("\t_____________________________");
	}
	printf("\n");
	for (k = 0; k < 5; k++)
	{
		for (j=0;j<r;j++)
		{
			printf("\tR%d ",j+1);
		}
		printf("\t");
	}
	printf("\n");
	printf("P%d ",1);
	for (k = 0; k < 5; k++)
	{
		if (k == 0)
		{
			for (j =0; j<r;j++)
			{
				printf("\t%d ",allocation[0][j]);
			}
		}
		if (k == 1)
		{
			for (j =0; j<r;j++)
			{
				printf("\t%d ",request[0][j]);
			}
		}
		if (k == 2)
		{
			for (j =0; j<r;j++)
			{
				printf("\t%d ",Avail[j]);
			}
		}
		if (k == 3)
		{
			for (j =0; j<r;j++)
			{
				printf("\t%d ",proc[0][j]);
			}
		}
		if (k == 4)
		{
			for (j =0; j<r;j++)
			{
				printf("\t%d ",Max[j]);
			}
		}
		printf("\t");
	}
	printf("\n");
	for (i = 1;i<p; i++)
	{
		printf("P%d ",i+1);
		for (k = 0; k < 5; k++)
		{
			if (k == 0)
			{
				for (j =0; j<r;j++)
				{
					printf("\t%d ",allocation[i][j]);
				}
			}
			if (k == 1)
			{
				for (j =0; j<r;j++)
				{
					printf("\t%d ",request[i][j]);
				}
			}
			if (k == 2)
			{
				for (j = 0; j< r; j++)
				{
					printf("\t  ");
				}
			}
			if (k == 3)
			{
				for (j =0; j<r;j++)
				{
					printf("\t%d ",proc[i][j]);
				}
			}
			printf("\t");
		}
		printf("\n");
	}
	printf("\n");
	
}


/*******************************************
 * Vector Compare
 *******************************************/
int veccmp(int r,int v1[r],int v2[r])
{
	int flag;
	int j;
	for (j = 0;j<r;j++)
	{
		if (v1[j] > v2[j])
		{
			flag = 0;
			break;
		}
		else
		{
			flag = 1;
		}
	}
	return flag;
}

/************************************
 * Keyboard signal (Ctrl+C) handler
 *************************************/

void sig_handler(int sig)
{
	stop_flag = 0;
}