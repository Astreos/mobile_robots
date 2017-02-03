/*! 
 * \author Nicolas Van der Noot
 * \file time_functions.h
 * \brief functions used to get the time expressed in us (micro seconds)
 */

#ifdef REAL_TIME

#ifndef _TIME_FUNCTIONS_H_
#define _TIME_FUNCTIONS_H_

#include <stdlib.h>

#ifdef UNIX
    #include <sys/time.h>
#else
    #include <time.h>
    #include <sys/timeb.h>

typedef struct timeval {
	long tv_sec;
	long tv_usec;
} timeval;
#endif

// prototypes
void time_get(int *t_sec, int *t_usec);
int t_usec(int init_t_sec, int init_t_usec);

#ifndef UNIX
int gettimeofday (struct timeval *tp, void *tz);
#endif

#endif
#endif
