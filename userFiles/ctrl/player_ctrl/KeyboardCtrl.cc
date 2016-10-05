#include "KeyboardCtrl.hh"

int KeyboardCtrl::nb_keyboard_ctrl = 0;

/// constructor
KeyboardCtrl::KeyboardCtrl(int robot_id, int nb_opp): PlayerCtrl(robot_id, nb_opp)
{
	nb_keyboard_ctrl++;

	switch (nb_keyboard_ctrl)
	{
		case 1:
			id_keyboard = 0;
			break;

		case 2:
			id_keyboard = 1;
			break;
	
		default:
			id_keyboard = -1;
			break;
	}
}

/// destructor
KeyboardCtrl::~KeyboardCtrl()
{

}

/*! \brief controller loop
 */
void KeyboardCtrl::loop_controller()
{
	// motion
	for(int i=0; i<2; i++)
	{
		if (id_keyboard >= 0)
		{
			input_command[i] = inputs->keyboard_arrow[id_keyboard][i] * 0.01;
		}
		else
		{
			input_command[i] = 0.0;
		}
	}
		
	set_wheel_commands();

	// release targets
	if (id_keyboard >= 0)
	{
		outputs->flag_release = inputs->keyboard_key[id_keyboard];
	}
}
