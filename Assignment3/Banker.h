#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

int stop_flag = 1;
void sig_handler(int sig);
int rdm_num(const int min, const int max);
void allocation_generator(int r,int p,int proc[p][r],int allocation[p][r]);
void request_generator(int r,int which,int p,int proc[p][r],int allocation[p][r],int request[r]);