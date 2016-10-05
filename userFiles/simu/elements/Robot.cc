
#include "Robot.hh"
#include "simu_functions.hh"
#include "robot_id.h"
#include "CppInterface.hh"
#include "user_model.h"

/*! \brief constructor
 * 
 * \param[in] geometry geometry shape
 * \param[in] mbs_data Robotran structure
 * \param[in] ctrl controller
 * \param[in] robot_id robot ID
 * \param[in] T1_id floating base T1 joint ID
 * \param[in] T2_id floating base T2 joint ID
 * \param[in] R3_id floating base R3 joint ID
 * \param[in] RW_id right wheel joint ID
 * \param[in] LW_id left wheel joint ID
 */
Robot::Robot(GeometryShape *geometry, MbsData *mbs_data, Controller *ctrl, int robot_id, int T1_id, int T2_id, int R3_id, int RW_id, int LW_id, int TW_id):
	PhysicElements(geometry)
{
	this->mbs_data = mbs_data;
	this->robot_id = robot_id;
	this->ctrl = ctrl;

	this->T1_id = T1_id;
	this->T2_id = T2_id;
	this->R3_id = R3_id;
	this->RW_id = RW_id;
	this->LW_id = LW_id;
	this->TW_id = TW_id;

	tower_dist  = 0.083;
	tower_angle = 0.0;

	update_tower();

	prev_tower_gamma = tower_gamma;
	prev_tower_theta = tower_theta;
	prev_tower_pos   = tower_pos;

	cur_t  = mbs_data->tsim;
	prev_t = cur_t;

	sensors = new RobotSensors(mbs_data, robot_id, RW_id, LW_id);

	switch (robot_id)
	{
		case ROBOT_B:
		case ROBOT_R:
			team_id = TEAM_A;
			break;

		case ROBOT_Y:
		case ROBOT_W:
			team_id = TEAM_B;
			break;

		default:
			std::cout << "Error: unknowd robot ID: " << robot_id << " !" << std::endl;
			exit(EXIT_FAILURE);
	}

	// targets
	nb_targets = 0;
	nb_max_targets = 2;

	for(int i=0; i<nb_max_targets; i++)
	{
		targets_carried.push_back(NULL);
	}

	last_target_seen = NULL;

	t_target_seen = 0.0;

	delta_t_grab = 1.5;
}

/*! \brief destructor
 */
Robot::~Robot()
{
	delete sensors;
	delete ctrl;
}

/*! \brief update the tower angle
 */
void Robot::update_tower()
{
	// save previous values for interpolation
	prev_t = cur_t;
	prev_tower_gamma = tower_gamma;
	prev_tower_theta = tower_theta;
	prev_tower_pos   = tower_pos;

	// time
	cur_t = mbs_data->tsim;

	// tower relative orientation
	tower_gamma = limit_angle(mbs_data->q[TW_id]);

	// tower absolute orientation
	tower_theta = limit_angle(get_alpha() + tower_gamma);

	// tower absolute position
	tower_pos = get_pos_dist_angle(get_pos(), tower_dist, limit_angle(get_alpha()+tower_angle));
}

/*! \brief get the absolute position from a position relative to the robot
 * 
 * \param[in] vec_rel_robot position relative to the robot
 * \return absolute position
 */
Vector3D Robot::get_abs_vector(Vector3D const& vec_rel_robot)
{
	Vector3D result;
	Vector2D vec_2D_rel, vec_2D_abs;

	GeometryShape *geom;

	geom = get_geometry();

	vec_2D_rel.set_vector(vec_rel_robot.get_x(), vec_rel_robot.get_y());

	vec_2D_abs = geom->get_pos() + get_rotated_vector(vec_2D_rel, geom->get_c_alpha(), geom->get_s_alpha());

	result.set_vector(vec_2D_abs.get_x(), vec_2D_abs.get_y(), vec_rel_robot.get_z());

	return result;
}

/*! \brief get the position relative to the robot from an absolute position
 * 
 * \param[in] vec_abs_robot absolute position
 * \return position relative to the robot
 */
Vector3D Robot::get_rel_vector(Vector3D const& vec_abs_robot)
{
	Vector3D result;
	Vector2D vec_2D_abs, vec_2D_rel;

	GeometryShape *geom;

	geom = get_geometry();

	vec_2D_abs.set_vector(vec_abs_robot.get_x(), vec_abs_robot.get_y());

	vec_2D_rel = get_rotated_vector(vec_2D_abs - geom->get_pos(), geom->get_c_alpha(), -geom->get_s_alpha());

	result.set_vector(vec_2D_rel.get_x(), vec_2D_rel.get_y(), vec_abs_robot.get_z());

	return result;
}

