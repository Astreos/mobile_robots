#include "JoystickCtrl.hh"

int JoystickCtrl::nb_joystick_ctrl = 0;

/// constructor
JoystickCtrl::JoystickCtrl(int robot_id, int nb_opp): PlayerCtrl(robot_id, nb_opp)
{
	nb_joystick_ctrl++;

	if ( (1 <= nb_joystick_ctrl) && (nb_joystick_ctrl <= 4) )
	{
		id_joystick = nb_joystick_ctrl - 1;
	}
	else
	{
		id_joystick = -1;
	}

	for(int i=0; i<4; i++)
	{
		flag_up[i] = 0;
		t_button_up[i] = 0.0;
	}

	max_time_up = 0.1;
}

/// destructor
JoystickCtrl::~JoystickCtrl()
{

}

/*! \brief controller loop
 */
void JoystickCtrl::loop_controller()
{
	// motion
	if (id_joystick >= 0)
	{
		input_command[0] = -inputs->joystick_handle[id_joystick][1];
		input_command[1] = -inputs->joystick_handle[id_joystick][0];
	}
	else
	{
		for(int i=0; i<2; i++)
		{
			input_command[i] = 0.0;
		}
	}
		
	set_wheel_commands();

	// release targets
	if ((id_joystick >= 0) && (id_joystick <= 3))
	{
		outputs->flag_release = inputs->joystick_button[id_joystick];

		// check that a button is not maintained up too long
		if (inputs->joystick_button[id_joystick])
		{
			if (!flag_up[id_joystick]) // rising edge
			{
				flag_up[id_joystick] = 1;
				t_button_up[id_joystick] = inputs->t;
			}
			else if (inputs->t - t_button_up[id_joystick] > max_time_up)
			{
				inputs->joystick_button[id_joystick] = 0;
				flag_up[id_joystick] = 0;
			}			
		}
	}
}
