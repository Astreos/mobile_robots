/*! 
 * \author Nicolas Van der Noot
 * \file realtime.h
 * \brief Real-time constraints main header
 */

#ifdef REAL_TIME

#ifndef _REAL_TIME_H_
#define _REAL_TIME_H_

#include "realtime_ext.h"
#include "color_sdl.h"
#include "mbs_data.h"

#include "mbsysc_realtime_export.h"
#include "mbs_project_fct_ptr.h"

// back in visualization period
#define PERIOD_BACK_IN_VISU 0.03 // [s]

// -- Structures -- //

/*! \brief external variables and stuctures
 */
typedef struct Realtime_extern
{
    #ifdef SDL
    Realtime_sdl *sdl; ///< SDL structure
    #endif

    #ifdef JAVA
    Realtime_java *java; ///< Java structure
    #endif

    MbsData* mbs_data; ///< Robotran main structure
    
} Realtime_extern;

/*! \brief user real-time option
 */
struct Realtime_option
{
    // simulation
    double t0;  ///< initial simulation time [s]
    double tf;  ///< final simulation time [s]
    double dt0; ///< simulation time step [s]

    // general
    int init_break;  ///< 1 to start with a break, 0 otherwise
    int final_break; ///< 1 to finish with a break, 0 otherwise

    int buffer_size; ///< size of the buffer for java and sdl

    double init_speed_factor; ///< initial speed factor

    // SDL plots and interactions
    int flag_plot; ///< 1 if SDL plots features activated, 0 otherwise

    int curve_width;   ///< plot curve width in number of pixels
    int screen_width;  ///< plot screen width in number of pixels
    int screen_height; ///< plot screen height in number of pixels

    int max_nb_curves;  ///< maximum number of curves plotted
    int max_nb_legends; ///< maximum number of curves legends indicated

    double y_min_init; ///< initial lower bound for the y values plotted
    double y_max_init; ///< initial upper bound for the y values plotted

    int color_tab[NB_COLORS_SDL]; ///< colors of the curves

    double fqc_screen; ///< frequence of the SDL plots refreshment [Hz]

    // Java visualization
    int flag_visu; ///< 1 if Java visualization features activated, 0 otherwise

    int nb_models;  ///< number of models to load
    int *nb_q;       ///< number of joints in the .mbs used for visualization (for each model)
    char **mbs_file; ///< path and file name fot the .mbs file used for Java visualization (for each model)

    int start_viewpoint; ///< initial visu viewpoint ID

    double fqc_visu; ///< frequence of the java visualization refreshment [Hz]

};

/*! \brief one constraint strcuture
 */
typedef struct Realtime_constraint
{
    int delta_t_usec; ///< delta time between to gates for real time [us]
    int next_t_usec;  ///< next gate to wait for real time [us]
    double next_tsim; ///< next gate to wait for simulation time [s]

} Realtime_constraint;

/*! \brief real-time structure
 */
struct Simu_realtime
{
    // flags
    int simu_quit;       ///< 1 to quit the simulation, 0 otherwise
    int simu_break;      ///< 1 to make a break, 0 otherwise
    int last_break;      ///< 1 to indicate last break, 0 otherwise
    int simu_speed_flag; ///< used to compute the simulation factor
    int no_additional_constraint; ///< 1 if additional constraint, 0 otherwise

    // speed factors
    double real_simu_speed_factor;      ///< real speed factor
    double last_real_simu_speed_factor; ///< last real speed factor

    // time
    double last_tsim; ///< last simulation time [s]
    double tsim;      ///< current simulation time [s]

    double t0; ///< initial simulation time [s]
    double tf; ///< final simulation time [s]

    int buffer_size; ///< size of the buffer for java and sdl

    // initial time
    int init_t_sec;  ///< initial real time [s], without us
    int init_t_usec; ///< initial real time [us], without s (kind of modulo)

    // last time
    int    speed_last_t_usec; ///< last real time [us], to compute the simulation speed
    double speed_last_tsim;   ///< last simulation time [s], to compute the simulation speed

    // constraints
    int nb_constraints; ///< number of real-time constraints

    // gates
    int next_t_usec_gate;  ///< next gate to wait for real time [us]
    double next_tsim_gate; ///< next gate to wait for simulation time [s]

    int flag_plot; ///< 1 if SDL plots features activated, 0 otherwise
    int flag_visu; ///< 1 if Java visualization features activated, 0 otherwise

    int first_break; ///< 1 if during the first break, 0 otherwise

    Realtime_constraint **constraints; ///< structures of multiple real-time constraints

    Realtime_extern *ext; ///< external variables

    Realtime_option *options; ///< user options

};

// -- Functions prototypes -- //

#ifdef __cplusplus
extern "C" {
#endif
    MBSYSC_REALTIME_EXPORT void mbs_realtime_init(MbsData* mbs_data, double t0, double tf, double dt0);
    MBSYSC_REALTIME_EXPORT void mbs_realtime_loop(Simu_realtime *realtime, double tsim);
    MBSYSC_REALTIME_EXPORT void mbs_realtime_update(Simu_realtime *realtime, double tsim);
    MBSYSC_REALTIME_EXPORT void mbs_realtime_finish(Simu_realtime *realtime);
#ifdef __cplusplus
}
#endif

Simu_realtime* init_realtime(MbsData* mbs_data, Realtime_option *options);
Simu_realtime* init_simu_realtime(MbsData* mbs_data, Realtime_option *options, int nb_constraints, double *fqc_tab);
Realtime_constraint* init_realtime_constraint(int delta_t_usec, int cur_t_usec, double tsim);
void free_realtime_constraint(Realtime_constraint* constraint);
void free_simu_realtime(Simu_realtime *realtime);
void update_simu_realtime(Simu_realtime *realtime);
void update_realtime_constraint(Realtime_constraint *constraint, int simu_speed_flag);
void update_realtime_constraints_break(Simu_realtime *realtime, int delta_break_u_sec);
double get_simu_speed_factor(int simu_speed_flag);
int get_simu_speed_flag(double simu_speed_factor);

Realtime_option* init_Realtime_option(MbsData *mbs_data);
void free_Realtime_option(Realtime_option *option);
void check_user_realtime_options(Realtime_option *options);
void compute_buffer_size(Realtime_option *option);

#ifdef __cplusplus
extern "C" {
#endif
    PROJECT_USERFCT_EXPORT void user_realtime_options(MbsData* mbs_data, Realtime_option *options);
#ifdef __cplusplus
}
#endif

#endif
#endif
