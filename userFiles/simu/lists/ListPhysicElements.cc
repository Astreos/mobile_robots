
#include "ListPhysicElements.hh"
#include "MovingObject.hh"
#include "FixedObject.hh"
#include "Robot.hh"
#include "WallShape.hh"
#include "RectangleShape.hh"
#include "CircleShape.hh"
#include "MiniBotShape.hh"
#include "user_all_id.h"
#include "robot_id.h"
#include "config_functions.hh"
#include "config_file.h"

#include "NoCtrl.hh"
#include "RestingCtrl.hh"
#include "JoystickCtrl.hh"
#include "KeyboardCtrl.hh"
#include "Gr1Ctrl.hh"
#include "Gr2Ctrl.hh"
#include "Gr3Ctrl.hh"
#include "Gr4Ctrl.hh"
#include "Gr5Ctrl.hh"
#include "Gr6Ctrl.hh"
#include "Gr7Ctrl.hh"
#include "Gr8Ctrl.hh"
#include "Gr9Ctrl.hh"
#include "Gr10Ctrl.hh"
#include "Gr11Ctrl.hh"
#include "Gr12Ctrl.hh"
#include "Gr13Ctrl.hh"
#include "Gr14Ctrl.hh"
#include "Gr15Ctrl.hh"
#include "Gr16Ctrl.hh"
#include "Gr17Ctrl.hh"
#include "Gr18Ctrl.hh"
#include "Gr19Ctrl.hh"
#include "Gr20Ctrl.hh"

#include <iostream>
#include <cstddef>
#include <cstring>

#define RHO_DEFAULT 100.0   ///< mass per volume (default) [kg/m^3]
#define HEIGHT_DEFAULT 0.1  ///< object default height [m]
#define WRAPPING_ROBOT 0.13 ///< wrapping radius for the robots [m]

#define NO_VISIBLE_T1  100.0 ///< distance to avoid seeing the robot in x [m]
#define NO_VISIBLE_T2 -100.0 ///< distance to avoid seeing the robot in y [m]
#define NO_VISIBLE_R3    0.0 ///< angle to avoid seeing the robot [rad]

/*! \brief constructor
 * 
 * \param[in] mbs_data Robotran structure
 */
ListPhysicElements::ListPhysicElements(MbsData *mbs_data)
{
	this->mbs_data = mbs_data;

	robot_ctrl();
}

/*! \brief destructor
 */
ListPhysicElements::~ListPhysicElements()
{
	for (unsigned int i=0; i<list_full.size(); i++)
	{
		delete list_full[i];	
	}
}

/*! \brief get a new controller from its name (to be released outside the function)
 * 
 * \param[in] ctrl_name name of the controller
 * \param[in] robot_id ID of the robot
 * \param[in] nb_opp number of opponents
 * \return requested controller
 */
