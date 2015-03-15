#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int IPtoDec(char*IPdot) {
   int a,b,c,d;
   // Scan them to the int type
   sscanf(IPdot,"%d.%d.%d.%d",&a,&b,&c,&d);
   unsigned int ip;
   // Bit shifting to the correct position
   unsigned int thisa = a<<24;
   unsigned int thisb = b<<16;
   unsigned int thisc = c<<8;
   unsigned int sum = thisa+thisb+thisc+d;
   ip = sum;
   return ip;
}

int main(int argc, char**argv)
{
	int cmpsize = atoi(argv[1]);
	int dec_t_ip = IPtoDec(argv[2]);
	int dec_p_ip = IPtoDec(argv[3]);
	int shift = 31-cmpsize;
	int t_cmp = dec_t_ip>>shift;
	int p_cmp = dec_p_ip>>shift;
	int flag;
	if (p_cmp == t_cmp)
	{
		flag = 1;
	}
	else
	{
		flag = 0;
	}
	printf("%d\n",flag);
	return 0;
}