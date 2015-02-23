#include <stdio.h>
#include <pthread.h>

pthread_t t_id; /*process identifier*/
long state; /*process identifier*/
unsigned int time_slice; /* scheduling information */
struct task_struct *parent; /* this process's parent */
struct list_head children; /* this process's children */
struct files_struct *files; /* list of open files */
struct mm_struct *mm; /* address space of this process */

