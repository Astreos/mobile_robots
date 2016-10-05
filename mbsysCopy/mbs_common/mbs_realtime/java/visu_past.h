/*! 
 * \author Nicolas Van der Noot
 * \file visu_past.h
 * \brief functions used to go backward or forward in the simulation time for visualization
 */

#ifdef JAVA

#ifndef _VISU_PAST_H_
#define _VISU_PAST_H_

#include "realtime.h"

// functions prototypes
void update_past_visu(Simu_realtime *realtime, double tsim);
void q_past_vector(Simu_realtime *realtime);
void java_time_visu_past(Simu_realtime *realtime, double tsim);
void update_visu_past(Simu_realtime *realtime, int flag_forward);

#endif
#endif
