
#ifdef REAL_TIME

#include "realtime_ext.h"
#include "realtime.h"
#include "user_realtime.h"
#include "time_functions.h"

#include <stdlib.h>

#ifdef SDL
#include "plot_sdl.h"
#include "events_sdl.h"
#endif

#ifdef JAVA
#include "java_functions.h"
#endif

#ifdef OPEN_GL
#include "open_gl_c_int.h"
#endif

#ifdef SDL

/*! \brief initialize SDL real-time variables
 *
 * \param[in] screen_sdl SDL main data structure
 * \return real-time structure for SDL
 */
Realtime_sdl* init_realtime_sdl(void *screen_sdl)
{
    // variable (pointer) to return
    Realtime_sdl *sdl;

    // memory allocation
    sdl = (Realtime_sdl*) malloc(sizeof(Realtime_sdl));

    sdl->mouse_init_x                     = 0;
    sdl->mouse_init_y                     = 0;
    sdl->mouse_delta_x                    = 0;
    sdl->mouse_delta_y                    = 0;
    sdl->mouse_cur_x                      = 0;
    sdl->mouse_cur_y                      = 0;
    sdl->mouse_wheel_flag                 = 0;
    sdl->start_mouse_usec                 = 0;
    sdl->last_action_break_usec           = 0;
    sdl->next_user_keyboard_event_usec    = 0;
    sdl->next_generic_keyboard_event_usec = 0;
    sdl->last_mouse_event_usec            = 0;
    sdl->mouse_left_pressed               = 0;
    sdl->mouse_right_pressed              = 0;

    sdl->screen_sdl = screen_sdl;

    return sdl;
}

/*! \brief free SDL real-time structure
 *
 * \param[out] sdl real-time structure for SDL
 */
void free_realtime_sdl(Realtime_sdl *sdl)
{
    free_screen_sdl(sdl->screen_sdl);
    free(sdl);
}
#endif

#ifdef VISU_3D

/*! \brief initialize 3D real-time variables
 *
 * \param[in] realtime_options options for real-time
 * \return real-time structure for 3D visualization
 */
