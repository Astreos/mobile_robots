#include "Controller.hh"
#include "ColorSensor.hh"
#include <stdlib.h>

/*! \brief constructor
 * 
 * \param[in] ctrl_type controller type
 * \param[in] robot_id robot ID
 * \param[in] nb_opp number of opponents
 */
Controller::Controller(int ctrl_type, int robot_id, int nb_opp)
{
	this->ctrl_type = ctrl_type;
	this->robot_id  = robot_id;
	
	inputs  = (CtrlIn*)  new CtrlIn;
	outputs = (CtrlOut*) new CtrlOut;

	flag_ctrl = 1;

	// -- Inputs -- //

	// robot ID
	inputs->robot_id = robot_id;

	// number of opponents
	inputs->nb_opponents = nb_opp;
	
	// time
	inputs->t = 0.0;

	// wheels
	inputs->r_wheel_speed = 0.0;
	inputs->l_wheel_speed = 0.0;

	// tower
	for(int i=0; i<NB_STORE_EDGE; i++)
	{
		inputs->last_rising[i]  = 0.0;
		inputs->last_falling[i] = 0.0;
	}

	inputs->tower_pos = 0.0;

	inputs->rising_index = NB_STORE_EDGE-1;
	inputs->falling_index = NB_STORE_EDGE-1;

	inputs->nb_rising  = 0;
	inputs->nb_falling = 0;

	// tower for fixed beacons
	for(int i=0; i<NB_STORE_EDGE; i++)
	{
		inputs->last_rising_fixed[i]  = 0.0;
		inputs->last_falling_fixed[i] = 0.0;
	}

	inputs->rising_index_fixed = NB_STORE_EDGE-1;
	inputs->falling_index_fixed = NB_STORE_EDGE-1;

	inputs->nb_rising_fixed  = 0;
	inputs->nb_falling_fixed = 0;

	// micro-switch
	for(int i=0; i<NB_U_SWITCH; i++)
	{
		inputs->u_switch[i] = 0;
	}

	// targets
	inputs->nb_targets      = 0;
	inputs->target_detected = 0;

	// color seen
	inputs->color_seen = MAP_OUT_COLOR;

	// keyboard
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<2; j++)
		{
			inputs->keyboard_arrow[i][j] = 0;
		}

		inputs->keyboard_key[i] = 0;
	}

	// joystick
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<2; j++)
		{
			inputs->joystick_handle[i][j] = 0.0;
		}

		inputs->joystick_button[i] = 0;
	}


	// -- Outputs -- //

	// wheels
	for(int i=0; i<2; i++)
	{
		outputs->wheel_commands[i] = 0.0;
	}

	// tower
	outputs->tower_command = 0.0;

	// release targets
	outputs->flag_release = 0;
}

/// destructor
Controller::~Controller()
{
	delete inputs;
	delete outputs;
}
