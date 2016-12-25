/*! 
 * \author Nicolas Van der Noot
 * \file user_realtime_plot.c
 * \brief Configure the SDL screen to plot simulations values
 *
 * In order to use C++ features, you just need to change the extension of this file (.c) to .cc (or .cpp).
 */

#ifdef SDL

#include "user_realtime.h"

/*! \brief assign values for the SDL functions
 *
 * \param[in] mbs_data Robotran main structure
 *
 * To plot a curve, use 'set_plot(value, label)' where 'value' is the value you want to plot
 *     and 'label' is a string corresponding to the curve label. You can use the 'set_plot' function
 *     in the following function ('user_realtime_plot') or anywhere in your code, provided you add
 *     the include '#include "user_realtime.h"' in the corresponding file. Using 'user_realtime_plot' is
 *     still useful to structure the code, avoiding to put the 'set_plot' function everywhere in the code.
 *     However, using 'set_plot' in other files can be faster and is especially relevant fot C++ code
 *     where some internal variables are private and cannot be used outside the corresponding class.
 *     To plot different curves, you must use different labels (otherwise, some curves won't be plotted).
 *
 * example:
 *   set_plot(mbs_data->q[4], "q4 [rad]");
 */
void user_realtime_plot(MbsData* mbs_data)
{

}

#endif
