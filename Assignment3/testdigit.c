#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int main()
{
	char *str = "182487aaaa l\0";
	char current_character;
 	/* While current_character isn't null */
 	while(current_character = *str)
 	{
  		if(isdigit(current_character))
  		{
   			printf("d");
  		}
  		else
  		{
			printf("c");
   			++str;
  		}
 	}
 	return 1;
}
