/*! 
 * \author Nicolas Van der Noot
 * \file realtime_ext.h
 * \brief Real-time constraints header for external modules
 */

#ifdef REAL_TIME

#ifndef _REAL_TIME_EXT_
#define _REAL_TIME_EXT_

#include "mbs_data.h"

// -- Structures -- //

#define SAFETY_TIME 0.001 ///< safety time: difference between min and max time in case of problems [s]


/*! \brief external SDL variables and stuctures
 */
#ifdef SDL
typedef struct Realtime_sdl
{
    int mouse_init_x;  ///< initial mouse x position
    int mouse_init_y;  ///< initial mouse y position
    int mouse_delta_x; ///< change in mouse x position
    int mouse_delta_y; ///< change in mouse y position
    int mouse_cur_x;   ///< current mouse x position
    int mouse_cur_y;   ///< current mouse y position
    int mouse_wheel_flag; ///< 1 to indicate mouse change, 0 otherwise
    int start_mouse_usec; ///< initial real time to move the mouse [us]
    int last_action_break_usec;           ///< last real time action during a break [us]
    int next_user_keyboard_event_usec;    ///< next time a user event can come from the keyboard [us]
    int next_generic_keyboard_event_usec; ///< next time a generic event can come from the keyboard [us]
    int last_mouse_event_usec; ///< last time a mouse event happened [us]
    int mouse_left_pressed;    ///< 1 if mouse left button pressed, 0 otherwise
    int mouse_right_pressed;   ///< 1 if mouse right button pressed, 0 otherwise

    void *screen_sdl; ///< SDL main data structure

} Realtime_sdl;
#endif

/*! \brief external Java visualization variables and stuctures
 */
#ifdef VISU_3D
typedef struct Realtime_visu
{
    int nb_models;   ///< number of models to load
    int buffer_size; ///< size of the buffer to save past visualization
    int change_viewpoint;  ///< 1 to change the viewpoint in simulation, 0 otherwise
    int cur_viewpoint;     ///< current viewpoint in simulation
    int nb_viewpoint;      ///< number of viewpoints
    int visu_past_flag;    ///< 1 to view what happened before, 0 otherwise
    int flag_t_last_past;  ///< 1 if 't_last_past' initialized, 0 otherwise
    double t_visu_past;    ///< requested time for past visualization [s]
    double t_last_past;    ///< last time the 'update_visu_past' was called [s]
    int flag_buffer_round; ///< 1 if buffer already filled, 0 otherwise
    int last_past_q_flag;  ///< 1 if last visu was with past_q, 0 otherwise

    int *nb_q;       ///< number of joints in the .mbs used for visualization (for each model)
    char **mbs_file; ///< path and file name fot the .mbs file used for Java visualization (for each model)
    double *tsim_save; ///< vector saving the simulation time [s]
    double ***q_save;   ///< tabular saving the joints positions, according to tsim_save (for each model)
    
    int min_tsim_index; ///< index correspondinf to minimal simulation time
    int max_tsim_index; ///< index correspondinf to maximal simulation time
    double min_tsim;    ///< minimal simulation time [s]
    double max_tsim;    ///< maximal simulation time [s]

    double cur_tsim; ///< current simulation time [s]
    double **cur_q;   ///< current joints positions vector (for each model)
    double **past_q;  ///< requested past joints positions vector (for each model)
    double **anim_q;  ///< joints position for .anim file (for each model)

    void *visu_class; ///< main class or structure for visualization

} Realtime_visu;
#endif

// -- Functions prototypes -- //

#ifdef SDL
Realtime_sdl* init_realtime_sdl(void *screen_sdl);
void free_realtime_sdl(Realtime_sdl *sdl);
#endif

#ifdef VISU_3D
Realtime_visu* init_realtime_visu(void *realtime_options, MbsData* mbs_data);
void free_realtime_visu(Realtime_visu *visu);
#endif

#endif
#endif
