
#ifdef REAL_TIME

#include "time_functions.h"

/*! \brief return the absolute time of the computer
 * 
 * \param[out] t_sec time in s
 * \param[out] t_usec time in us
 */
void time_get(int *t_sec, int *t_usec)
{
    struct timeval timeValue;

    gettimeofday( &timeValue, NULL );
    *t_sec  = (int)timeValue.tv_sec;
    *t_usec = (int)timeValue.tv_usec;
}

/*! \brief return the elapsed time (in us) since time 'init_t' (expressed in s and us)
 * 
 * \param[in] init_t_sec initial time in s
 * \param[in] init_t_usec initial time in us
 */
int t_usec(int init_t_sec, int init_t_usec)
{
    int current_t_sec, current_t_usec;

    time_get(&current_t_sec, &current_t_usec);

    return 1e6 * (current_t_sec - init_t_sec) + (current_t_usec - init_t_usec);
}

/// get current time fot Windows
#ifndef UNIX
int gettimeofday (struct timeval *tp, void *tz)
{
    struct _timeb timebuffer;
    _ftime (&timebuffer);
    tp->tv_sec  = timebuffer.time;
    tp->tv_usec = timebuffer.millitm * 1000;
    return 0;
}
#endif
#endif
