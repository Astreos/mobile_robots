/*! 
 * \author Nicolas Van der Noot
 * \file realtime_functions.h
 * \brief useful functions used by the real-time modules
 */

#ifdef REAL_TIME

#ifndef _REALTIME_FUNCTIONS_H_
#define _REALTIME_FUNCTIONS_H_

// get past indexes
void guess_tsim_index(double request_tsim, double *tsim_save, int min_ind, int max_ind, double min_tsim, double max_tsim, int buffer_size, int flag_round, int *prev_ind, int *next_ind);

#endif
#endif
