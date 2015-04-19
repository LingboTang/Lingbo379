#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


/* Why Synchronization */
// Processes executing concurrently may be interrupted at any time
// May leave shared data in inconsistent state
// Process synchronization is necessary to maintain data consistency

void Producer_Consumer()
{
	/* Producer */
	while (true)
	{
		while (counter == BUFFER_SIZE)
			;/* do nothing */
		buffer[in] = next_produced;
		in = (in+1) % BUFFER_SIZE;
		counter ++;
	}

	/* Consumer */
	while(true)
	{
		while (counter == 0)
			; /* do nothing */
		next_consumed = buffer[out];
		out = (out+1)%BUFFER_SIZE;
		counter--;
	}
}

/* Race Condition */

/* counter ++ could be implemented as */
// register1 = counter
// register1 = register1 + 1
// counter = register1

/* counter -- could be implemented as */
// register2 = counter
// register2 = register2-1
// counter = register2

/* Consider this execution interleaving with "counter = 5" initially:
	// S0 : Producer execute register1 = counter {register1 = 5}
	// S1 : Producer execute register1 = register1+1 {register1 = 6}
	// S2 : Consumer execute register2 = counter {register2 = 5}
	// S3 : Consumer execute register2 = register2 -1 {register2 = 4}
	// S4 : Producer execute counter = register1 {counter = 6}
	// S5 : Consumer execute counter = register2 {counter = 4}


/* Critical section problem */
	// Need to ensure that only one process is in critical section at
	// any moment

// General structure of process Pi

void Critical_General()
{
	do{
		//entry section
		// critical section
		//exit section
		// remainder section
	} while (true);
}

// entry section: acquires permission to enter critical section
// exit section: release the permission

// Solution to Critical-Section problem

// Must satisfy

// 1. Mutual Exclusion - only one process may be in critical section at any time
// 2. Progress - if no process is in its critical section and some processes wich
//    to enter their critical sections, then the slection of the next process allowed
//    to enter the critical section cannot be postponed indefinitely
// 3. Bounded Waiting - a bound must exist on the number of times that other processes
//	  are allowed to enter their critical sections after a process has made a request to
//    enter its critical section and before that request is granted


/* Peterson's Solution */

void Peterson()
{
	// turn indicates whose turn it is to
	// enter the critical section
	// flag[i] = 1 implies that process Pi
	// wichees to enter the critical section
	int turn;
	int flag[2] = {1,1};
	int i,j;

	do{
		flag[i] = 1;
		turn = j;
		while (flag[j] && turn == j)
			//critical section
		flag[i] = 0;
			//remainder section
	}while (true)
}

/* Synchronization Hardware */

// Uniprocessor system may solve CSP by disabling interrupts
// when executing in CSP
	// Not efficient on multiprocessor systems
// Modern machines provide special Atomic Hardware instructions
// Solutions based on idea of protecting critical section via locks

void locks()
{
	//acquire lock
		//critical section
	// release lock
		//remainder section
}while (true)

/* Test and Set */
int test_and_set (int *target)
{
	int rv = *target;
	*target = 1;
	return rv;
}

// Shared Boolean variable lock, initialized to 0
// Solution:
void using_test_and_set(int lock)
{
	do{
		while (test_and_set(&lock))
			;

		/*critical section */
		lock = 0;

		/* remainder section*/
	}while(true)
}