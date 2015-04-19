#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int thd_var[10];
pthread_mutex_t mutex;

void *runner(void *param); /*the thread */


int main()
{
	pthread_t tid[10];
	pthread_attr_t attr;

	int i;
	if (pthread_mutex_init(&mutex,NULL) != 0)
	{
		fprintf(stderr,"Unable to initialize mutex\n");
		return 1;
	}

	pthread_mutex_init(&attr);

	for (i = 0;i<10;i++)
	{
		thd_var[i] = i;
		pthread_create(&tid[i],&attr,runner,(void *) &thd_var[i]);
	}

	for (i = 0;i<10;i++)
	{
		pthread_join(tid[i],NULL);
	}

	pthread_mutex_destroy(&mutex);

	printf("Finish");
}

/* Thread will begin control in this function */
void *runner(void *param)
{
	int *p;
	FILE *f;

	p = (int *) param;
}