#ifndef MEMCHUNK_H
#define MEMCHUNK_H

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <setjmp.h>

struct memchunk {
	void *start;
	usigned long length;
	int RW;
}

int getpagesize();

int get_mem_layout (struct memchunk *chunk_list, int size);

void read_fail(int signo);

void write_fail(int signo);

static sigjmp_buf env;

int writeable;
int readable;
int prevWriteable;
int prevReadable;

#endif