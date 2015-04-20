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
	}while (true);
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
	while (true);
}

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
	}while(true);
}

// Compare_and_Swap Instruction
int compare_and_swap(int *value, int expected, int new_value)
{
	int temp = *value;

	if(*value == expected)
	{
		*value = new_value;
	}
	return temp;
}

void using_compare_and_swap(int *value,int expected,int new_value,int lock)
{
	do{
		while(compare_and_swap(&lock,0,1) !=0)
			;

		/* critical section */

		lock = 0;

		/* remainder section */

	}while(true);
}

// Bounded-waiting with test_and_set
void bounded()
{
	do
	{
		waiting[i] = true;
		key = true;
		while(waiting[i] && key)
			key = test_and_set(&lock);
		wating[i] = 0;

		/* critical section */
		j = (i+1) % n;
		while((j!=i) && !waiting[j])
			j = (j+1) % n;
		if (j == i)
			lock = 0;
		else
			waiting[j] = 0;
		/* remainder section */
	}while(true);
}

// Mutex Locks
// Some software based Solutions
	// Simplest is mutex lock (mutual exclusion)
// Protect a critical section by first acquire() a lock then release() the lock
// Calls to acquire() and release() must be atomic
	//Usually implemented via Hardware atomic instructions
// But this Solution requires busy waiting
	//This mutex lock is called spinlock


void acquire()
{
	while (!available)
		; /* busy waiting */
	available = 0;
}

void release()
{
	available = 1;
}

void acquire_and release()
{
	do{
		acquire()
		/* critical section */
		release()
		/* remainder section */
	}while(true);
}

// Semaphore
// Sophisticated tool to synchronize process activities
// Semaphore S - int variable
// Can only be accessed via two atomic operations
// wait() and signal()

void wait(S){
	while (S<=0)
		; // busy wait
	S --;
}

void signal(S)
{
	S++;
}

// Counting Semaphore - int value can range over an unrestricted domain
// 		May be used to access a resource; initialized to # of instances
// Binary Semaphore - int value that can be 0 or 1
//	 	May be used for critical section problem; same as a mutex lock
// Can solve various synchronization problems
//		Consider P1 and P2 that require S1 to happen before S2
//		Create a Semaphore "synch" initialized to 0

// P1: ...
//		S1;
//		signal(synch);
//	   ...
// P2: ...
//		wait(synch);
//	   S2;
//	   ...


// Semaphore Implementation without Busy waiting
// With each semaphore there is an associated waiting queue
// Each semaphore is defined as
typedef struct {
	int value;
	struct process * list;
} semaphore;

// Two additional operations (implemented as sys calls):
//	block - place the process invoking the operation on the waiting queue of related semaphore
//  wakeup - remove one of process from the waiting queue and place it in the ready queue

void xwait(semaphore *S)
{
	S -> value--;
	if (S-> value < 0) {
		//add this process to S->list;
		block();
	}
}

void xsignal(semaphore *S)
{
	S->value++;
	if (S->value <=0)
	{
		//remove a process P from S-> list;
		wakeup(P);
	}
}

// Deadlock and Starvation
// Deadlock - two or more processes are waiting indefinitely for an
// event that can be caused by only one of the waiting processes
// Let S and Q be two semaphores initialized to 1
//        P0           P1
//       wait(S);	wait(Q);
//       wait(Q);	wait(S);
//       ...          ...
//       signal(S); signal(Q);
//       signal(Q); signal(S);
// Starvation - indefinite blocking
//	A process may never be removed from the semaphore queue(saym if
//  we use LIFO scheme)

// Dining-Philosophers problem

void philosopher()
{
	do{
		//think

		wait(chopstick[i]);
		wait(chopstick[(i+1)%5]);

		//eat
	}while(true);
}

// May create deadlock and Starvation

/* Monitors */
// Programming error in semaphore may result deadlock or Starvation
// Monitor is a more robust high-level abstracttion for synchronization
// that combine and hides

//	shared data, operations on data, synchronization with condition variables

struct monitor{
	// condition & shared variable declarations
	// function F1 (...) {...}
	// function Fn (...) {...}
	// Initialization_code (...) {...}
};

// Monitor's local variables can only be accessed by local functions
// local fucntions can only access local variables and parameters
// Only one process may be active within the Monitor at a time
// ENsures mutual exclusion

monitor sharedcounter()
{
	int counter
	function add() {counter++;}
	function sub() {counter--;}
	Initialization_code() {counter=0;}
}

// If a process calls sharedcounter.add(), then it has exclusive access to the
// Monitor until it leaves add()
//	No other process can come in, and has to wait

// condition x, y
// Two operations are allowed on a condition variable:
// x.wait() - a process that invokes the operation is suspended until another process invokes x.signal()
// x.signal() - resumes one of processes(if any) that invokes x.wait()
//				if no x.wait() on the variable, then it has no effect

monitor DiningPhilosophers
{
	enum{ THINKING;HUNGRY,EATING} state [5];
	condition self [5];

	void pickup(int i)
	{
		state[i] = HUNGRY;
		test(i);
		if (state[i]!=EATING)
			self[i].wait;
	}

	void putdown (int i)
	{
		state[i]=THINKING;
		test((i+4) %5); // test L& R neighbors
		test((i+1) %5);
	}

	void test(int i)
	{
		if ((state[(i+4)%5] != EATING) && (state[i] == HUNGRY) && (state[(i+1) % 5] == EATING))
		{
			state[i] = EATING;
			self[i].signal();
		}
	}

	Initialization_code()
	{
		for (int i = 0; i<5; i++)
		{
			state[i] = THINKING;
		}
	}
}

/* philosopher 2 wants food in thread 2 */
DiningPhilosophers.pickup(2);
eat
DiningPhilosophers.putdown(2);

/* philosopher 5 wants food in thread 5 */
DiningPhilosophers.pickup(5);
eat
DiningPhilosophers.putdown(5);

// end