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
int rdm_req(const int min, const int max);
int* request_generator(int r,int which,int p,int proc[p][r]);