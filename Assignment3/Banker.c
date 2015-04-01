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
	int process[number_p][number_r];
	printf("\nDetail of process max table:\n");
	for (i = 0;i<number_p;i++)
	{
		for (j=0;j<number_r;j++)
		{
			scanf("%d",&process[i][j]);
		}
	}

	int allocation[number_p][number_r];
	allocation_generator(number_r,number_p,process,allocation);

	int request[number_r];
	int current_Avail[number_r];
	(void) signal(SIGINT, sig_handler);
	int counter = 0;
	while (stop_flag)
	{
		if ((counter % 5) == 0)
		{
			sleep(2);
		}

		int k = rand()%(number_p-0);
		request_generator(number_r,k,number_p,process,allocation,request);
		current_Avilable(number_p,number_r,Availres,allocation,current_Avail);
	}
	printf("\nSimulation has been ended.\n");
	return 0;
}

int rdm_num(const int min, const int max)
{
	return min + ( rand() % (max-min+1) );
}



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

void request_generator(int r,int which,int p,int proc[p][r],int allocation[p][r],int request[r])
{
	int j;
	for (j=0; j<r; j++)
	{
		request[j] = rdm_num(0,proc[which][j]-allocation[which][j]);
	}
}

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

/*void report(int p,int r,int Availres[r],int proc[p][r],int allocation[p][r],int request[r])
{
	printf("\n\t Current Allocation\t Current Request\t Currently Available Resouces\t
		Maximum Possible Request\t Maximum Available Resources\n");
	int i,j;
	
}*/

/*void snapshot()*/
void sig_handler(int sig)
{
	stop_flag = 0;
}