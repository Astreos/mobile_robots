
#ifdef REAL_TIME

#include "realtime.h"
#include "time_functions.h"
//#include "cmake_config.h"
#include "useful_functions.h"
#include "user_realtime.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef SDL
#include "plot_sdl.h"
#endif

#ifdef VISU_3D
#include "visu_past.h"
#endif

#ifdef JAVA
#include "java_functions.h"
#endif

#ifdef OPEN_GL
#include "open_gl_c_int.h"
#endif

// -- Macros -- //

#define USEC_TO_SEC 1.0e-6         ///< factor from us to s
#define DEFAULT_DELTA_T_USEC 1e5   ///< default time between 2 refresh moments (in us)
#define NB_REAL_TIME_CONSTRAINTS 2 ///< number of real-time constraints
#define REAL_TIME_SPEED_PERIOD_USEC 25e4 ///< real time speed factor computation time [s]
#define DEFAULT_MAX_MODELS 20 ///< default maximal numbers of models to load

/*! \brief initialize the real-time structures
 *
 * \param[in,out] mbs_data Robotran main structure
 * \param[in] t0 initial time [s]
 * \param[in] tf final time [s]
 * \param[in] dt0 intitial time step [s]
 */
void mbs_realtime_init(MbsData* mbs_data, double t0, double tf, double dt0)
{
    // -- variables declaration -- //
    Simu_realtime *realtime;
    Realtime_option *options;

    options = init_Realtime_option(mbs_data);

    // options determined by the simulation
    options->t0   = t0;
    options->tf   = tf;
    options->dt0  = dt0;

    user_realtime_options(mbs_data, options);

    check_user_realtime_options(options);
    compute_buffer_size(options);

    realtime = init_realtime(mbs_data, options);

    realtime->options = options;

    // initial plot
    #ifdef SDL
    if (options->flag_plot)
    {
        plot_screen_sdl(realtime, options->t0, 2);
    }
    #endif

    mbs_data->realtime = realtime;
}

/*! \brief update save vectors for past investigations (graphs and Java visualization)
 *
 * \param[in,out] realtime real-time structure
 * \param[in] tsim current simulation time [s]
 */
void mbs_realtime_update(Simu_realtime *realtime, double tsim)
{
    // update simulation vectors save for the plot
    #ifdef SDL
    if (realtime->flag_plot)
    {
        update_full_vectors(realtime, tsim);
    }
    #endif

    // update simulation vectors save for the Java visualization
    #ifdef VISU_3D
    if (realtime->flag_visu)
    {
        update_past_visu(realtime, tsim);
    }
    #endif
}

/*! \brief one loop iteration of the real-time process
 *
 * \param[in,out] realtime real-time structure
 * \param[in] tsim current simulation time [s]
 */
