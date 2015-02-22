#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
	printf("\nBefore fork.\n");
	fork();
	printf("\nAfter 1st fork.\n");
	fork();
	printf("\nAfter 2nd fork.\n");
	return 0;
}
