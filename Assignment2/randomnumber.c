#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
	srand(time(NULL));
	int randi = rand() % (9-0 + 1) + 0;
	if (randi == 0)
	{
		printf("\nyou got it zero!\n");
	}
	else if (randi == 1)
	{
		printf("\nyou got it one!\n");
	}
	if (randi == 2)
	{
		printf("\nyou got it two!\n");
	}
	if (randi == 3)
	{
		printf("\nyou got it three!\n");
	}
	if (randi == 4)
	{
		printf("\nyou got it four!\n");
	}
	if (randi == 5)
	{
		printf("\nyou got it five!\n");
	}
	if (randi == 6)
	{
		printf("\nyou got it six!\n");
	}
	if (randi == 7)
	{
		printf("\nyou got it seven!\n");
	}
	if (randi == 8)
	{
		printf("\nyou got it eight!\n");
	}
	if (randi == 9)
	{
		printf("\nyou got it nine!\n");
	}
	return 0;
}
