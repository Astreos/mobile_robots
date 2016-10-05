#include "RobotSensors.hh"
#include "CppInterface.hh"
#include "user_model.h"
#include "Robot.hh"

/*! \brief constructor
 * 
 * \param[in] mbs_data Robotran structure
 * \param[in] robot_id ID of the robot
 * \param[in] RW_id right wheel ID
 * \param[in] LW_id left wheel ID
 */
RobotSensors::RobotSensors(MbsData *mbs_data, int robot_id, int RW_id, int LW_id)
{
	this->mbs_data = mbs_data;
	this->robot_id = robot_id;

	timing  = new TimeSensor(mbs_data);
	wheels  = new WheelsSensor(mbs_data, RW_id, LW_id);
	tower   = new TowerSensor(mbs_data, robot_id);
	target  = new TargetSensor(mbs_data, robot_id);
	color   = new ColorSensor(mbs_data, robot_id);

	for(int i=0; i<NB_U_SWITCH; i++)
	{
		switch (i)
		{
			case R_ID:
				u_switch_list.push_back(new USwitchSensor(mbs_data, robot_id, -0.06-0.005, -0.09));
				break;

			case L_ID:
				u_switch_list.push_back(new USwitchSensor(mbs_data, robot_id, -0.06-0.005, 0.09));
				break;
		
			default:
				break;
		}
	}

	init_flag = 0;
}

/*! \brief destructor
 */
RobotSensors::~RobotSensors()
{
	for(unsigned int i=0; i<u_switch_list.size(); i++)
	{
		delete u_switch_list[i];
	}

	delete color;
	delete target;
	delete tower;
	delete wheels;
	delete timing;
}

/*! \brief update all sensors values
 */
void RobotSensors::update()
{
	// time
	timing->update();

	// wheels
	wheels->update();

	// tower
	tower->update();

	// targets
	target->update();

	// colors
	color->update();

	// micro-switches
	for(unsigned int i=0; i<u_switch_list.size(); i++)
	{
		u_switch_list[i]->update();
	}
}

/*! \brief fill the controller inputs
 * 
 * \param[out] inputs robot input structure
 */
void RobotSensors::fill_inputs(CtrlIn *inputs)
{
	double cur_tower, cur_gamma;
	CppInterface *cpp_interface;

	// initialization
	if (!init_flag)
	{
		init_flag = 1;

		cpp_interface = static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);
		rob = cpp_interface->get_global_gestion()->get_list_elem()->get_list_phys_elem()->get_robot(robot_id);
	}

	// time
	inputs->t = timing->get_t();

	// wheels speed
	inputs->r_wheel_speed = wheels->get_r_wheel_sp();
	inputs->l_wheel_speed = wheels->get_l_wheel_sp();

	// u-switches
	inputs->u_switch[R_ID] = u_switch_list[R_ID]->get_u_switch_flag();
	inputs->u_switch[L_ID] = u_switch_list[L_ID]->get_u_switch_flag();

	// tower
	for(int i=0; i<NB_STORE_EDGE; i++)
	{
		inputs->last_rising[i]  = tower->get_robot_input().last_rising_noise[i];
		inputs->last_falling[i] = tower->get_robot_input().last_falling_noise[i];

		inputs->last_rising_fixed[i]  = tower->get_fixed_input().last_rising_noise[i];
		inputs->last_falling_fixed[i] = tower->get_fixed_input().last_falling_noise[i];
	}

	inputs->rising_index  = tower->get_robot_input().rising_index;
	inputs->falling_index = tower->get_robot_input().falling_index;

	inputs->nb_rising  = tower->get_robot_input().nb_rising;
	inputs->nb_falling = tower->get_robot_input().nb_falling;

	inputs->rising_index_fixed  = tower->get_fixed_input().rising_index;
	inputs->falling_index_fixed = tower->get_fixed_input().falling_index;

	inputs->nb_rising_fixed  = tower->get_fixed_input().nb_rising;
	inputs->nb_falling_fixed = tower->get_fixed_input().nb_falling;

	inputs->tower_pos = tower->get_tower_gamma();

	// targets
	inputs->target_detected = target->get_flag_detect();
	inputs->nb_targets = rob->get_nb_targets();

	// colors
	inputs->color_seen = color->get_color_seen();
}
