#include <pthread.h>
#include <stdio.h>

int sum;

void *runner(void *param); /* the thread */

int main(int argc, char *argv[])
{
	pthread_t tid; /* the thread identifier */
	pthread_attr_t attr; /*set of attributes for the thread */
	
	/* Error Handling */
		
	/* get the default attributes */
	pthread_attr_init(&attr);

	/* create the thread */
	
	/* tid,attr,runner, argv[1] */
	pthread_create(&tid,&attr,runner,argv[1]);
	
	/* now wait for the thread to exit */
	/* join when finished */
	pthread_join(tid,NULL);

	/* results */
	printf("\n\nsum = %d\n\n",sum);
}

/* The thread will begin control in this function */
void * runner (void * param)
{
	int i, upper = atoi( (char *) param);

	sum =  0;

	if (upper >0)
	{
		/* inside */
		for (i = 1; i<=upper; i++)
			sum += i;
	}

	pthread_exit((void *) 0);
}


