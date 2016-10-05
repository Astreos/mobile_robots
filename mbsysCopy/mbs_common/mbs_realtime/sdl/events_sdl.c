
#ifdef SDL

#include "plot_sdl.h"
#include "realtime.h"
#include "user_realtime.h"
#include "events_sdl.h"
#include "mbs_data.h"
#include "mbs_project_interface.h"

#ifdef JAVA
    #include "visu_past.h"
#endif

// personnal joystick values
#define MIN_JOYSTICK -32768 ///< minimal joystick value
#define MAX_JOYSTICK 32767  ///< maximal joystick value
#define MEAN_JOYSTICK (0.5 * (MIN_JOYSTICK + MAX_JOYSTICK))
#define DIFF_JOYSTICK (MAX_JOYSTICK - MIN_JOYSTICK)
#define SEMI_DIFF_JOYSTICK (DIFF_JOYSTICK / 2.0)

/*! \brief handles the events (SDL)
 *
 * \param[in,out] realtime real-time structure
 * \param[in] cur_t_usec current time [s]
 */
void events_sdl(Simu_realtime *realtime, int cur_t_usec)
{
    // -- Vraiables declaration -- //

    SDL_Event event;

    Screen_sdl *screen_sdl;
    Realtime_sdl *realtime_sdl;
    MbsData* mbs_data;

    const Uint8 *keystates;

    double tsim;


    // -- Variables initialization -- //

    tsim = realtime->tsim;

    realtime_sdl = realtime->ext->sdl;
    mbs_data     = realtime->ext->mbs_data;
    screen_sdl   = (Screen_sdl*) realtime_sdl->screen_sdl;

    // -- Handles the events -- //

    // SDL events refresh
    SDL_PumpEvents();
    keystates = SDL_GetKeyboardState(NULL);

    if (cur_t_usec >= realtime_sdl->next_user_keyboard_event_usec)
    {
        // user keyboard
        user_keyboard(mbs_data, realtime, cur_t_usec, keystates);
    }

    // user joystick axes
    user_joystick_axes(mbs_data, realtime, screen_sdl->nb_joysticks);



    // get event (if any)
    if (SDL_PollEvent(&event))
    {
        // analyze the type of event
        switch (event.type)
        {
            // joystick button pressed
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
                user_joystick_buttons(mbs_data, event.jbutton.button);

                break;

            // top corner button pressed
            case SDL_QUIT:
                realtime->simu_quit  = 1;
                realtime->simu_break = 0;
                realtime->last_break = 0;
                break;

            // mouse wheel
            case SDL_MOUSEWHEEL:

                if (cur_t_usec - realtime_sdl->last_mouse_event_usec > TIME_MOUSE_DELAY)
                {
                    realtime_sdl->last_mouse_event_usec = cur_t_usec;

                    if ( (PLOT_X_START < realtime_sdl->mouse_cur_x) && (realtime_sdl->mouse_cur_x < screen_sdl->plot_x_end) &&
                         (PLOT_Y_START < realtime_sdl->mouse_cur_y) && (realtime_sdl->mouse_cur_y < screen_sdl->plot_y_end) )
                    {
                        // mouse wheel up
                        if (event.wheel.y > 0)
                        {
                            mouse_sdl(screen_sdl, realtime, tsim, 1);
                            realtime_sdl->last_action_break_usec = cur_t_usec;
                        }
                        // mouse wheel down
                        else if (event.wheel.y < 0)
                        {
                            mouse_sdl(screen_sdl, realtime, tsim, -1);
                            realtime_sdl->last_action_break_usec = cur_t_usec;
                        }
                    }
                }
                break;

            default:
                break;
        }
    }

    // generic keyboard inputs
    if (cur_t_usec >= realtime_sdl->next_generic_keyboard_event_usec)
    {
        // key: P (pause)
        if (keystates[SDL_SCANCODE_P])
        {
            if (!realtime->simu_break)
            {
                realtime->simu_break = 1; // break flag
                realtime_sdl->last_action_break_usec = cur_t_usec;
            }
            else
            {
                realtime->simu_break = 0;
            }

            wait_key_generic(realtime, cur_t_usec, 0.2);
        }

        // key M: quit the simulation
        else if (keystates[SDL_SCANCODE_M])
        {
            realtime->simu_quit  = 1; // flag to quit the simulation
            realtime->simu_break = 0;
            realtime->last_break = 0;
        }

        // key L: decrease the simulation speed
        else if (keystates[SDL_SCANCODE_L])
        {
            realtime->simu_speed_flag--;
            screen_sdl->break_plot_flag = 1;

            wait_key_generic(realtime, cur_t_usec, 0.2);
        }

        // key O: increase the simulation speed
        else if (keystates[SDL_SCANCODE_O])
        {
            realtime->simu_speed_flag++;
            screen_sdl->break_plot_flag = 1;

            wait_key_generic(realtime, cur_t_usec, 0.2);
        }

        // key K: activate / deactivate the signals auto-scaling
        else if (keystates[SDL_SCANCODE_K])
        {
            if (screen_sdl->signal_auto_scaling)
            {
                screen_sdl->signal_auto_scaling = 0;
            }
            else
            {
                screen_sdl->signal_auto_scaling = 1;
            }
            screen_sdl->break_plot_flag = 1;

            wait_key_generic(realtime, cur_t_usec, 0.2);
        }

        // key I: activate / deactivate the y axis auto-scaling
        else if (keystates[SDL_SCANCODE_I])
        {
            if (screen_sdl->plot_auto_scaling)
            {
                screen_sdl->plot_auto_scaling = 0;
            }
            else
            {
                screen_sdl->plot_auto_scaling = 1;
            }
            screen_sdl->break_plot_flag = 1;

            wait_key_generic(realtime, cur_t_usec, 0.2);
        }

        // key U: broaden x or y axis
        else if (keystates[SDL_SCANCODE_U])
        {
            if (screen_sdl->hor_plot_scaling)
            {
                screen_sdl->increase_plot_x_diff_flag = 1;
            }
            else
            {
                screen_sdl->increase_plot_y_diff_flag = 1;
                screen_sdl->plot_auto_scaling         = 0;
            }
            screen_sdl->break_plot_flag = 1;


            wait_key_generic(realtime, cur_t_usec, 0.2);
        }

        // key J: shrink x or y axis
        else if (keystates[SDL_SCANCODE_J])
        {
            if (screen_sdl->hor_plot_scaling)
            {
                screen_sdl->decrease_plot_x_diff_flag = 1;
            }
            else
            {
                screen_sdl->decrease_plot_y_diff_flag = 1;
                screen_sdl->plot_auto_scaling         = 0;
            }
            screen_sdl->break_plot_flag = 1;

            wait_key_generic(realtime, cur_t_usec, 0.2);
        }

        // key Y: shift x or y axis: up or right
        else if (keystates[SDL_SCANCODE_Y])
        {
            if (screen_sdl->hor_plot_scaling)
            {
                screen_sdl->right_plot_y_flag = 1;
            }
            else
            {
                screen_sdl->up_plot_y_flag    = 1;
                screen_sdl->plot_auto_scaling = 0;
            }
            screen_sdl->break_plot_flag = 1;

            wait_key_generic(realtime, cur_t_usec, 0.2);
        }

        // key H: shift x or y axis: bottom or left
        else if (keystates[SDL_SCANCODE_H])
        {
            if (screen_sdl->hor_plot_scaling)
            {
                screen_sdl->left_plot_y_flag = 1;
            }
            else
            {
                screen_sdl->bottom_plot_y_flag = 1;
                screen_sdl->plot_auto_scaling  = 0;
            }
            screen_sdl->break_plot_flag = 1;

            wait_key_generic(realtime, cur_t_usec, 0.2);
        }

        // key N: switch from y axis to x axis auto-scaling and vice-versa
        else if (keystates[SDL_SCANCODE_N])
        {
            if (realtime->simu_break)
            {
                if (screen_sdl->hor_plot_scaling)
                {
                    screen_sdl->hor_plot_scaling = 0;
                }
                else
                {
                    screen_sdl->hor_plot_scaling = 1;
                }
            }
            else
            {
                screen_sdl->hor_plot_scaling = 0;
            }
            screen_sdl->break_plot_flag = 1;

            wait_key_generic(realtime, cur_t_usec, 0.2);
        }

        #ifdef JAVA
        // key V: change viewpoint for Java
        else if (realtime->flag_visu && keystates[SDL_SCANCODE_V])
        {
            realtime->ext->java->change_viewpoint = 1;

            wait_key_generic(realtime, cur_t_usec, 0.2);
        }
        #endif

        // key T: go forward in simulation visualization
        else if (keystates[SDL_SCANCODE_T])
        {
            #ifdef JAVA
            if (realtime->flag_visu)
            {
                realtime->ext->java->visu_past_flag = 1;
                update_visu_past(realtime, 1);
            }
            #endif

            wait_key_generic(realtime, cur_t_usec, PERIOD_BACK_IN_VISU);
        }

        // key G: go backward in simulation visualization
        else if (keystates[SDL_SCANCODE_G])
        {
            #ifdef JAVA
            if (realtime->flag_visu)
            {
                realtime->ext->java->visu_past_flag = 1;
                update_visu_past(realtime, 0);
            }
            #endif

            wait_key_generic(realtime, cur_t_usec, PERIOD_BACK_IN_VISU);
        }
    }

    // mouse position
    SDL_GetMouseState(&(realtime_sdl->mouse_cur_x), &(realtime_sdl->mouse_cur_y));

    // left mouse button
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        // press left button
        if (!realtime_sdl->mouse_left_pressed)
        {
            realtime_sdl->mouse_left_pressed = 1;

            realtime_sdl->mouse_init_x = realtime_sdl->mouse_cur_x;
            realtime_sdl->mouse_init_y = realtime_sdl->mouse_cur_y;

            realtime_sdl->start_mouse_usec = cur_t_usec;
        }
        realtime_sdl->last_action_break_usec = cur_t_usec;
    }
    else
    {
        // release left button
        if (realtime_sdl->mouse_left_pressed)
        {
            realtime_sdl->mouse_left_pressed = 0;

            realtime_sdl->mouse_delta_x = realtime_sdl->mouse_cur_x - realtime_sdl->mouse_init_x;
            realtime_sdl->mouse_delta_y = realtime_sdl->mouse_cur_y - realtime_sdl->mouse_init_y;
            realtime_sdl->mouse_init_x  = realtime_sdl->mouse_cur_x;
            realtime_sdl->mouse_init_y  = realtime_sdl->mouse_cur_y;

            mouse_sdl(screen_sdl, realtime, tsim, 0);
        }
    }

    // right mouse button
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
    {
        // press right button
        if (!realtime_sdl->mouse_right_pressed)
        {
            realtime_sdl->mouse_right_pressed = 1;

            realtime_sdl->mouse_wheel_flag++;

            if (realtime_sdl->mouse_wheel_flag > 2)
            {
                realtime_sdl->mouse_wheel_flag = 0;
            }

            screen_sdl->change_mouse_wheel_flag = 1;
            plot_screen_sdl(realtime, tsim, 1);
        }
        realtime_sdl->last_action_break_usec = cur_t_usec;
    }
    else
    {
        // release right button
        if (realtime_sdl->mouse_right_pressed)
        {
            realtime_sdl->mouse_right_pressed = 0;
        }
    }

    /*
     * According to SDL 2.0 specifications, the lines with SDL_BUTTON(4) and SDL_BUTTON(5)
     * do not correspond to the mouse wheel motions anymore.
     * Only the lines with SDL_MOUSEWHEEL should be used.
     * Nevertheless, this is not always working with all mouses.
     */
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(4))
    {
        if (cur_t_usec - realtime_sdl->last_mouse_event_usec > TIME_MOUSE_DELAY)
        {
            realtime_sdl->last_mouse_event_usec = cur_t_usec;

            if ( (PLOT_X_START < realtime_sdl->mouse_cur_x) && (realtime_sdl->mouse_cur_x < screen_sdl->plot_x_end) &&
                 (PLOT_Y_START < realtime_sdl->mouse_cur_y) && (realtime_sdl->mouse_cur_y < screen_sdl->plot_y_end) )
            {
                // mouse wheel up
                mouse_sdl(screen_sdl, realtime, tsim, 1);
            }
            realtime_sdl->last_action_break_usec = cur_t_usec;
        }
    }
    else if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(5))
    {
        if (cur_t_usec - realtime_sdl->last_mouse_event_usec > TIME_MOUSE_DELAY)
        {
            realtime_sdl->last_mouse_event_usec = cur_t_usec;

            if ( (PLOT_X_START < realtime_sdl->mouse_cur_x) && (realtime_sdl->mouse_cur_x < screen_sdl->plot_x_end) &&
                 (PLOT_Y_START < realtime_sdl->mouse_cur_y) && (realtime_sdl->mouse_cur_y < screen_sdl->plot_y_end) )
            {
                // mouse wheel down
                mouse_sdl(screen_sdl, realtime, tsim, -1);
            }
            realtime_sdl->last_action_break_usec = cur_t_usec;
        }
    }


    // regulary (TIME_RELEASE_MOUSE_PLOT micro-seconds) release the plot when the left mouse button is pressed
    if ( realtime_sdl->mouse_left_pressed && (cur_t_usec - realtime_sdl->start_mouse_usec > TIME_RELEASE_MOUSE_PLOT) )
    {
        realtime_sdl->mouse_delta_x = realtime_sdl->mouse_cur_x - realtime_sdl->mouse_init_x;
        realtime_sdl->mouse_delta_y = realtime_sdl->mouse_cur_y - realtime_sdl->mouse_init_y;
        realtime_sdl->mouse_init_x  = realtime_sdl->mouse_cur_x;
        realtime_sdl->mouse_init_y  = realtime_sdl->mouse_cur_y;

        mouse_sdl(screen_sdl, realtime, tsim, 0);

        realtime_sdl->start_mouse_usec = cur_t_usec;

        realtime_sdl->last_action_break_usec = cur_t_usec;
    }
}

/*! \brief get the value of an axis of a joystick
 *
 * \param[in] joystickID joystick ID
 * \param[in] axisID axis ID
 * \param[in] realtime real-time structure
 * \return a value in [-1 ; 1] or -10 if this joystickID is not available
 */
double get_Joystick_axis(int joystickID, int axisID, Simu_realtime *realtime)
{
    double val;

    Screen_sdl *screen_sdl;

    screen_sdl = (Screen_sdl*) realtime->ext->sdl->screen_sdl;

    if (joystickID >= screen_sdl->nb_joysticks)
    {
        return -10.0;
    }

    val = SDL_JoystickGetAxis(screen_sdl->joysticks[joystickID],axisID);

    return (val - MEAN_JOYSTICK) / SEMI_DIFF_JOYSTICK;
}

#endif
