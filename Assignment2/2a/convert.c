#include <stdio.h>
#include <stdlib.h>
 
char *decimal_to_binary(int);
unsigned int IPtoDec(char*IPdot);
 
main()
{
   int n, c, k;
   char *pointer;
 
   printf("Enter an integer in decimal number system\n");
   scanf("%d",&n);
 
   pointer = decimal_to_binary(n);
   printf("Binary string of %d is: %s\n", n, pointer);
 
   free(pointer);
 
   return 0;
}
 
char *decimal_to_binary(int n)
{
   int c, d, count;
   char *pointer;
 
   count = 0;
   pointer = (char*)malloc(32+1);
 
   if ( pointer == NULL )
      exit(EXIT_FAILURE);
 
   for ( c = 31 ; c >= 0 ; c-- )
   {
      d = n >> c;
 
      if ( d & 1 )
         *(pointer+count) = 1 + '0';
      else
         *(pointer+count) = 0 + '0';
 
      count++;
   }
   *(pointer+count) = '\0';
 
   return  pointer;
}

// Convert the IP string to decimal
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