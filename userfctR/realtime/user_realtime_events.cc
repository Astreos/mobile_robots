/*! 
 * \author Nicolas Van der Noot
 * \file user_realtime_events.c
 * \brief Handles the events coming from the keyboard or from other sources via the SDL framework
 *
 * In order to use C++ features, you just need to change the extension of this file (.c) to .cc (or .cpp).
 */
 
#ifdef SDL

#include "realtime.h"
#include "events_sdl.h"
#include "user_realtime.h"

#include "ctrl_io.h"
#include "CppInterface.hh"
#include "user_model.h"
#include "ListElements.hh"
#include "ListPhysicElements.hh"
#include "Robot.hh"

inline int limit_int(int x, int min, int max) { return (x < min) ? min : (x > max) ? max : x; }

#define MIN_KEYBOARD -100 
#define MAX_KEYBOARD  100

#define KEYBOARD_COMMAND 75

/*! \brief handle inputs comming from the keyboard
 * 
 * \param[in,out] mbs_data Robotran main structure
 * \param[in] realtime real-time structure
 * \param[in] cur_t_usec curent time [us]
 * \param[in] keystates state of the keys (from the keyboard)
 *
 * Use keystates['key code'] (see https://wiki.libsdl.org/SDL_Scancode)
 * to detect when the corresponding key is pressed and add your own functions
 * according to the key pressed.
 *
 * Call 'wait_key(realtime, cur_t_usec, time);' in the statement where you replace 'time'
 * by a time in seconds if you want to wait before detecting a new user command.
 * Pay attention, when you call 'wait_key', the program is automatically kept awake
 * for a few seconds. Consequently, the process can not be realeased after a few seconds
 * during a break in case 'wait_key' is always called.
 *
 * example:
 *      if (keystates[SDL_SCANCODE_UP]) 
 *      {
 *          mbs_data->user_IO->my_command++;
 *          wait_key(realtime, cur_t_usec, 0.1);
 *      }
 *      else if (keystates[SDL_SCANCODE_DOWN]) 
 *      {
 *          mbs_data->user_IO->my_command--;
 *          wait_key(realtime, cur_t_usec, 0.1);
 *      }
 */
void user_keyboard(MbsData* mbs_data, Simu_realtime *realtime, int cur_t_usec, const Uint8 *keystates)
{
	int *arrow_a, *arrow_b;
	CppInterface *cpp_interface;
	ListElements *list_elem;
	ListPhysicElements *list_phys_elem;
	CtrlIn *cur_inputs;

	cpp_interface = static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);

	list_elem = cpp_interface->get_global_gestion()->get_list_elem();
	list_phys_elem = list_elem->get_list_phys_elem();

	for(int i=0; i<list_phys_elem->get_nb_robot(); i++)
	{
		cur_inputs = list_phys_elem->get_robot(i)->get_ctrl()->get_inputs();

		arrow_a = cur_inputs->keyboard_arrow[0];
		arrow_b = cur_inputs->keyboard_arrow[1];


		// -- First keyboard -- //

		if (keystates[SDL_SCANCODE_UP]) // Up arrow: increase speed
		{
			arrow_a[0] = KEYBOARD_COMMAND;
		}
		else if (keystates[SDL_SCANCODE_DOWN]) // Down arrow: decrease speed
		{
			arrow_a[0] = -KEYBOARD_COMMAND;
		}
		else
		{
			arrow_a[0] = 0;
		}

		if (keystates[SDL_SCANCODE_LEFT]) // Left arrow: increase left turn
		{
			arrow_a[1] = KEYBOARD_COMMAND;
		}
		else if (keystates[SDL_SCANCODE_RIGHT]) // Right arrow: increase right turn
		{
			arrow_a[1] = -KEYBOARD_COMMAND;
		}
		else
		{
			arrow_a[1] = 0;
		}


		// -- Second keyboard -- //

		if (keystates[SDL_SCANCODE_W] || keystates[SDL_SCANCODE_Z]) // W or Z: increase speed
		{
			arrow_b[0] = KEYBOARD_COMMAND;
		}
		else if (keystates[SDL_SCANCODE_S]) // S: decrease speed
		{
			arrow_b[0] = -KEYBOARD_COMMAND;
		}
		else
		{
			arrow_b[0] = 0;
		}

		if (keystates[SDL_SCANCODE_A] || keystates[SDL_SCANCODE_Q]) // A or Q: increase left turn
		{
			arrow_b[1] = KEYBOARD_COMMAND;
		}
		else if (keystates[SDL_SCANCODE_D]) // D: increase right turn
		{
			arrow_b[1] = -KEYBOARD_COMMAND;
		}
		else
		{
			arrow_b[1] = 0;
		}


		// limiting the keyboard inputs
		for(int j=0; j<2; j++)
		{
			arrow_a[j] = limit_int(arrow_a[j], MIN_KEYBOARD, MAX_KEYBOARD);
			arrow_b[j] = limit_int(arrow_b[j], MIN_KEYBOARD, MAX_KEYBOARD);
		}

		// keys
		cur_inputs->keyboard_key[0] = keystates[SDL_SCANCODE_SPACE];
		cur_inputs->keyboard_key[1] = keystates[SDL_SCANCODE_RETURN];
	}
}

