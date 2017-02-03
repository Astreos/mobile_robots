/*! 
 * \author Nicolas Van der Noot
 * \file set_plot.h
 * \brief set_plot function called by the user to plot a curve
 */

#ifndef _SET_PLOT_H_
#define _SET_PLOT_H_

#include "user_realtime.h"
#include "mbsysc_realtime_export.h"
#include "plot_sdl.h"

#ifdef SDL   
    MBSYSC_REALTIME_EXPORT void init_set_plot(Screen_sdl *screen_sdl);
    MBSYSC_REALTIME_EXPORT void free_set_plot();
#endif

#endif
