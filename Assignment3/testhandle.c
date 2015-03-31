#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>



int main() 
{
	char number_r;
	printf("\nNumber of different resource types: ");
	scanf("%c",&number_r);
	while (isalpha(number_r))
	{
		char number_r;
		printf("\nNumber of different resource types: ");
		scanf("%c",&number_r);
	}
	return 0;
}