/*! \brief check if it is a robot
 * 
 * \param[out] robot_index index of the robot (if robot)
 * \return 1 if robot, 0 otherwise
 */
int Robot::is_robot(int &robot_index)
{
	robot_index = robot_id;

	return 1;
}

/*! \brief update the sensors values
 */
void Robot::update_sensors()
{
	// tower quantities related to 'Robot.hh'
	update_tower();

	// sensors
	sensors->update();
}

/*! \brief fill the robot input structure
 * 
 * \param[out] inputs robot input structure
 */
void Robot::fill_inputs(CtrlIn *inputs)
{
	sensors->fill_inputs(inputs);
}

/*! \brief release all targets carried
 */
void Robot::release_all_targets()
{
	ListBases *bases;
	Target *cur_target;

	// time to release
	if (get_time_gestion()->get_state() != RUN_GAME)
	{
		return;
	}

	bases = get_list_elem()->get_list_bases();

	for(int i=0; i<nb_targets; i++)
	{
		cur_target = targets_carried[i];

		if (bases->target_full_inside_any(cur_target))
		{
			get_score_gestion()->add_target(cur_target, bases);
			cur_target->set_fixed();
		}
		else
		{
			cur_target->set_ground();
		}

		targets_carried[i] = NULL;
	}

	nb_targets = 0;
}

/*! \brief grab a target
 * 
 * \param[in,out] cur_target current target to grab
 */
void Robot::grab_target(Target *cur_target)
{
	// safety
	if (nb_targets == nb_max_targets)
	{
		return;
	}
	else if (nb_targets > nb_max_targets)
	{
		std::cout << "Error: number of targets carried (" << nb_targets << ") id higher than " << nb_max_targets << "!" << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (nb_targets < 0)
	{
		std::cout << "Error: negative (" << nb_targets << ") number of targets carried !" << std::endl;
		exit(EXIT_FAILURE);
	}

	// time to grab
	if (get_time_gestion()->get_state() != RUN_GAME)
	{
		return;
	}

	targets_carried[nb_targets] = cur_target;

	cur_target->grab();

	nb_targets++;
}

/*! \brief update the target seen
 *
 * \param[in] new_target new target seen
 */
void Robot::update_target_seen(Target *new_target)
{
	double cur_t;

	cur_t = mbs_data->tsim;

	// no target currently seen
	if (new_target == NULL)
	{
		last_target_seen = NULL;

		t_target_seen = cur_t;

		return;
	}

	if (last_target_seen != new_target) // new target
	{
		last_target_seen = new_target;

		t_target_seen = cur_t;
	}
	else // same target
	{
		if (cur_t - t_target_seen > delta_t_grab) // seen the same target long enough to grab it
		{
			grab_target(new_target);
		}
	}
}

/*! \brief update all the carried targets positions
 */
void Robot::update_all_targets_position()
{
	for(int i=0; i<nb_targets; i++)
	{
		switch (i)
		{
			case 0:
				targets_carried[i]->set_pos(get_pose_x(), get_pose_y(), 0.28);
				break;

			case 1:
				targets_carried[i]->set_pos(get_pose_x(), get_pose_y(), 0.33);
				break;
		
			default:
				std::cout << "Error: unknown target carried (" << i << ") !" << std::endl;
				exit(EXIT_FAILURE);
		}
	}
}

/*! \brief get the list of elements
 *
 * \return requested list
 */
ListElements* Robot::get_list_elem()
{
	CppInterface *cpp_interface;

	cpp_interface = static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);

	return cpp_interface->get_global_gestion()->get_list_elem();
}

/*! \brief get the score gestion
 *
 * \return requested score gestion
 */
ScoreGestion* Robot::get_score_gestion()
{
	CppInterface *cpp_interface;

	cpp_interface = static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);

	return static_cast<ScoreGestion*>(cpp_interface->get_global_gestion()->get_gestion(SCORE_GESTION));
}

/*! \brief get the time gestion
 *
 * \return requested time gestion
 */
TimeGestion* Robot::get_time_gestion()
{
	CppInterface *cpp_interface;

	cpp_interface = static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);

	return static_cast<TimeGestion*>(cpp_interface->get_global_gestion()->get_gestion(TIME_GESTION));
}
