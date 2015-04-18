#include <pthread.h>
#include <stdio.h>

int n = 10;
long s1,s2,sum;

void *runner (void *param);


int main (void)
{
	pthread_t tid_1, tid_2;
	pthread_attr_t attr;

	int tag1,tag2;

	pthread_attr_init(&attr);

	tag1 = 0;
	pthread_create(&tid_1,&attr,runner,(void *) &tag1);

	tag2 = 1;
	pthread_create(&tid_2,&attr,runner,(void *) &tag2);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	sum = s1 + s2;
	printf("\n\nsum: %ld\n\n",sum);
	return 0;
}

void *runner(void *param)
{
	int * t,i;

	t = (int *) param;
	if (*t == 0)
	{
		/* Thread 1*/
		s1  = 0;
		for (i= 0; i<=n/2;i++)
			s1 += i;
	}
	else
	{
		/* Thread 2*/
		s2 = 0;
		for (i = n/2+1;i<n;i++)
			s2 +=i;
	}

	pthread_exit( (void *) 0);
}