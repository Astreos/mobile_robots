/*! 
 * \author Nicolas Van der Noot
 * \file user_realtime_visu.h
 * \brief user realtime visu prototype
 */

#ifndef _USER_REALTIME_VISU_H_
#define _USER_REALTIME_VISU_H_

#include "mbsysc_realtime_export.h"

#ifdef REAL_TIME

#ifdef VISU_3D
#ifdef __cplusplus
extern "C" {
#endif
    PROJECT_USERFCT_EXPORT void user_realtime_visu(MbsData* mbs_data, int nb_models, int *nb_q, double **q_vec);
#ifdef __cplusplus
}
#endif
#endif

#endif
#endif
