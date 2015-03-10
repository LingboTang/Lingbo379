#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/* Random generator Referrence
 * http://ubuntuforums.org/showthread.php?t=1979310 
 * author: Buffay
 * Edit date: March 13th, 2012
 */


void random_payload(char * string, size_t length)
{
  /* Seed number for rand() */
  srand((unsigned int) time(0));  
   
  /* ASCII characters 33 to 126 */
  unsigned int num_chars = length - 1;
  unsigned int i;
  for (i = 0; i < num_chars; ++i)
    {
      string[i] = rand() % (126 - 33 + 1) + 33;
    }
 
  string[num_chars] = '\0';  
}

/*
 * Referrence of itoa function
 * http://stackoverflow.com/questions/9655202/how-to-convert-integer-to-string-in-c
 * author: bhuwansahni
 * Edit date: March 12,2012
 */
 
char* itoa(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

void random_IP(char * string,size_t length)
{
	srand(time(NULL));
	int r;
	//%char* newstring;
	for (int i =0;i<4; i++)
	{
		r = rand() % (255-0+1)+0;
		string = itoa(r,string);
	    //string = strcat(string,newstring);		
	}
}


int main(void)
{
  char s[20];
  random_payload(s, 20);  
  printf("%s\n", s);
  char IP[32];
  random_IP(IP, 32);
  printf("%s\n", IP);	  
  return 0;
}
