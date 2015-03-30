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

	(void) signal(SIGINT, sig_handler);
	int counter = 0;
	while (stop_flag)
	{
		if (((counter % 5) == 0) && (counter >0))
		{
			sleep(2);
		}
		int k = rdm_req(0,number_p);
		request_generator(number_r,k,number_p,process);
		counter++;

	}
	printf("\nSimulation has been ended.\n");
	return 0;
}

int rdm_req(const int min, const int max) 
{
	return min + ( rand() % (max-min+1) );
}

int* request_generator(int r,int which,int p,int proc[p][r])
{
	int j;
	int request[r];
	for (j=0; j<r; j++)
	{
		printf("%d ",proc[which][j]);
		request[j] = rdm_req(0,proc[which][j]);
	}
	//printf("%d\n",request[1]);
	return request[r];
}


/*void snapshot()*/

void sig_handler(int sig)
{
	stop_flag = 0;
}