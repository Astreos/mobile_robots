
#ifdef VISU_3D

#include "visu_past.h"
#include "plot_sdl.h"
#include "realtime_functions.h"
#include "user_realtime.h"
#include "time_functions.h"

#include <math.h>

/*! \brief update the buffers used to store the positions (to go back in visualization)
 *
 * \param[in,out] realtime real-time structure
 * \param[in] tsim current simulation time [s]
 */
void update_past_visu(Simu_realtime *realtime, double tsim)
{
    // variables declaration
    int i, j;
    int min_tsim_index;
    int buffer_size, nb_models;

    int *nb_q;

    double *tsim_save;
    double **cur_q;
    double ***q_save;

    Realtime_visu *visu;
    MbsData* mbs_data;

    // variables initialization
    visu     = realtime->ext->visu;
    mbs_data = realtime->ext->mbs_data;

    nb_models = visu->nb_models;

    nb_q = visu->nb_q;

    min_tsim_index = visu->min_tsim_index;

    tsim_save = visu->tsim_save;
    q_save    = visu->q_save;

    buffer_size = visu->buffer_size;

    cur_q = visu->cur_q;

    // update the time
    tsim_save[min_tsim_index] = tsim;

    user_realtime_visu(mbs_data, nb_models, nb_q, cur_q);

    // update the vectors
    for(i=0; i<nb_models; i++)
    {
        for (j=0; j<nb_q[i]; j++)
        {
            q_save[i][j][min_tsim_index] = cur_q[i][j];
        }
    }

    // update indexes
    visu->max_tsim_index = min_tsim_index;

    min_tsim_index++;

    if (min_tsim_index >= buffer_size)
    {
        min_tsim_index = 0;
        visu->flag_buffer_round = 1;
    }

    visu->min_tsim_index = min_tsim_index;

    // update min-max
    visu->min_tsim = tsim_save[min_tsim_index];
    visu->max_tsim = tsim;
}

/*! \brief update the requested q past visualization vector
 *
 * \param[in,out] realtime real-time structure
 */
void q_past_vector(Simu_realtime *realtime)
{
    // variables declaration
    int i, j;
    int nb_models;
    int prev_ind, next_ind;

    double tsim_prev, tsim_next;
    double request_tsim;
    double weight_prev;

    double ***q_save;
    double **past_q;
    double *tsim_save;

    Realtime_visu *visu;

    // variables initialization
    visu = realtime->ext->visu;

    nb_models = visu->nb_models;

    q_save = visu->q_save;
    past_q = visu->past_q;

    tsim_save = visu->tsim_save;

    request_tsim = visu->t_visu_past; ///< requested simulation time [s]

    // get requested indexes: before and after
    guess_tsim_index(request_tsim, tsim_save, visu->min_tsim_index, visu->max_tsim_index, visu->min_tsim, visu->max_tsim, visu->buffer_size, visu->flag_buffer_round, &prev_ind, &next_ind);

    if (prev_ind == next_ind)
    {
        for(i=0; i<nb_models; i++)
        {
            for(j=0; j<visu->nb_q[i]; j++)
            {
                past_q[i][j] = q_save[i][j][prev_ind];
            }
        }
    }
    else
    {
        // time
        tsim_prev = tsim_save[prev_ind];
        tsim_next = tsim_save[next_ind];

        // weight for the linear interpolation
        if (tsim_next > tsim_prev)
        {
            weight_prev = (tsim_next - request_tsim) / (tsim_next - tsim_prev);
        }
        else
        {
            weight_prev = 0.0;
        }

        for(i=0; i<nb_models; i++)
        {
            for(j=0; j<visu->nb_q[i]; j++)
            {
                // linear interpolation
                past_q[i][j] = weight_prev * q_save[i][j][prev_ind] + (1 - weight_prev) * q_save[i][j][next_ind];
            }
        }
    }

    // black vertical line in plot
    #ifdef SDL
    if (realtime->flag_plot && realtime->simu_break)
    {
        plot_screen_sdl(realtime, visu->max_tsim, 1);
    }
    #endif
}

/*! \brief update the time for past visualization
 *
 * \param[out] realtime real-time structure
 * \param[int] tsim simulation time [s]
 */
void visu_time_past(Simu_realtime *realtime, double tsim)
{
    realtime->ext->visu->t_visu_past = tsim;
}

/*! \brief update 't_visu_part' according to the simulation speed factor
 *
 * \param[in,out] realtime real-time structure
 * \param[in] flag_forward 1 if forward in simulation, 0 fot backward
 */
void update_visu_past(Simu_realtime *realtime, int flag_forward)
{
    double simu_speed_factor;
    double min_tsim, max_tsim;
    double delta_t_visu;
    double cur_t;

    Realtime_visu *visu;

    visu = realtime->ext->visu;

    min_tsim = visu->min_tsim;
    max_tsim = visu->max_tsim;

    // simulation speed
    simu_speed_factor = get_simu_speed_factor(realtime->simu_speed_flag);

    // delta time for update
    cur_t = 1.0e-6 * t_usec(realtime->init_t_sec, realtime->init_t_usec);

    if (!visu->flag_t_last_past)
    {
        visu->flag_t_last_past = 1;
        delta_t_visu = PERIOD_BACK_IN_VISU;
    }
    else
    {
        delta_t_visu = cur_t - visu->t_last_past;

        if (delta_t_visu <= 0.0 || delta_t_visu >= INTERRUPT_BACK_IN_VISU)
        {
            delta_t_visu = PERIOD_BACK_IN_VISU;
        }
    }

    visu->t_last_past = cur_t;

    // new 't_visu_part'
    if (flag_forward)
    {
        visu->t_visu_past += simu_speed_factor * delta_t_visu;
    }
    else
    {
        visu->t_visu_past -= simu_speed_factor * delta_t_visu;
    }

    // bounds
    if (visu->t_visu_past < min_tsim)
    {
        visu->t_visu_past = min_tsim;
    }

    if (visu->t_visu_past > max_tsim)
    {
        visu->t_visu_past = max_tsim;
    }

    q_past_vector(realtime);
}

#endif