Controller* ListPhysicElements::get_ctrl_from_name(char *ctrl_name, int robot_id, int nb_opp)
{
	if (!strcmp(ctrl_name, "NoCtrl")) { return new NoCtrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "RestingCtrl"))  { return new RestingCtrl(robot_id, nb_opp);  }
	else if (!strcmp(ctrl_name, "KeyboardCtrl")) { return new KeyboardCtrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "JoystickCtrl")) { return new JoystickCtrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "Gr1Ctrl"))  { return new Gr1Ctrl(robot_id, nb_opp);  }
	else if (!strcmp(ctrl_name, "Gr2Ctrl"))  { return new Gr2Ctrl(robot_id, nb_opp);  }
	else if (!strcmp(ctrl_name, "Gr3Ctrl"))  { return new Gr3Ctrl(robot_id, nb_opp);  }
	else if (!strcmp(ctrl_name, "Gr4Ctrl"))  { return new Gr4Ctrl(robot_id, nb_opp);  }
	else if (!strcmp(ctrl_name, "Gr5Ctrl"))  { return new Gr5Ctrl(robot_id, nb_opp);  }
	else if (!strcmp(ctrl_name, "Gr6Ctrl"))  { return new Gr6Ctrl(robot_id, nb_opp);  }
	else if (!strcmp(ctrl_name, "Gr7Ctrl"))  { return new Gr7Ctrl(robot_id, nb_opp);  }
	else if (!strcmp(ctrl_name, "Gr8Ctrl"))  { return new Gr8Ctrl(robot_id, nb_opp);  }
	else if (!strcmp(ctrl_name, "Gr9Ctrl"))  { return new Gr9Ctrl(robot_id, nb_opp);  }
	else if (!strcmp(ctrl_name, "Gr10Ctrl")) { return new Gr10Ctrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "Gr11Ctrl")) { return new Gr11Ctrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "Gr12Ctrl")) { return new Gr12Ctrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "Gr13Ctrl")) { return new Gr13Ctrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "Gr14Ctrl")) { return new Gr14Ctrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "Gr15Ctrl")) { return new Gr15Ctrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "Gr16Ctrl")) { return new Gr16Ctrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "Gr17Ctrl")) { return new Gr17Ctrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "Gr18Ctrl")) { return new Gr18Ctrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "Gr19Ctrl")) { return new Gr19Ctrl(robot_id, nb_opp); }
	else if (!strcmp(ctrl_name, "Gr20Ctrl")) { return new Gr20Ctrl(robot_id, nb_opp); }
	else
	{
		std::cout << "Error: unknown controller (" << ctrl_name << ") !" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*! \brief check if it is an opponent of team A
 * 
 * \param[in] ctrl_name name of the controller
 * \param[in] robot_id ID of the robot
 * \return 1 if opponent acting on the game, 0 otherwise
 */
int ListPhysicElements::is_opp_team_A(char *ctrl_name, int robot_id)
{
	switch (robot_id)
	{
		case ROBOT_Y:
		case ROBOT_W:
			if (!strcmp(ctrl_name, "NoCtrl"))
			{
				return 0;
			}
			else
			{
				return 1;
			}
	
		default:
			return 0;
	}
}

/*! \brief check if it is an opponent of team B
 * 
 * \param[in] ctrl_name name of the controller
 * \param[in] robot_id ID of the robot
 * \return 1 if opponent acting on the game, 0 otherwise
 */
int ListPhysicElements::is_opp_team_B(char *ctrl_name, int robot_id)
{
	switch (robot_id)
	{
		case ROBOT_B:
		case ROBOT_R:
			if (!strcmp(ctrl_name, "NoCtrl"))
			{
				return 0;
			}
			else
			{
				return 1;
			}
	
		default:
			return 0;
	}
}

/*! \brief set the robot controllers
 *
 * \param[in] robot_id ID of the robot
 */
void ListPhysicElements::robot_ctrl()
{
	FILE *config_file;
	int nb_opp_A, nb_opp_B;
	char *ctrl_B_name, *ctrl_R_name, *ctrl_Y_name, *ctrl_W_name;

	config_file = open_config();

	#ifdef HARDCODED_CONFIG
	ctrl_B_name = (char*) BLUE_CTRL;
	ctrl_R_name = (char*) RED_CTRL;
	ctrl_Y_name = (char*) YELLLOW_CTRL;
	ctrl_W_name = (char*) WHITE_CTRL;
	#else
	ctrl_B_name = config_char(config_file, "blue_ctrl");
	ctrl_R_name = config_char(config_file, "red_ctrl");
	ctrl_Y_name = config_char(config_file, "yellow_ctrl");
	ctrl_W_name = config_char(config_file, "white_ctrl");
	#endif

	nb_opp_A = is_opp_team_A(ctrl_Y_name, ROBOT_Y) + is_opp_team_A(ctrl_W_name, ROBOT_W);
	nb_opp_B = is_opp_team_B(ctrl_B_name, ROBOT_B) + is_opp_team_B(ctrl_R_name, ROBOT_R);

	// blue robot
	add_robot(ROBOT_B, get_ctrl_from_name(ctrl_B_name, ROBOT_B, nb_opp_A));
	add_robot(ROBOT_R, get_ctrl_from_name(ctrl_R_name, ROBOT_R, nb_opp_A));
	add_robot(ROBOT_Y, get_ctrl_from_name(ctrl_Y_name, ROBOT_Y, nb_opp_B));
	add_robot(ROBOT_W, get_ctrl_from_name(ctrl_W_name, ROBOT_W, nb_opp_B));

	#ifndef HARDCODED_CONFIG
	free(ctrl_W_name);
	free(ctrl_Y_name);
	free(ctrl_R_name);
	free(ctrl_B_name);
	#endif

	close_config(config_file);
}

/*! \brief add a wall to the list of physical elements
 * 
 * \param[in] init_center center of the wall [m]
 * \param[in] angle orientation of the wall [rad]
 * \param[in] wrapping_radius wrapping radius of the wall [m]
 */
void ListPhysicElements::add_wall(Vector2D const& init_center, double angle, double wrapping_radius)
{
	GeometryShape *cur_geom;
	FixedObject   *cur_fixed;

	cur_geom  = new WallShape(init_center, angle, wrapping_radius);
	
	cur_fixed = new FixedObject(cur_geom);
	
	list_fixed.push_back(cur_fixed);
	list_full.push_back(cur_fixed);
}

/*! \brief add a circle to the list of physical elements
 * 
 * \param[in] init_center center of the circle [m]
 * \param[in] radius radius of the circle [m]
 * \param[in] visu_T1 ID of the T1 joint in the anim .mbs (-1 if none)
 * \param[in] visu_T2 ID of the T2 joint in the anim .mbs (-1 if none)
 * \param[in] visu_R3 ID of the R3 joint in the anim .mbs (-1 if none)
 * \param[in] flag_moving 1 if moving, 0 if fixed
 */
void ListPhysicElements::add_circle(Vector2D const& init_center, double radius, int visu_T1, int visu_T2, int visu_R3, int flag_moving)
{
	double cur_mass;

	GeometryShape *cur_geom;
	FixedObject   *cur_fixed;
	MovingObject  *cur_moving;

	cur_geom = new CircleShape(init_center, radius);

	if (flag_moving)
	{
		cur_geom->set_index_elem(MOVING_TYPE);
		cur_moving = new MovingObject(cur_geom);

		cur_mass = RHO_DEFAULT * M_PI * radius * radius * HEIGHT_DEFAULT;

		cur_moving->set_mass(cur_mass);
		cur_moving->set_inertia(0.5*cur_mass*radius*radius);

		list_moving.push_back(cur_moving);
		list_full.push_back(cur_moving);

		moving_T1_id.push_back(visu_T1);
		moving_T2_id.push_back(visu_T2);
		moving_R3_id.push_back(visu_R3);
	}
	else
	{
		cur_geom->set_index_elem(FIXED_TYPE);
		cur_fixed = new FixedObject(cur_geom);

		list_fixed.push_back(cur_fixed);
		list_full.push_back(cur_fixed);
	}	
}

/*! \brief add a rectangle to the list of physical elements
 * 
 * \param[in] init_center center of the rectangle [m]
 * \param[in] orientation orientation of the rectangle [rad]
 * \param[in] semi_width semi width of the rectangle [m]
 * \param[in] semi_height semi height of the rectangle [m]
 * \param[in] visu_T1 ID of the T1 joint in the anim .mbs (-1 if none)
 * \param[in] visu_T2 ID of the T2 joint in the anim .mbs (-1 if none)
 * \param[in] visu_R3 ID of the R3 joint in the anim .mbs (-1 if none)
 * \param[in] flag_moving 1 if moving, 0 if fixed
 */
void ListPhysicElements::add_rect(Vector2D const& init_center, double orientation, double semi_width, 
	double semi_height, int visu_T1, int visu_T2, int visu_R3, int flag_moving)
{
	double cur_mass;

	GeometryShape *cur_geom;
	FixedObject   *cur_fixed;
	MovingObject  *cur_moving;

	cur_geom = new RectangleShape(init_center, orientation, semi_width, semi_height);

	if (flag_moving)
	{
		cur_geom->set_index_elem(MOVING_TYPE);
		cur_moving = new MovingObject(cur_geom);

		cur_mass = RHO_DEFAULT * 4 * semi_width * semi_height * HEIGHT_DEFAULT;

		cur_moving->set_mass(cur_mass);
		cur_moving->set_inertia(cur_mass*semi_width*semi_width*semi_height*semi_height/3.0);

		list_moving.push_back(cur_moving);
		list_full.push_back(cur_moving);

		moving_T1_id.push_back(visu_T1);
		moving_T2_id.push_back(visu_T2);
		moving_R3_id.push_back(visu_R3);
	}
	else
	{
		cur_geom->set_index_elem(FIXED_TYPE);
		cur_fixed = new FixedObject(cur_geom);

		list_fixed.push_back(cur_fixed);
		list_full.push_back(cur_fixed);
	}
}

/*! \brief add a fixed circle to the list of physical elements
 * 
 * \param[in] init_center center of the circle [m]
 * \param[in] radius radius of the circle [m]
 */
void ListPhysicElements::add_fixed_circle(Vector2D const& init_center, double radius)
{
	add_circle(init_center, radius, -1, -1, -1, 0);
}

/*! \brief add a moving circle to the list of physical elements
 * 
 * \param[in] init_center center of the circle [m]
 * \param[in] radius radius of the circle [m]
 * \param[in] visu_T1 ID of the T1 joint in the anim .mbs (-1 if none)
 * \param[in] visu_T2 ID of the T2 joint in the anim .mbs (-1 if none)
 * \param[in] visu_R3 ID of the R3 joint in the anim .mbs (-1 if none)
 */
void ListPhysicElements::add_moving_circle(Vector2D const& init_center, double radius, int visu_T1, int visu_T2, int visu_R3)
{
	add_circle(init_center, radius, visu_T1, visu_T2, visu_R3, 1);
}

/*! \brief add a fixed rectangle to the list of physical elements
 * 
 * \param[in] init_center center of the rectangle [m]
 * \param[in] orientation orientation of the rectangle [rad]
 * \param[in] semi_width semi width of the rectangle [m]
 * \param[in] semi_height semi height of the rectangle [m]
 */
void ListPhysicElements::add_fixed_rect(Vector2D const& init_center, double orientation, double semi_width, double semi_height)
{
	add_rect(init_center, orientation, semi_width, semi_height, -1, -1, -1, 0);
}

/*! \brief add a moving rectangle to the list of physical elements
 * 
 * \param[in] init_center center of the rectangle [m]
 * \param[in] orientation orientation of the rectangle [rad]
 * \param[in] semi_width semi width of the rectangle [m]
 * \param[in] semi_height semi height of the rectangle [m]
 * \param[in] visu_T1 ID of the T1 joint in the anim .mbs (-1 if none)
 * \param[in] visu_T2 ID of the T2 joint in the anim .mbs (-1 if none)
 * \param[in] visu_R3 ID of the R3 joint in the anim .mbs (-1 if none)
 */
void ListPhysicElements::add_moving_rect(Vector2D const& init_center, double orientation, double semi_width, double semi_height,
	 int visu_T1, int visu_T2, int visu_R3)
{
	add_rect(init_center, orientation, semi_width, semi_height, visu_T1, visu_T2, visu_R3, 1);
}

/*! \brief add a fixed union to the list of physical elements
 * 
 * \param[in] cur_union new union to add 
 */
void ListPhysicElements::add_fixed_union(UnionShape *cur_union)
{
	FixedObject *cur_fixed;

	cur_union->set_index_elem(FIXED_TYPE);

	cur_fixed = new FixedObject(cur_union);

	list_fixed.push_back(cur_fixed);
	list_full.push_back(cur_fixed);
}

/*! \brief add a moving union to the list of physical elements
 * 
 * \param[in] cur_union new union to add
 * \param[in] visu_T1 ID of the T1 joint in the anim .mbs (-1 if none)
 * \param[in] visu_T2 ID of the T2 joint in the anim .mbs (-1 if none)
 * \param[in] visu_R3 ID of the R3 joint in the anim .mbs (-1 if none)
 */
void ListPhysicElements::add_moving_union(UnionShape *cur_union, int visu_T1, int visu_T2, int visu_R3)
{
	MovingObject *cur_moving;

	cur_union->set_index_elem(MOVING_TYPE);

	cur_moving = new MovingObject(cur_union);

	list_moving.push_back(cur_moving);
	list_full.push_back(cur_moving);

	moving_T1_id.push_back(visu_T1);
	moving_T2_id.push_back(visu_T2);
	moving_R3_id.push_back(visu_R3);
}

/*! \brief add a robot to the list of physical elements
 * 
 * \param[in] robot_id ID of the robot
 * \param[in] ctrl controller
 */
void ListPhysicElements::add_robot(int robot_id, Controller *ctrl)
{
	int T1_id, T2_id, R3_id, RW_id, LW_id, TW_id;
	int ux_T1_id, ux_T2_id, ux_R3_id;
	double *ux;

	Vector2D init_pos;
	MiniBotShape *cur_robot_shape;
	Robot        *cur_robot;

	// joint IDs
	switch (robot_id)
	{
		case ROBOT_B:
			T1_id = FJ_T1_robot_B_id;
			T2_id = FJ_T2_robot_B_id;
			R3_id = FJ_R3_robot_B_id;
			RW_id = Right_wheel_joint_B_id;
			LW_id = Left_wheel_joint_B_id;
			TW_id = Tower_joint_B_id;
			ux_T1_id = UX_T1_B;
			ux_T2_id = UX_T2_B;
			ux_R3_id = UX_R3_B;
			break;

		case ROBOT_R:
			T1_id = FJ_T1_robot_R_id;
			T2_id = FJ_T2_robot_R_id;
			R3_id = FJ_R3_robot_R_id;
			RW_id = Right_wheel_joint_R_id;
			LW_id = Left_wheel_joint_R_id;
			TW_id = Tower_joint_R_id;
			ux_T1_id = UX_T1_R;
			ux_T2_id = UX_T2_R;
			ux_R3_id = UX_R3_R;
			break;

		case ROBOT_Y:
			T1_id = FJ_T1_robot_Y_id;
			T2_id = FJ_T2_robot_Y_id;
			R3_id = FJ_R3_robot_Y_id;
			RW_id = Right_wheel_joint_Y_id;
			LW_id = Left_wheel_joint_Y_id;
			TW_id = Tower_joint_Y_id;
			ux_T1_id = UX_T1_Y;
			ux_T2_id = UX_T2_Y;
			ux_R3_id = UX_R3_Y;
			break;

		case ROBOT_W:
			T1_id = FJ_T1_robot_W_id;
			T2_id = FJ_T2_robot_W_id;
			R3_id = FJ_R3_robot_W_id;
			RW_id = Right_wheel_joint_W_id;
			LW_id = Left_wheel_joint_W_id;
			TW_id = Tower_joint_W_id;
			ux_T1_id = UX_T1_W;
			ux_T2_id = UX_T2_W;
			ux_R3_id = UX_R3_W;
			break;
	
		default:
			std::cout << "Error:unknown robot ID: " << robot_id << " !" << std::endl;
			exit(EXIT_FAILURE);
	}

	// no controller, no robot on the game
	if (!ctrl->is_ctrl())
	{
		ux = mbs_data->ux;
		init_pos.set_vector(NO_VISIBLE_T1, NO_VISIBLE_T2); // robot not visible

		ux[ux_T1_id] = NO_VISIBLE_T1;
		ux[ux_T2_id] = NO_VISIBLE_T2;
		ux[ux_R3_id] = NO_VISIBLE_R3;
	}
	else
	{
		init_pos.set_vector(mbs_data->q[T1_id], mbs_data->q[T2_id]);
	}

	cur_robot_shape = new MiniBotShape(init_pos, mbs_data->q[R3_id], WRAPPING_ROBOT);

	// no controller, no robot on the game
	if (!ctrl->is_ctrl())
	{
		cur_robot_shape->set_contact(0);
		cur_robot_shape->set_uSwitch(0);
	}

	cur_robot = new Robot(cur_robot_shape, mbs_data, ctrl, robot_id, T1_id, T2_id, R3_id, RW_id, LW_id, TW_id);

	list_robot.push_back(cur_robot);
	list_full.push_back(cur_robot);
}

/*! \brief fill the IDs of T1, T2 and R3
 * 
 * \param[in] moving_id ID of the moving object
 * \param[out] T1 T1 joint ID
 * \param[out] T2 T2 joint ID
 * \param[out] R3 R3 joint ID
 */
void ListPhysicElements::fill_T1_T2_R3(int moving_id, int &T1, int &T2, int &R3)
{
	// safety
	if (moving_id >= list_moving.size())
	{
		std::cout << "Error: moving_id (" << moving_id << ") >= list_moving.size() (" << list_moving.size() << ") !" << std::endl;
		exit(EXIT_FAILURE);
	}

	T1 = moving_T1_id[moving_id];
	T2 = moving_T2_id[moving_id];
	R3 = moving_R3_id[moving_id];
}
