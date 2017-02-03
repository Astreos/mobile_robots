/*! 
 * \author Nicolas Van der Noot
 * \file void_set_plot.c
 * \brief void set_plot function in case set_plot is called while the SDL real-time modules were not activated
 */

#ifndef SDL

#include "user_realtime.h"

/*! \brief function called by the user to plot a curve (void in this case)
 * 
 * \param[in] value current value of the curve
 * \param[in] label label of the curve
 */
void set_plot(double value, char* label)
{

}

#endif