Realtime_visu* init_realtime_visu(void *realtime_options, MbsData* mbs_data)
{
    int i, j, k;
    int buffer_size, nb_models;
    int *nb_q;

    double t0;

    Realtime_option *options;
	Realtime_visu *visu;

    // variable (pointer) to return
    options = (Realtime_option*) realtime_options;

    buffer_size = options->buffer_size;

    // memory allocation
    visu = (Realtime_visu*) malloc(sizeof(Realtime_visu));

    t0 = options->t0;

    visu->buffer_size = buffer_size;

    nb_models = options->nb_models;

    visu->nb_models = nb_models;

    // nb_q
    visu->nb_q = (int*) malloc(nb_models*sizeof(int));

    for(i=0; i<nb_models; i++)
    {
        visu->nb_q[i] = options->nb_q[i];
    }

    nb_q = visu->nb_q;

    // mbs_file
    visu->mbs_file = (char**) malloc(nb_models*sizeof(char*));

    for(i=0; i<nb_models; i++)
    {
        visu->mbs_file[i] = options->mbs_file[i];
    }

    // view point of the simulation
    visu->change_viewpoint = 1;
    visu->cur_viewpoint    = options->start_viewpoint-1;

    visu->visu_past_flag = 0;
    visu->t_visu_past    = 0.0;
    visu->t_last_past    = 0.0;

    visu->flag_t_last_past = 0;

    visu->cur_tsim = t0;

    visu->min_tsim_index = 0;
    visu->max_tsim_index = buffer_size-1;

    visu->min_tsim = t0;
    visu->max_tsim = t0 + SAFETY_TIME;

    visu->flag_buffer_round = 0;

    visu->last_past_q_flag = 0;

    visu->cur_q = (double**) malloc(nb_models*sizeof(double*));

    for(i=0; i<nb_models; i++)
    {
        visu->cur_q[i] = (double*) malloc(nb_q[i]*sizeof(double));
        for(j=0; j<nb_q[i]; j++)
        {
            visu->cur_q[i][j] = 0.0;
        }
    }

    visu->past_q = (double**) malloc(nb_models*sizeof(double*));
    for(i=0; i<nb_models; i++)
    {
        visu->past_q[i] = (double*) malloc(nb_q[i]*sizeof(double));
        for(j=0; j<nb_q[i]; j++)
        {
            visu->past_q[i][j] = 0.0;
        }
    }

    visu->anim_q = (double**) malloc(nb_models*sizeof(double*));
    for(i=0; i<nb_models; i++)
    {
        visu->anim_q[i] = (double*) malloc(nb_q[i]*sizeof(double));
        for(j=0; j<nb_q[i]; j++)
        {
            visu->anim_q[i][j] = 0.0;
        }
    }

    // full time save
    visu->tsim_save = (double*) malloc(buffer_size * sizeof(double));
    for(i=0; i<buffer_size; i++)
    {
        visu->tsim_save[i] = t0;
    }

    // full q (positions) save
    visu->q_save = (double***) malloc(nb_models * sizeof(double**));
    for(i=0; i<nb_models; i++)
    {
        visu->q_save[i] = (double**) malloc(nb_q[i] * sizeof(double*));
        for(j=0; j<nb_q[i]; j++)
        {
            visu->q_save[i][j] = (double*) malloc(buffer_size * sizeof(double));
            for(k=0; k<buffer_size; k++)
            {
                visu->q_save[i][j][k] = 0.0;
            }
        }
    }

    user_realtime_visu(mbs_data, nb_models, nb_q, visu->cur_q);

    #if (defined JAVA)
        init_jni(visu, nb_models, nb_q, visu->cur_q, options->mbs_file, options->start_viewpoint);
    #elif (defined OPEN_GL)
        init_open_gl(visu, mbs_data, nb_models, nb_q, visu->cur_q, options->mbs_file, options->start_viewpoint);
    #endif

    return visu;
}

/*! \brief free 3D real-time structure
 *
 * \param[out] visu real-time structure for 3D visualization
 */
void free_realtime_visu(Realtime_visu *visu)
{
    int i, j;

    for(i=0; i<visu->nb_models; i++)
    {
        free(visu->cur_q[i]);
        free(visu->past_q[i]);
        free(visu->anim_q[i]);
    }
    free(visu->cur_q);
    free(visu->past_q);
    free(visu->anim_q);

    for(i=0; i<visu->nb_models; i++)
    {
        for(j=0; j<visu->nb_q[i]; j++)
        {
            free(visu->q_save[i][j]);
        }
        free(visu->q_save[i]);
    }
    free(visu->q_save);

    free(visu->nb_q);
    free(visu->mbs_file);
    free(visu->tsim_save);

    #if (defined JAVA)
        free_jni(visu->visu_class);
    #elif (defined OPEN_GL)
        free_open_gl(visu->visu_class);
    #endif

    free(visu);
}
#endif

#ifdef SDL

/*! \brief hanfles the events coming from the user
 *
 * \param[in,out] realtime real-time structure
 * \param[in] tsim simulation time [s]
 */
void events_simu(Simu_realtime *realtime, double tsim)
{
    int cur_t_usec;

    cur_t_usec = t_usec(realtime->init_t_sec, realtime->init_t_usec);

    // -- handle events -- //
    events_sdl(realtime, cur_t_usec);

    // break in the simulation if needed
    if (realtime->simu_break == 1)
    {
        break_gestion(realtime, tsim);
    }
}

/*! \brief hanfles break
 *
 * \param[in,out] realtime real-time structure
 * \param[in] tsim simulation time [s]
 */
void last_break_gestion(Simu_realtime *realtime, double tsim)
{
    if (realtime->last_break)
    {
        realtime->simu_break = 1;

        events_simu(realtime, tsim);
    }
}

#endif

#endif
