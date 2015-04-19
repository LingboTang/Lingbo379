#include <stdio.h>
#include <stdlib.h>


int main()
{
	int frames;
	printf("Please enter how many frames in system: \n");
	scanf("%d",frames);
	int no_strings;
	printf("How many strings: \n");
	int strings[no_strings];
	printf("\nNumber of instances of each resource type: ");
	for (i=0;i<no_strings; i++)
	{
		scanf("%d",&strings[i]);
	}

	int process[frames][no_strings];
	int i,j;
	for (j = 0;j<no_strings;j++)
	{
		for(i = 0;i<frames;i++)
		{
			if (i == 0)
			{
				process[i][j]
			}
		}
	}
}