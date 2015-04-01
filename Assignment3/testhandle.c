#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>



int main() 
{
	char number_r[1];
	memset(number_r,0,1);
	printf("\nNumber of resources: ");
	scanf("%s",number_r);
	int r = atoi(number_r);
	while (!isalpha(r))
	{
		char number_r[1];
		memset(number_r,0,2);
		printf("\nNumber of resources: ");
		scanf("%s",number_r);
		int r = atoi(number_r);
	}
	return 0;
}