void mbs_realtime_loop(Simu_realtime *realtime, double tsim)
{
    // variables declaration
    int i;
    int cur_t_usec;
    int init_t_sec, init_t_usec;
    int speed_last_t_usec;
    int speed_new_t_usec;

    double speed_new_tsim;
    double speed_last_tsim;

    Realtime_constraint **constraints;
    MbsData* mbs_data;
			
    // variables initialization
    init_t_sec  = realtime->init_t_sec;
    init_t_usec = realtime->init_t_usec;

    speed_last_t_usec = realtime->speed_last_t_usec;
    speed_last_tsim   = realtime->speed_last_tsim;

    mbs_data = realtime->ext->mbs_data;

    cur_t_usec = t_usec(init_t_sec, init_t_usec);

    constraints = realtime->constraints;

    realtime->tsim = tsim;

    // -- Update SDL vectors to plot -- //

    #ifdef SDL
    if (realtime->flag_plot)
    {
        // assign values for the plot
        user_realtime_plot(mbs_data);

        // check maximal values for the plot
        update_y_min_max(realtime);
    }
    #endif

    #ifdef VISU_3D
    if (realtime->flag_visu)
    {
        visu_time_past(realtime, tsim);
    }
    #endif


    // -- Real time gestion -- //
						

    // update real-time constraints
    for (i=0; i<NB_REAL_TIME_CONSTRAINTS; i++)
    {
        if (tsim >= constraints[i]->next_tsim)
        {
            // plot screen sdl
            #ifdef SDL
            if (realtime->flag_plot)
            {
                if (!i)
                {
                    update_plot_vectors(realtime, tsim);
                    plot_screen_sdl(realtime, tsim, 0);
                }
            }
            #endif
			
            // 3D visualization
            #ifdef VISU_3D
            if (realtime->flag_visu)
            {
                if (i == 1)
                {
                    #if (defined JAVA)
                        update_java(realtime);
                    #elif (defined OPEN_GL)
                        update_open_gl(realtime);
                    #endif
                }
            }
            #endif

            // new real-time constraints
            update_realtime_constraint(constraints[i], realtime->simu_speed_flag);
			
        }
    }
	

    // handle events (coming from the keyboard...)
    #ifdef SDL
    if (realtime->flag_plot)
    {
        events_simu(realtime, tsim);
    }
    #endif

    // gate locked: waiting time
    if (tsim >= realtime->next_tsim_gate)
    {
        // loop in order to wait to respect the real-time constraints
        while (realtime->next_t_usec_gate > cur_t_usec)
        {
            // update real time
            cur_t_usec = t_usec(init_t_sec, init_t_usec);

            // handle events (coming from the keyboard...)
            #ifdef SDL
            if (realtime->flag_plot)
            {
                events_simu(realtime, tsim);
            }
            #endif
        }

        // in case there is no additional constraint
        if (realtime->no_additional_constraint)
        {
            update_realtime_constraint(realtime->constraints[0], realtime->simu_speed_flag);
        }

        // update real-time strcuture and related variables
        update_simu_realtime(realtime);
    }

    // computes the real time speed factor of the simulation (every REAL_TIME_SPEED_PERIOD_USEC s)
    if (cur_t_usec - speed_last_t_usec > REAL_TIME_SPEED_PERIOD_USEC)
    {
        speed_new_t_usec = cur_t_usec;
        speed_new_tsim   = tsim;

        realtime->real_simu_speed_factor = (speed_new_tsim - speed_last_tsim) / (1.0e-6 * (speed_new_t_usec - speed_last_t_usec));

        // change the plot
        #ifdef SDL
        if (realtime->flag_plot)
        {
            set_bottom_flag(realtime);
        }
        #endif

        realtime->speed_last_t_usec = speed_new_t_usec;
        realtime->speed_last_tsim   = speed_new_tsim;
    }

}

/*! \brief release memory used for the real-time related functions
 *
 * \param[out] realtime real-time structure to release memory
 */
void mbs_realtime_finish(Simu_realtime *realtime)
{
    // SDL window
    #ifdef SDL
    if (realtime->flag_plot)
    {
        last_break_gestion(realtime, realtime->tsim);
    }
    #endif

    // Real-time constraints
    free_simu_realtime(realtime);
}

/*! \brief generates a real-time options structures with the default values
 *
 * \return requested real-time options structure
 */
