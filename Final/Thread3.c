#include <pthread.h>
#include <stdio.h>

struct thread_data
{
	int index,lower,upper;
};

long sum[11];

void *runner(void *param);

int main(int argc, char *argv[])
{
	pthread_i tid[11];
	pthread_attr_t attr;

	int i,n,k,no_of_threads;
	struct thread_data t[11];

	/* Error handling */

	n = atoi(argv[1]);
	k  = n/10;
	no_of_threads = (n% 10 == 0) ?10:11;

	pthread_attr_init(&attr);
	for(i = 0;i<no_of_threads;i++)
	{
		t[i].index = i;
		t[i].lower = i*k + 1;
		t[i].upper = (i == 10)? n: ((i + 1)*k);

		pthread_create(&tid[i],&attr,runner.(void *) &t[i]);
	}

	for (i = 0;i< no_of_threads;i++)
		pthread_join(tid[i],NULL);

	for (i = 0;i< no_of_threads;i++)
		sum[0] += sum[i];

	printf("\n\nsum = %ld\n\n",sum[0]);
	return 0;
}


/* Thread here */
void *runner(void *param)
{
	struct thread_data *p;
	int i,l;

	p = (struct thread_data *param);
	i = p->index;

	sum[i] = 0;
	for (l = p->lower;l <= p->upper;i++)
		sum[i] += 1;

	pthread_exit((void *) 0);

}