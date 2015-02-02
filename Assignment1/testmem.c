/*
 * CMPUT379 Assignment 1
 *
 * File: testmem.c
 *
 * Author: Lingbo Tang
 *
 * Student ID: 1353070
 *
 */

#include "memchunk.h"

int main () {
	
	struct memchunk *chunklist;
	int i, chunks;
	chunklist = malloc(sizeof(struct memchunk) * 200);
	chunks = get_mem_layout(chunklist, 200);
	for (i = 0; ((i < chunks) && (i < 200)); i++) {
		

		/*Need some meaningful display of RW
		 *rather than just numbers.
		 */
		
		char display_RW;
		if (chunklist[i].RW == 0)
		{
			display_RW = 'R';
		}
		else if (chunklist[i].RW == 1)
		{
			display_RW = 'W';
		}
		else if (chunklist[i].RW != 1 && chunklist[i].RW != 0){
			display_RW = 'N';
		}
		printf("\nAddress: %p\nLength: %lu\nRW: %c\n",chunklist[i].start, chunklist[i].length,display_RW);
	}
	printf("\nTotal chunk is: %d\n", chunks);
	free(chunklist);
	return 0;
}