Realtime_option* init_Realtime_option(MbsData *mbs_data)
{
    int i;
    Realtime_option *options;

    options = (Realtime_option*) malloc(sizeof(Realtime_option));

    // simulation
    options->t0  = 0.0; ///< initial simulation time [s]
    options->tf  = 1.0; ///< final simulation time [s]
    options->dt0 = 1.0e-3; ///< simulation time step [s]

    // general
    options->init_break  = 0; ///< 1 to start with a break, 0 otherwise
    options->final_break = 0; ///< 1 to finish with a break, 0 otherwise

    options->buffer_size = -1; ///< size of the buffer for 3D visu and sdl

    options->init_speed_factor = 1.0; ///< initial speed factor

    // SDL plots and interactions
    options->flag_plot = 1; ///< 1 if SDL plots features activated, 0 otherwise

    options->curve_width   = 3;   ///< plot curve width in number of pixels
    options->screen_width  = 660; ///< plot screen width in number of pixels
    options->screen_height = 520; ///< plot screen height in number of pixels

    options->max_nb_curves  = 12; ///< maximum number of curves plotted
    options->max_nb_legends = 6;  ///< maximum number of curves legends indicated

    options->y_min_init = -1.0; ///< initial lower bound for the y values plotted
    options->y_max_init =  1.0; ///< initial upper bound for the y values plotted

    // colors of the curves
    options->color_tab[0]  = BLUE_SDL;
    options->color_tab[1]  = RED_SDL;
    options->color_tab[2]  = DARK_GREEN_SDL;
    options->color_tab[3]  = PURPLE_SDL;
    options->color_tab[4]  = ORANGE_SDL;
    options->color_tab[5]  = LIGHT_BLUE_SDL;
    options->color_tab[6]  = PINK_SDL;
    options->color_tab[7]  = TURQUOISE_SDL;
    options->color_tab[8]  = LIGHT_GREEN_SDL;
    options->color_tab[9]  = DARK_YELLOW_SDL;
    options->color_tab[10] = YELLOW_SDL;
    options->color_tab[11] = GREEN_SDL;

    options->fqc_screen = 30.0; ///< frequence of the SDL plots refreshment [Hz]

    // Java visualization
    options->flag_visu = 1; ///< 1 if Java visualization features activated, 0 otherwise

    options->nb_models = 1; ///< number of models to load

    // number of joints in the .mbs used for visualization
    options->nb_q = (int*) malloc(DEFAULT_MAX_MODELS*sizeof(int));

    options->nb_q[0] = mbs_data->njoint;

    for(i=1; i<DEFAULT_MAX_MODELS; i++)
    {
        options->nb_q[i] = 0;
    }

    // path and file name fot the .mbs file used for Java visualization
    options->mbs_file = (char**) malloc(DEFAULT_MAX_MODELS*sizeof(char*));

    options->mbs_file[0] = mbs_data->mbs_filename;

    for(i=1; i<DEFAULT_MAX_MODELS; i++)
    {
        options->mbs_file[i] = NULL;
    }

    options->start_viewpoint  = 0; ///< initial visu viewpoint ID

    options->fqc_visu = 30.0; ///< frequence of the 3D visualization refreshment [Hz]

    return options;
}

/*! \brief release memory for the options structure
 *
 * \param[out] options options structure
 */
void free_Realtime_option(Realtime_option *options)
{
    free(options->mbs_file);
    free(options->nb_q);
    free(options);
}

/*! \brief check that the modifications performed by the user on the options are correct
 *
 * \param[in,out] options options structure
 */
void check_user_realtime_options(Realtime_option *options)
{
    int i;

    // errors generated
    if (options->t0 >= options->tf)
    {
        printf("\nReal-time options error: t0 (%f) >= tf (%f).\n", options->t0, options->tf);
        exit(EXIT_FAILURE);
    }

    if (options->dt0 <= 0.0)
    {
        printf("\nReal-time options error: dt0 (%f) is not strictly positive.\n", options->dt0);
        exit(EXIT_FAILURE);
    }

    if (options->y_min_init >= options->y_max_init)
    {
        printf("\nReal-time options error: y_min_init (%f) >= y_max_init (%f).\n", options->y_min_init, options->y_max_init);
        exit(EXIT_FAILURE);
    }

    for(i=0; i<options->nb_models; i++)
    {
        if (options->nb_q[i] < 0)
        {
            printf("\nReal-time options error: nb_q[%d] (%d) negative.\n", i, options->nb_q[i]);
            exit(EXIT_FAILURE);
        }
    }

    // warnings generated
    if (options->init_speed_factor <= 0.0)
    {
        printf("\nReal-time options warning: init_speed_factor (%f) <= 0.0; set to 1.0.\n", options->init_speed_factor);
        options->init_speed_factor = 1.0;
    }

    if (options->start_viewpoint < -1)
    {
        printf("\nReal-time options warning: start_viewpoint (%d) < -1; set to 0.\n", options->start_viewpoint);
        options->start_viewpoint = 0;
    }

    if (options->curve_width < 1)
    {
        printf("\nReal-time options warning: curve_width (%d) < 1; set to 3.\n", options->curve_width);
        options->curve_width = 3;
    }

    if (options->screen_width < 200)
    {
        printf("\nReal-time options warning: screen_width (%d) < 200; set to 660.\n", options->screen_width);
        options->screen_width = 660;
    }

    if (options->screen_height < 200)
    {
        printf("\nReal-time options warning: screen_height (%d) < 200; set to 520.\n", options->screen_height);
        options->screen_height = 520;
    }

    if (options->max_nb_curves < 0)
    {
        printf("\nReal-time options warning: max_nb_curves (%d) < 0; set to 12.\n", options->max_nb_curves);
        options->max_nb_curves = 12;
    }

    if (options->max_nb_legends > options->max_nb_curves)
    {
        printf("\nReal-time options warning: max_nb_legends (%d) > max_nb_curves (%d); set to %d.\n", options->max_nb_legends, options->max_nb_curves, options->max_nb_curves);
        options->max_nb_legends = options->max_nb_curves;
    }

    if (options->fqc_visu <= 0.0)
    {
        printf("\nReal-time options warning: fqc_visu (%f) <= 0.0; set to 30.0.\n", options->fqc_visu);
        options->fqc_visu = 30.0;
    }

    if (options->fqc_screen <= 0.0)
    {
        printf("\nReal-time options warning: fqc_screen (%f) <= 0.0; set to 30.0.\n", options->fqc_screen);
        options->fqc_screen = 30.0;
    }
}

