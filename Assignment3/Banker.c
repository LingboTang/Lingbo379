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
	
	printf("%d\n",process[4][2]);
	
	//(void) signal(SIGINT, sig_handler);
	//while (stop_flag)
	//{
	//	
	//}
	//printf("\nSimulation has been ended.\n");
	return 0;
}



/*int* request_generator(int r)
{

	return request;
}*/


/*void snapshot()*/

void sig_handler(int sig)
{
	stop_flag = 0;
}
