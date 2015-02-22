#include <pthread.h>

int n, data[100];
long s1, s2, sum;

void *runner(void *param);

int main()
{

	pthread_t tid1,tid2;
	pthread_attr_t attr;

	int tag1, tag2;
	
	pthread_attr_init(&attr);
	
	tag1 = 0;
	pthread_create(&tid1,&attr, runner, (void*)&tag1);
	tag2 = 1;
	pthread_create(&tid2,&attr, runner. (void*)&tag2);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	sum = s1+s2;
}

void *runner(void *param)
{
	int *t,i;

	t = (int*)param;

	if(*t==0)
	{
		s1 = 0;
		for (i=0; i < n/2; i++)
			s1 += data[i];
	}
	else
	{ 
		s2 = 0;
		for (i=n+2/1;i<n;i++)
			s2 + = data[i];
	}
	
	pthread_exit((void*)0);
}