/*! \brief automatically compute the buffer size
 *
 * \param[in,out] options options structure
 */
void compute_buffer_size(Realtime_option *options)
{
    if (options->buffer_size <= 0)
    {
        options->buffer_size = ((options->tf - options->t0) / options->dt0) + 1; // size of the SDL buffer [-], if possible, equal (or higher) to the number of time steps
    }
}

/*! \brief initialize one real-time constraint
 *
 * \param[in] delta_t_usec time step for this constraint [us]
 * \param[in] cur_t_usec current time (real-time) [us]
 * \param[in] tsim current simulation time [s]
 */
Realtime_constraint* init_realtime_constraint(int delta_t_usec, int cur_t_usec, double tsim)
{
    // variable (pointer) to return
    Realtime_constraint *constraint;

    // memory allocation
    constraint = (Realtime_constraint*) malloc(sizeof(Realtime_constraint));

    // time between 2 refresh moments (in us)
    constraint->delta_t_usec = delta_t_usec;

    // next gate for this constraint
    constraint->next_t_usec  = cur_t_usec + delta_t_usec;               // real time (in us)
    constraint->next_tsim    = tsim       + delta_t_usec * USEC_TO_SEC; // simulation time (in s)

    return constraint;
}

/*! \brief initialize the real-time structure
 *
 * \param[in,out] mbs_data Robotran main structure
 * \param[in] options options structure
 * \return real-time structure initialized
 */
Simu_realtime* init_realtime(MbsData* mbs_data, Realtime_option *options)
{
    // Real-time structure
    Simu_realtime *realtime;

    // tab with the time constraints
    double fqc_tab[NB_REAL_TIME_CONSTRAINTS];

    fqc_tab[0]  = options->fqc_screen; // constraints
    fqc_tab[1]  = options->fqc_visu;
	
    realtime = init_simu_realtime(mbs_data, options, NB_REAL_TIME_CONSTRAINTS, fqc_tab);

    return realtime;
}

/*! \brief initialize the structure with all the real-time constraints
 *
 * \param[in,out] mbs_data Robotran main structure
 * \param[in] options options structure
 * \param[in] nb_constraints number of real-time constraints
 * \param[in] fqc_tab tabular with the constraints freqeuncies
 * \return real-time structure initialized
 */
