
#include "RobotGestion.hh"
#include "Controller.hh"
#include "Robot.hh"
#include "ctrl_io.h"
#include "CppInterface.hh"
#include "user_model.h"
#include "ActuatorRobot.hh"

/// constructor
RobotGestion::RobotGestion(ListElements *list_elem, MbsData *mbs_data, ActuatorGestion *actuators_gestion): Gestion(list_elem)
{
	this->mbs_data = mbs_data;
	this->actuators_gestion = actuators_gestion;

	list_phys_elem = list_elem->get_list_phys_elem();

	nb_robot = list_phys_elem->get_nb_robot();

	flag_init = 0;
}

/// destructor
RobotGestion::~RobotGestion()
{

}

/*! \brief computations related to the robots
 */
void RobotGestion::compute()
{
	Robot *cur_robot;
	Controller *cur_ctrl;
	
	// controllers loop
	for(int i=0; i<nb_robot; i++)
	{
		// get robot and controller
		cur_robot = list_phys_elem->get_robot(i);
		cur_ctrl  = cur_robot->get_ctrl();

		// inputs
		set_inputs(i);

		if (!flag_init)
		{
			// controller init
			cur_ctrl->init_controller();
		}

		// controller loop
		cur_ctrl->loop_controller();

		// outputs
		set_outputs(i);
	}

	flag_init = 1;
}

/*! \brief call the controller closing (last) operations
 */
void RobotGestion::close_controllers()
{
	for(int i=0; i<nb_robot; i++)
	{
		list_phys_elem->get_robot(i)->get_ctrl()->close_controller();
	}
}

/*! \brief set the robots inputs
 *
 * \param[in] robot_id ID of the robot
 */
void RobotGestion::set_inputs(int robot_id)
{
	Robot *cur_robot;
	Controller *cur_ctrl;

	CtrlIn *cur_inputs;

	// get robot and controller
	cur_robot = list_phys_elem->get_robot(robot_id);
	cur_ctrl  = cur_robot->get_ctrl();

	// get io (inputs)
	cur_inputs = cur_ctrl->get_inputs();

	// update the sensors values
	cur_robot->update_sensors();
	cur_robot->fill_inputs(cur_inputs);
}

/*! \brief set the outputs computed by the robots
 *
 * \param[in] robot_id ID of the robot
 */
void RobotGestion::set_outputs(int robot_id)
{
	Robot *cur_robot;
	Controller *cur_ctrl;

	ActuatorRobot *cur_actuators_robot;

	CtrlOut *cur_outputs;

	// get robot and controller
	cur_robot = list_phys_elem->get_robot(robot_id);
	cur_ctrl  = cur_robot->get_ctrl();

	// get actuators
	cur_actuators_robot = actuators_gestion->get_actuators_robot(robot_id);

	// get io (outputs)
	cur_outputs = cur_ctrl->get_outputs();

	// wheel motor voltages
	cur_actuators_robot->update_voltages(cur_outputs->wheel_commands[R_ID], cur_outputs->wheel_commands[L_ID], cur_outputs->tower_command);
}
