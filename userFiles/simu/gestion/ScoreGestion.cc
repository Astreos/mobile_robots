#include "ScoreGestion.hh"
#include "CppInterface.hh"
#include "user_model.h"
#include "Robot.hh"
#include "config_functions.hh"

#define PENALTY_INIT_MOTION 10   ///< penalty points when moving at the beginning
#define PENALTY_FINISH_MOTION 10 ///< penalty points when moving at the end
#define PENALTY_CONTACT 3        ///< penalty points for contacts
#define SAFETY_CHECK_DIST 1.0e-3 ///< safety distance for initial check [m]

/*! \brief constructor
 * 
 * \param[in] list_elem list of elements
 * \param[in] mbs_data Robotran structure
 */
ScoreGestion::ScoreGestion(ListElements *list_elem, MbsData *mbs_data): Gestion(list_elem)
{
	FILE *config_file;
	std::vector<double> x_vec_A, y_vec_A, angle_vec_A, semi_width_vec_A, semi_height_vec_A;
	std::vector<double> x_vec_B, y_vec_B, angle_vec_B, semi_width_vec_B, semi_height_vec_B;

	this->mbs_data = mbs_data;

	list_phys_elem = list_elem->get_list_phys_elem();
	list_targets   = list_elem->get_list_targets();
	list_bases     = list_elem->get_list_bases();

	score_A = 0;
	score_B = 0;

	flag_init = 0;

	nb_robot = list_phys_elem->get_nb_robot();

	speed_motion = 5.0e-2;

	penalty_init_A = 0;
	penalty_init_B = 0;

	penalty_finish_A = 0;
	penalty_finish_B = 0;

	// init bases
	config_file = open_config();

	config_target_bases(config_file, 1, TEAM_A, x_vec_A, y_vec_A, angle_vec_A, semi_width_vec_A, semi_height_vec_A);
	config_target_bases(config_file, 1, TEAM_B, x_vec_B, y_vec_B, angle_vec_B, semi_width_vec_B, semi_height_vec_B);

	// safety
	if (x_vec_A.size() != 1)
	{
		std::cout << "Error: only one init basis for team A requested, but " << x_vec_A.size() << " received !" << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (x_vec_B.size() != 1)
	{
		std::cout << "Error: only one init basis for team B requested, but " << x_vec_B.size() << " received !" << std::endl;
		exit(EXIT_FAILURE);
	}

	init_basis_A = new RectangleShape(get_vector(x_vec_A[0],  y_vec_A[0]), angle_vec_A[0], semi_width_vec_A[0]+SAFETY_CHECK_DIST, semi_height_vec_A[0]+SAFETY_CHECK_DIST);
	init_basis_B = new RectangleShape(get_vector(x_vec_B[0],  y_vec_B[0]), angle_vec_B[0], semi_width_vec_B[0]+SAFETY_CHECK_DIST, semi_height_vec_B[0]+SAFETY_CHECK_DIST);

	close_config(config_file);

	no_contact_time = 5.0;
	last_t_penalty = mbs_data->tsim - no_contact_time;

	final_printed = 0;

	std::cout << "-------------------" << std::endl;
}

/*! \brief destructor
 */
ScoreGestion::~ScoreGestion()
{
	delete init_basis_B;
	delete init_basis_A;
}

/*! \brief score permanent computation
 */
void ScoreGestion::compute()
{
	CppInterface *cpp_interface;

	if (!flag_init)
	{
		flag_init = 1;

		cpp_interface = static_cast<CppInterface*>(mbs_data->user_model->user.cpp_interface);
		time_gestion  = static_cast<TimeGestion*>(cpp_interface->get_global_gestion()->get_gestion(TIME_GESTION));
	}

	switch (time_gestion->get_state())
	{
		case CALIBRATION_GAME:
			break;

		case CHECK_INIT_GAME:
			check_init();
			break;

		case RUN_GAME:
			break;

		case CHECK_FINISH_GAME:
			check_end();
			break;

		case FINISH_GAME:
			if (!final_printed)
			{
				final_printed = 1;
				print_final();
			}
			break;
	
		default:
			std::cout << "Error: unknown time state (" << time_gestion->get_state() << ") !" << std::endl;
			exit(EXIT_FAILURE);
	}	
}

/*! \brief check that robots do not move out of the bases during the initialization
 */
void ScoreGestion::check_init()
{
	Robot *cur_robot;
	int cur_team;

	for(int i=0; i<nb_robot; i++)
	{
		cur_robot = list_phys_elem->get_robot(i);
		cur_team  = cur_robot->get_team();

		if (cur_robot->is_ctrl())
		{
			switch (cur_team)
			{
				case TEAM_A:
					if (!penalty_init_A)
					{
						if (!cur_robot->get_minibot_shape()->inside_rect(init_basis_A))
						{
							penalty_init_A = 1;
							score_A -= PENALTY_INIT_MOTION;
							std::cout << "team A: -" << PENALTY_INIT_MOTION << " points (not inside basis)" << std::endl;
						}
					}
					break;

				case TEAM_B:
					if (!penalty_init_B)
					{
						if (!cur_robot->get_minibot_shape()->inside_rect(init_basis_B))
						{
							penalty_init_B = 1;
							score_B -= PENALTY_INIT_MOTION;
							std::cout << "team B: -" << PENALTY_INIT_MOTION << " points (not inside basis)" << std::endl;
						}
					}
					break;
			
				default:
					std::cout << "Error:unknown team (" << cur_team << ") !" << std::endl;
					exit(EXIT_FAILURE);
			}
		}
	}
}

/*! \brief check that robots do not move when the game ended
 */
void ScoreGestion::check_end()
{
	Robot *cur_robot;
	double cur_speed;
	int cur_team;

	for(int i=0; i<nb_robot; i++)
	{
		cur_robot = list_phys_elem->get_robot(i);

		if (cur_robot->is_ctrl())
		{
			cur_speed = cur_robot->get_speed();
			cur_team  = cur_robot->get_team();

			if (cur_speed > speed_motion)
			{
				switch (cur_team)
				{
					case TEAM_A:
						if (!penalty_finish_A)
						{
							penalty_finish_A = 1;
							score_A -= PENALTY_FINISH_MOTION;
							std::cout << "team A: -" << PENALTY_FINISH_MOTION << " points (end motion)" << std::endl;
						}
						break;

					case TEAM_B:
						if (!penalty_finish_B)
						{
							penalty_finish_B = 1;
							score_B -= PENALTY_FINISH_MOTION;
							std::cout << "team B: -" << PENALTY_FINISH_MOTION << " points (end motion)" << std::endl;
						}
						break;

					default:
						std::cout << "Error:unknown team (" << cur_team << ") !" << std::endl;
						exit(EXIT_FAILURE);
				}
			}
		}	
	}
}

/*! \brief set a new contact
 * 
 * \param[in] geom_1 geometry shape of the first robot
 * \param[in] geom_2 geometry shape of the second robot
 * \param[in] robot_id_1 ID of the first robot with contact
 * \param[in] robot_id_2 ID of the second robot with contact
 */
void ScoreGestion::set_contact(GeometryShape* geom_1, GeometryShape* geom_2, int robot_id_1, int robot_id_2)
{
	int team_1, team_2, team_penalty;
	double speed_1, speed_2;

	// last contact too close
	if (mbs_data->tsim - last_t_penalty < no_contact_time)
	{
		return;
	}

	// safety
	if ((robot_id_1 < 0) || (robot_id_2 < 0))
	{
		std::cout << "Error: unknown robots indexes for contact !" << std::endl;
		exit(EXIT_FAILURE);
	}

	switch (robot_id_1)
	{
		case ROBOT_B:
		case ROBOT_R:
			team_1 = TEAM_A;
			break;

		case ROBOT_Y:
		case ROBOT_W:
			team_1 = TEAM_B;
			break;
	
		default:
			std::cout << "Error: unknown robot ID (" << robot_id_1 << ") !" << std::endl;
			break;
	}

	switch (robot_id_2)
	{
		case ROBOT_B:
		case ROBOT_R:
			team_2 = TEAM_A;
			break;

		case ROBOT_Y:
		case ROBOT_W:
			team_2 = TEAM_B;
			break;
	
		default:
			std::cout << "Error: unknown robot ID (" << robot_id_2 << ") !" << std::endl;
			break;
	}

	// contacts between the same team
	if (team_1 == team_2)
	{
		return;
	}

	speed_1 = geom_1->get_abs_speed();
	speed_2 = geom_2->get_abs_speed();

	// exact same speeds
	if (speed_1 == speed_2)
	{
		return;
	}

	// penalty team
	team_penalty = (speed_1 > speed_2) ? team_1 : team_2;

	last_t_penalty = mbs_data->tsim;

	switch (team_penalty)
	{
		case TEAM_A:
			score_A -= PENALTY_CONTACT;
			std::cout << "team A: -" << PENALTY_CONTACT << " points (contact)" << std::endl;
			break;

		case TEAM_B:
			score_B -= PENALTY_CONTACT;
			std::cout << "team B: -" << PENALTY_CONTACT << " points (contact)" << std::endl;
			break;
	
		default:
			std::cout << "Error: unknown team (" << team_penalty << ") !" << std::endl;
			exit(EXIT_FAILURE);
	}
}

/*! \brief add a target to one basis
 * 
 * \param[in] target target to add
 */
void ScoreGestion::add_target(Target *target, ListBases *bases)
{
	int target_value;

	target_value = target->get_points();

	if (bases->target_full_inside_team(target, TEAM_A))
	{
		score_A += target_value;
		std::cout << "team A: +" << target_value << " points" << std::endl;
	}
	else if (bases->target_full_inside_team(target, TEAM_B))
	{
		score_B += target_value;
		std::cout << "team B: +" << target_value << " points" << std::endl;
	}
	else
	{
		std::cout << "Warning: target not attributed to any team !" << std::endl;
	}
}

/*! \brief print final score
 */
void ScoreGestion::print_final()
{
	std::cout << "-------------------" << std::endl;

	if (score_A > score_B)
	{
		std::cout << "Winner: TEAM A !" << std::endl;
	}
	else if (score_A < score_B)
	{
		std::cout << "Winner: TEAM B !" << std::endl;
	}
	else
	{
		std::cout << "Drawn game !" << std::endl;
	}

	std::cout << "team A: " << score_A << " points >< team B: " << score_B << " points" << std::endl;

	std::cout << "-------------------" << std::endl;
}