Simu_realtime* init_simu_realtime(MbsData* mbs_data, Realtime_option *options, int nb_constraints, double *fqc_tab)
{
    // -- Variables decalration -- //
    int i;
    int cur_t_usec;
    int cur_next_t_usec, min_next_t_usec;
    int init_t_sec, init_t_usec;

    double cur_next_tsim, min_next_tsim;
    double tsim;

    Simu_realtime *realtime;
    Realtime_constraint **constraints;


    // -- Initialization -- //

    tsim = options->t0;

    // check the number of constraints
    if (nb_constraints < 0)
    {
        printf("Error: negative number of realtime constraints!\n");
        exit(1);
    }

    // memory allocation
    realtime = (Simu_realtime*) malloc(sizeof(Simu_realtime));

    realtime->ext = (Realtime_extern*) malloc(sizeof(Realtime_extern));

    realtime->ext->mbs_data = mbs_data;

    #ifdef VISU_3D
    if (options->flag_visu)
    {
        realtime->ext->visu = init_realtime_visu(options, mbs_data);
    }
    #else
    if (options->flag_visu)
    {
        printf("\nTo use the Java visualization (realtime->options->flag_visu activated), set the CMake flag 'FLAG_VISU' to ON !\n");
        exit(EXIT_FAILURE);
    }
    #endif

    #ifdef SDL
    if (options->flag_plot)
    {
        realtime->ext->sdl = init_realtime_sdl(init_screen_sdl(options));
    }
    #else
    if (options->flag_plot)
    {
        printf("\nTo use the real-time features (realtime->options->flag_plot activated), set the CMake flag 'FLAG_PLOT' to ON !\n");
        exit(EXIT_FAILURE);
    }
    #endif
	
    // absolute time before starting loop
    time_get(&init_t_sec, &init_t_usec);

    cur_t_usec = t_usec(init_t_sec, init_t_usec);

    // not the default time constraint
    if (nb_constraints > 0)
    {
        // flag
        realtime->no_additional_constraint = 0;

        // memory allocation: general structure
        constraints = (Realtime_constraint**) malloc(nb_constraints*sizeof(Realtime_constraint*));

        // memory allocation: each constraint (one by one)
        for (i=0; i<nb_constraints; i++)
        {
            #ifdef UNIX
            constraints[i] = init_realtime_constraint((int) round(1e6 / fqc_tab[i]), cur_t_usec, tsim);
            #else
            constraints[i] = init_realtime_constraint((int) (1e6 / fqc_tab[i]), cur_t_usec, tsim);
            #endif
        }
    }
    // default time constraint
    else
    {
        // flag
        realtime->no_additional_constraint = 1;

        // one default constraint
        nb_constraints = 1;

        // memory allocation: general structure
        constraints = (Realtime_constraint**) malloc(1*sizeof(Realtime_constraint*));

        // memory allocation: default constraint
        constraints[0] = init_realtime_constraint(DEFAULT_DELTA_T_USEC, cur_t_usec, tsim);
    }

    min_next_tsim   = 0.0;
    min_next_t_usec = 0;

    // get the first constraint (the most restrictive)
    for (i=0; i<nb_constraints; i++)
    {
        cur_next_tsim   = constraints[i]->next_tsim;
        cur_next_t_usec = constraints[i]->next_t_usec;

        if (!i)
        {
            min_next_tsim   = cur_next_tsim;
            min_next_t_usec = cur_next_t_usec;
        }
        else
        {
            if(cur_next_tsim < min_next_tsim)
            {
                min_next_tsim = cur_next_tsim;
            }
            if (cur_next_t_usec < min_next_t_usec)
            {
                min_next_t_usec = cur_next_t_usec;
            }
        }
    }

    realtime->next_tsim_gate   = min_next_tsim;
    realtime->next_t_usec_gate = min_next_t_usec;

    // number of constraints
    realtime->nb_constraints  = nb_constraints;

    // flags
    realtime->simu_quit       = 0;           // quit the simulation
    realtime->simu_break      = options->init_break;  // break in the simulation at the beginning
    realtime->last_break      = options->final_break; // break in the simulation at the end
    realtime->simu_speed_flag = get_simu_speed_flag(options->init_speed_factor); // simulation speed flag

    // end of the simulation time (in s)
    realtime->last_tsim = tsim;

    // simulation speed
    realtime->last_real_simu_speed_factor = get_simu_speed_factor(realtime->simu_speed_flag);
    realtime->real_simu_speed_factor      = realtime->last_real_simu_speed_factor;

    // constraints structure
    realtime->constraints = constraints;

    realtime->buffer_size = options->buffer_size;

    realtime->first_break = options->init_break;

    realtime->flag_plot = options->flag_plot;
    realtime->flag_visu = options->flag_visu;

    realtime->init_t_sec  = init_t_sec;
    realtime->init_t_usec = init_t_usec;

    realtime->speed_last_t_usec = t_usec(init_t_sec, init_t_usec);
    realtime->speed_last_tsim   = options->t0;

    realtime->tsim = options->t0;

    realtime->t0 = options->t0;
    realtime->tf = options->tf;

    // return the real-time structure
    return realtime;
}

/*! \brief free one real-time constraint
 *
 * \param[out] constraint constraint to release memory
 */
void free_realtime_constraint(Realtime_constraint* constraint)
{
    free(constraint);
}