/*! \brief handle inputs comming from joysticks axes
 * 
 * \param[in,out] mbs_data Robotran main structure
 * \param[in] realtime real-time structure
 * \param[in] nb_joysticks number of joysticks detected
 *
 * Use get_Joystick_axis(int joystickID, int axisID, Simu_realtime *realtime)
 * to return the value associated with the joystick number joystickID
 *
 * joystickID: ID of the joystick (starting at 0)
 * axisID: ID of the axis (starting at 0)
 *
 * get_Joystick_axis returns a value in [-1 ; 1] or -10 if this joystickID is not available
 *
 * plugged joysticks are automatically detected at launch (see nb_joysticks)
 *
 * example:
 *     joystick_val = get_Joystick_axis(0, 0, realtime);
 *     mbs_data->user_IO->my_variable = joystick_val * scaling_factor;
 */
void user_joystick_axes(MbsData* mbs_data, Simu_realtime *realtime, int nb_joysticks)
{
	double cur_val;

	CppInterface *cpp_interface;
	ListPhysicElements *list_phys_elem;
	CtrlIn *cur_inputs;

	cpp_interface = static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);

	list_phys_elem = cpp_interface->get_global_gestion()->get_list_elem()->get_list_phys_elem();


	for(int k=0; k<list_phys_elem->get_nb_robot(); k++)
	{
		cur_inputs = list_phys_elem->get_robot(k)->get_ctrl()->get_inputs();

		for(int i=0; i<4; i++)
		{
			for(int j=0; j<2; j++)
			{
				cur_val = get_Joystick_axis(i, j, realtime);

				cur_inputs->joystick_handle[i][j] = (cur_val < -2.0) ? 0.0 : cur_val;
			}
		}
	}	
}

/*! \brief handle inputs comming from joysticks buttons
 * 
 * \param[in,out] mbs_data Robotran main structure
 * \param[in] buttonID ID of the joystick button
 *
 * buttonID takes a value (usually starting at 0) when a joystick button is pressed
 *
 * Use printf to detect which 'buttonID' is used for the different buttons.
 *
 * example:
 *      if (buttonID == 2) 
 *      {
 *          mbs_data->user_IO->my_command++;
 *      }
 */
void user_joystick_buttons(MbsData* mbs_data, int buttonID)
{
	CppInterface *cpp_interface;
	ListPhysicElements *list_phys_elem;
	CtrlIn *cur_inputs;

	cpp_interface = static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);

	list_phys_elem = cpp_interface->get_global_gestion()->get_list_elem()->get_list_phys_elem();

	for(int k=0; k<list_phys_elem->get_nb_robot(); k++)
	{
		cur_inputs = list_phys_elem->get_robot(k)->get_ctrl()->get_inputs();

		if ((buttonID >= 0) && (buttonID <= 3))
		{
			cur_inputs->joystick_button[buttonID] = 1;
		}
	}
}

#endif
