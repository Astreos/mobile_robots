
#ifdef REAL_TIME

/*! \brief get the indexes for a requested time on a turning vector of time values
 * 
 * \param[in] request_tsim requested simulation time [s]
 * \param[in] tsim_save vector with past simulation time values [s]
 * \param[in] min_ind index of tsim_save corresonding to the minimal time value
 * \param[in] max_ind index of tsim_save corresonding to the maximal time value
 * \param[in] min_tsim minimal time value [s]
 * \param[in] max_tsim maximal time value [s]
 * \param[in] buffer_size size of tsim_save
 * \param[in] flag_round 1 if tsim_save already filled, 0 otherwise
 * \param[out] prev_ind index before the requested time
 * \param[out] next_ind index after the requested time
 */
void guess_tsim_index(double request_tsim, double *tsim_save, int min_ind, int max_ind, double min_tsim, double max_tsim, int buffer_size, int flag_round, int *prev_ind, int *next_ind)
{
    int cur_index;

    double ratio_tsim;
    double cur_tsim;

    ratio_tsim = (request_tsim - min_tsim) / (max_tsim - min_tsim);

    // buffer not full
    if(!flag_round)
    {
        cur_index = (int) (ratio_tsim * max_ind);
    }
    else
    {
        cur_index = (int) (ratio_tsim * buffer_size + min_ind);
    }

    // safety buffer size
    if (cur_index >= buffer_size)
    {
        cur_index -= buffer_size;
    }
    else if (cur_index < 0)
    {
        cur_index += buffer_size;
    }

    // safety ratio extrema
    if ((cur_index == min_ind) && (ratio_tsim > 0.95))
    {
        cur_index = max_ind;
    }
    else if ((cur_index == max_ind) && (ratio_tsim < 0.05))
    {
        cur_index = min_ind;
    }

    // corresponding time
    cur_tsim = tsim_save[cur_index];

    if (request_tsim < cur_tsim)
    {
        while (cur_index != min_ind)
        {
            cur_index--;

            if (cur_index < 0)
            {
                if (flag_round)
                {
                    cur_index += buffer_size;
                }
                else
                {
                    *prev_ind = 0;
                    *next_ind = 0;
                    return;
                }
            }

            cur_tsim = tsim_save[cur_index];

            if (request_tsim >= cur_tsim)
            {
                break;
            }
        }
    }
    else
    {
        while (cur_index != max_ind)
        {
            cur_index++;

            if (cur_index >= buffer_size)
            {
                cur_index -= buffer_size;
            }

            cur_tsim = tsim_save[cur_index];

            if (request_tsim <= cur_tsim)
            {
                break;
            }
        }
    }

    // get previous and next indexes
    if (cur_index == min_ind)
    {
        *prev_ind = cur_index;

        if (request_tsim <= cur_tsim)
        {   
            *next_ind = cur_index;
        }
        else
        {
            cur_index++;

            if (cur_index >= buffer_size)
            {
                cur_index -= buffer_size;
            }

            *next_ind = cur_index;
        }
    }
    else if (cur_index == max_ind)
    {
        *next_ind = cur_index;

        if (request_tsim >= cur_tsim)
        {   
            *prev_ind = cur_index;
        }
        else
        {
            cur_index--;

            if (cur_index < 0)
            {
                cur_index += buffer_size;
            }

            *prev_ind = cur_index;
        }
    }
    else
    {
        if (request_tsim < cur_tsim)
        {
            *next_ind = cur_index;

            cur_index--;

            if (cur_index < 0)
            {
                cur_index += buffer_size;
            }

            *prev_ind = cur_index;
        }
        else if (request_tsim > cur_tsim)
        {
            *prev_ind = cur_index;

            cur_index++;

            if (cur_index >= buffer_size)
            {
                cur_index -= buffer_size;
            }

            *next_ind = cur_index;
        }
        else
        {
            *prev_ind = cur_index;
            *next_ind = cur_index;
        }
    }
}

#endif
