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
int rdm_num(int min, int max);
void allocation_generator(int r,int p,int proc[p][r],int allocation[p][r]);
void request_generator(int r,int which,int p,int proc[p][r],int allocation[p][r],int request[r]);
void current_Avilable(int p,int r,int Availres[r],int allocation[p][r],int current_Avail[r]);
void curr_Need(int r, int p, int proc[p][r],int allocation[p][r],int Need[p][r]);
void release(int p, int r,int which,int current_Avail[r],int allocation[p][r],int request_Table[p][r]);
int safety_Checker(int p, int r, int allocation[p][r],int current_Avail[r],int Need[p][r],int finish[p]);
void print_snapshot(int p,int r,int allocation[p][r],int request[p][r],int Avail[r],int proc[p][r],int Max[r]);
