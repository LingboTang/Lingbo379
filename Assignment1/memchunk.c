/*
 * CMPUT379 Assignment 1
 *
 * File: memchunk.c
 *
 * Author: Lingbo Tang
 *
 * Student ID: 1353070
 *
 */

#include "memchunk.h"

/*Initialize signal variables*/
static sigjmp_buf env;
static struct sigaction newsa, oldsa;

/*When we met the signal, just do nothing
 *but jump to the available addres
 */

static void signal_handler (int sig) {
	siglongjmp(env, 1);
}


/* This is actually a signal blocker
 * it will block the conflict signal
 * (Old signal and new signal)
 * and handle the segfault signal
 * Basically, just save the old and
 * update to the new signal.
 * I get the reference from my
 * 201 last assignment which 
 * we need to handle the keyboard
 * signal.
 */

static void handle_signals() {
	newsa.sa_handler = signal_handler;
	sigemptyset(&newsa.sa_mask);
	newsa.sa_flags = 0;
	if (sigaction(SIGSEGV, NULL, &oldsa) == -1)
		err(1, "Can't save this");
	if (sigaction(SIGSEGV, &newsa, NULL) == -1)
		err(1, "Can't write this");
}

int get_mem_layout (struct memchunk *chunk_list, int size) {
	
	/* Instant values just used in main function
	 * Update them each time when available
	 * That's why I didn't extract them to header file
     */
	
	int counter = 0;
	int this = 0;
	int page_size = getpagesize();
	char tmp_address ='c';
	char *tmp_start = 0;
	
	/* Save the old sig Update to new one */


	handle_signals();


	/* begin looping through 32-bit memory space */
	int i;
	for (i = 0; i < (0xFFFFFFFF / page_size); i++) {
		
		/* But first we need to check if there's any
		 * jump signal.
         */
		if (!(sigsetjmp(env, 1))) 
		{
			/*Initialize the address*/

			address = (char *)(i * page_size);

			/*Then we need to check if it is readable*/
			if (readable) 
			{
				/*And we need check if the currect chunk
				 *has been readed or not. if not, write the info
				 */

				if (!this)
				{
					this = 1;
					tmp_start = address;

					/* If the counter isn't reached to the bound
					 * just let the writer to write the chunk info.
					 */
 
					if (counter < size) 
					{
						/*Change RW*/
						chunk_list[counter].start = (void*)(address);
						chunk_list[counter].RW = writeable;
					}
				}
			}

			/*If it is not readable*/

			if (!readable) 
			{
				/*And we need check if the currect chunk
				 *has been readed or not. If is, then jump.
				 */
				chunk_list[counter].RW = -1;
				if (this)
				{
					this = 0;

					/* If the counter isn't reached to the bound
					 * just get the length, and point to next linked
					 * chunk.
					 */

					if (counter < size) 
					{
						/* Get length */
						length = (address - tmp_start);
						chunk_list[counter].length = (length);
					}
					counter++;
				}
			}
			
			/*If it is readable but not writeable*/

			if (readable  && !writeable) 
			{
				/*And we need check if the currect chunk
				 *has been handled or not. If is Get the length
				 *make it writeable.
				 */
				if (this)
				{
					/*Compare to the previous one*/
					if (chunk_list[counter].RW)
					{
						if (counter < size) 
						{
							/*Get length*/
							length = (address - tmp_start);
							chunk_list[counter].length = (length);
						}
						counter++;
						tmp_start = address;
						if (counter < size) 
						{
							/* Change RW */
							chunk_list[counter].start = (void*)(address);
							chunk_list[counter].RW = writeable;
						}
					}
				}
			}

			/*If it is writeable, then we don't need to check if it is readable*/			
			
			if (writeable) 
			{
				/*But we still have to check if the current chunk
				 *has been handled or not. If is, then get the length
				 *If not, write it.
				 */

				if (this)
				{
					/*Compare to the previous one*/
					if (!chunk_list[counter].RW)
					{
						if (counter < size) 
						{
							/* Get length */
							length = (address - tmp_start);
							chunk_list[counter].length = (length);
						}
						counter++;
						tmp_start = address;
						if (counter < size) 
						{
							/* change RW */
							chunk_list[counter].start = (void*)(address);
							chunk_list[counter].RW = writeable;
						}
					}
				}
			}

			/* Change the RW
			 * By get the address
			 * dereferencing the address
			 * and write info in the address
			 * and then set the address back
			 */

			readable = 0;
			writeable = 0;
			tmp_address = *address;
			readable = 1;
			*address = tmp_address;
			writeable = 1;
		}
	}
	return counter;
}