/*! \brief free the structure of all the real-time constraints
 *
 * \param[out] realtime real-time structure to release memory
 */
void free_simu_realtime(Simu_realtime *realtime)
{
    int i;

    #ifdef SDL
    if (realtime->flag_plot)
    {
        free_realtime_sdl(realtime->ext->sdl);
    }
    #endif

    #ifdef VISU_3D
    if (realtime->flag_visu)
    {
        free_realtime_visu(realtime->ext->visu);
    }
    #endif

    free(realtime->ext);

    for (i=0; i<realtime->nb_constraints; i++)
    {
        free_realtime_constraint(realtime->constraints[i]);
    }

    free(realtime->constraints);

    free_Realtime_option(realtime->options);

    free(realtime);
}

/*! \brief update one real-time constraint
 *
 * \param[in] constraint real-time constraint
 * \param[in] simu_speed_flag index used to compute the speed factor
 */
void update_realtime_constraint(Realtime_constraint *constraint, int simu_speed_flag)
{
    int delta_t_usec;
    double simu_speed_factor;

    // simulation speed
    simu_speed_factor = get_simu_speed_factor(simu_speed_flag);

    // next gates
    delta_t_usec = constraint->delta_t_usec;

    constraint->next_t_usec += delta_t_usec;
    constraint->next_tsim   += simu_speed_factor * delta_t_usec * 1.0e-6;
}

/*! \brief update the structure of all the constraints
 *
 * \param[in,out] realtime real-time structure
 */
void update_simu_realtime(Simu_realtime *realtime)
{
    int i;
    int cur_next_t_usec, min_next_t_usec;
    double cur_next_tsim, min_next_tsim;

    min_next_t_usec = 0;
    min_next_tsim   = 0.0;

    // loop on all the constraints
    for (i=0; i<realtime->nb_constraints; i++)
    {
        cur_next_t_usec = realtime->constraints[i]->next_t_usec;
        cur_next_tsim   = realtime->constraints[i]->next_tsim;

        // first element is the minimum
        if (!i)
        {
            min_next_t_usec = cur_next_t_usec;
            min_next_tsim   = cur_next_tsim;
        }
        else
        {
            if(cur_next_t_usec < min_next_t_usec)
            {
                min_next_t_usec = cur_next_t_usec;
            }
            if(cur_next_tsim < min_next_tsim)
            {
                min_next_tsim = cur_next_tsim;
            }
        }
    }

    realtime->next_t_usec_gate = min_next_t_usec;
    realtime->next_tsim_gate   = min_next_tsim;
}

/*! \brief get simulation factor depending on the 'simu_speed_flag' flag
 *
 * \param[in] simu_speed_flag index used to compute the speed factor
 * \return speed factor
 */
double get_simu_speed_factor(int simu_speed_flag)
{
    if (simu_speed_flag < 0)
    {
        return pow_int(2.0,simu_speed_flag);
    }
    else if (simu_speed_flag > 0)
    {
        return simu_speed_flag + 1.0;
    }
    else
    {
        return 1.0;
    }
}

/*! \brief get the 'simu_speed_flag' flag depending on simulation factor
 *
 * \param[in] simu_speed_factor speed factor
 * \return index used to compute the speed factor
 */
int get_simu_speed_flag(double simu_speed_factor)
{
    double float_flag;

    // safety
    if (simu_speed_factor <= 0.0)
    {
        return 0;
    }

    if (simu_speed_factor >= 1.0)
    {
        float_flag = simu_speed_factor - 1.0;
    }
    else
    {
        float_flag = -(log(1.0/simu_speed_factor) / log(2.0));
    }

    // round to integer
    return ((float_flag - floor(float_flag) > 0.5) ? ceil(float_flag) : floor(float_flag));
}

/*! \brief update the real-time constraints after a break
  *
  * \param[in,out] realtime real-time structure
  * \param[in] delta_break_u_sec delta of time for next gate [us]
  */
void update_realtime_constraints_break(Simu_realtime *realtime, int delta_break_u_sec)
{
    int i;

    Realtime_constraint **constraints;

    constraints = realtime->constraints;

    // loop on all the constraints
    for (i=0; i<realtime->nb_constraints; i++)
    {
        constraints[i]->next_t_usec += delta_break_u_sec;
    }

    update_simu_realtime(realtime);
}

#endif
