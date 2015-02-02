/*
 * CMPUT379 Assignment 1
 *
 * File: memchunk.h
 *
 * Author: Lingbo Tang
 *
 * Student ID: 1353070
 *
 */


/*Needed headers*/
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*Main Struct*/
struct memchunk {
	void *start;
	unsigned long length;
	int RW;
};


/*Function Prototypes*/
int get_mem_layout (struct memchunk *chunk_list, int size);
static void signal_handler (int sig);
static void handle_signals();

/*global variables*/
static char *address;
static unsigned long length;
static int readable = 0;
static int writeable= 0;
