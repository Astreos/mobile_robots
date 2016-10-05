/*! 
 * \author Nicolas Van der Noot
 * \file user_realtime_options.c
 * \brief let the user select real-time options different from the default ones
 */
 
#ifdef REAL_TIME

#include "realtime.h"
#include "cmake_config.h"
#include "config_functions.hh"
#include "config_file.h"

/*! \brief modify the real-time options
 * 
 * \param[in] mbs_data Robotran main structure
 * \param[out] options real-time options
 *
 * These are the options that the user can configure:
 *
 *   * Simulation options
 *      * options->t0: initial simulation time [s] (default: 0.0)
 *      * options->tf: final simulation time [s] (default: mbs_dirdyn->options->tf)
 *      * options->dt0: simulation time step [s] (default: mbs_dirdyn->options->dt0)
 *      * options->buffer_size: size of the buffer for java and sdl (default: -1).
 *                              This size corresponds to the number of time steps saved to analyze what happened 
 *                              back in time. When the buffers are full, they erase the oldest values with the new
 *                              ones (rotating buffer). If you put a value <= 0, the size is automatically computed
 *                              so that the buffer is big enough for the whole simulation.
 *      * options->init_speed_factor: initial speed factor (default: 1.0)
 *                              For speed factors equal or higher than one, this factor should be an integer.
 *                              For speed factors in ]0;1[, this factor is set as 2^x, where x is a negative integer.
 *                              So, if your factor does not match these requirements, it will be rounded to fulfill them.
 *                              Note: there is a minimal speed factor value related to the simulation time step and to
 *                              the refreshment rate of the plots and the visualization.
 *                              min speed factor = mbs_dirdyn->options->dt0 * max(options->fqc_screen, options->fqc_visu)
 *                              Modify these options for very slow motions.
 *
 *   * SDL plots options
 *      * options->flag_plot: 1 to activate SDL plots features (and keyboard/joystick interactions), 0 otherwise (default: 1)
 *      * options->curve_width: curves width in number of pixels (default: 3)
 *      * options->screen_width: plot screen width in number of pixels (default: 660)
 *      * options->screen_height: plot screen height in number of pixels (default: 520)
 *                                For 'screen_width' and 'screen_height', pay attention that the plot layout was optimized
 *                                for a size of 660x520. Consequently, layout could be deteriorated in some cases if
 *                                you modify it, especially when you decrease these values.
 *      * options->max_nb_curves: maximum number of curves plotted (default: 12)
 *      * options->max_nb_legends: maximum number of curves legends indicated (default: 6)
 *      * options->y_min_init: initial lower bound for the y values plotted (default: -1.0)
 *      * options->y_max_init: initial upper bound for the y values plotted (default: 1.0)
 *      * options->init_break: 1 to start with a break, 0 otherwise (default: 0)
 *      * options->final_break: 1 to finish with a break, 0 otherwise (default: 0)
 *      * options->color_tab[i]: colors of the curves (default: 0:BLUE_SDL, 1:RED_SDL, 2:DARK_GREEN_SDL,
 *                            3:PURPLE_SDL, 4:ORANGE_SDL, 5:LIGHT_BLUE_SDL, 6:PINK_SDL, 7:TURQUOISE_SDL,
 *                            8:LIGHT_GREEN_SDL, 9: DARK_YELLOW_SDL, 10:YELLOW_SDL, 11:GREEN_SDL).
 *                            You can modify some curve colors, replacing 'i' by a number between 0 and 11 (included).
 *                            Use the colors listed in the default values. For instance: 'options->color_tab[0] = ORANGE_SDL;'.
 *                            If you have more than 12 curves, the curve index modulo 12 is used to determine the colour.
 *      * options->fqc_screen: frequence of the SDL plots refreshment [Hz] (default: 30.0)
 *                             This frequence also determines the speed of the moving cursor.
 *
 *   * Java visualization options
 *      * options->flag_visu: 1 to activate Java visualization features, 0 otherwise (default: 1)
 *                            Activating the Java visualization features might prevent you from using a debugger.
 *                            Consequently, it is better to set this flag to 0 before entering in debugging mode.
 *                            Also, when a seg fault appears in the code and the Java visualization features are activated,
 *                            a Java error message will appear. This does not mean that the error is related to Java.
 *      * options->nb_models: number of .mbs models to load (default: 1)
 *      * options->nb_q[i]: number of joints in the .mbs files used for visualization of model i (see 'nb_models')
 *                           (default: mbs_data->njoint for nb_q[0]; 0 for the others)
 *                           'nb_q' has a size of 20. If 'nb_models' is more than 20, you have to manually deallocate and
 *                           reallocate it to a size of 'nb_models'.
 *      * options->mbs_file[i]: path and file name for the .mbs file used for Java visualization for model i
 *                           (default: .mbs file indicated in workR, in 'mbs_load', for mbs_file[0]; NULL for the others)
 *                           'mbs_file' has a size of 20. If 'nb_models' is more than 20, you have to manually deallocate and
 *                           reallocate it to a size of 'nb_models'.
 *      * options->start_viewpoint: initial visualization viewpoint ID, defined in the .mbs file (default: 0).
 *                                  Use -1 to start with the default viewpoint when opening MBSysPad.
 *      * options->fqc_visu: frequence of the java visualization refreshment [Hz] (default: 30.0)
 *
 * example:
 *      options->y_min_init =  0.0;
 *      options->y_max_init = 10.0;
 */
void user_realtime_options(MbsData* mbs_data, Realtime_option *options)
{
	FILE *config_file;

	config_file = open_config();

	#ifdef HARDCODED_CONFIG
	options->nb_q[0] = NB_JOINTS_ANIM;
	#else
	options->nb_q[0] = config_int(config_file, "nb_joints_anim");
	#endif
	options->mbs_file[0] = (char*) PROJECT_SOURCE_DIR"/../dataR/m454_project_anim.mbs";

	close_config(config_file);
}

#endif
