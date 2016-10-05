#include "TowerSensor.hh"
#include "robot_id.h"
#include "Robot.hh"
#include "config_functions.hh"
#include "config_file.h"

#define MIN_X_TABLE -1.0
#define MAX_X_TABLE  1.0
#define MIN_Y_TABLE -1.5
#define MAX_Y_TABLE  1.5

#define SAFETY_BEACON 0.5

#define MIN_X_TABLE_SAFETY (MIN_X_TABLE - SAFETY_BEACON)
#define MAX_X_TABLE_SAFETY (MAX_X_TABLE + SAFETY_BEACON)
#define MIN_Y_TABLE_SAFETY (MIN_Y_TABLE - SAFETY_BEACON)
#define MAX_Y_TABLE_SAFETY (MAX_Y_TABLE + SAFETY_BEACON)

#define NB_OPPONENTS 2

#define OPTICAL_BARRIER (-(3.0/4.0)*M_PI) ///< optical barrier position [rad]

/*! \brief contructor
 * 
 * \param[in] mbs_data Robotran structure
 * \param[in] robot_id ID of the robot
 * \param[in] tower_dist distance from the robot center position to its tower [m]
 * \param[in] tower_angle angle between the robot center and the tower [rad]
 */
TowerSensor::TowerSensor(MbsData *mbs_data, int robot_id): Sensor(mbs_data)
{
	FILE* config_file;

	config_file = open_config();

	#ifdef HARDCODED_CONFIG
	tower_noise = TOWER_NOISE;
	#else
	tower_noise = config_double(config_file, "tower_noise");
	#endif

	this->robot_id = robot_id;

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
			std::cout << "Error: unknown robot ID (" << robot_id << ") !" << std::endl;
			exit(EXIT_FAILURE);
	}

	switch (team_id)
	{
		case TEAM_A:
			#ifdef HARDCODED_CONFIG
			beacon_fixed[0] = get_vector(TEAM_A_BEACON_1_X, TEAM_A_BEACON_1_Y);
			beacon_fixed[1] = get_vector(TEAM_A_BEACON_2_X, TEAM_A_BEACON_2_Y);
			beacon_fixed[2] = get_vector(TEAM_A_BEACON_3_X, TEAM_A_BEACON_3_Y);
			#else
			beacon_fixed[0] = get_vector(config_double(config_file, "team_A_beacon_1_x"), config_double(config_file, "team_A_beacon_1_y"));
			beacon_fixed[1] = get_vector(config_double(config_file, "team_A_beacon_2_x"), config_double(config_file, "team_A_beacon_2_y"));
			beacon_fixed[2] = get_vector(config_double(config_file, "team_A_beacon_3_x"), config_double(config_file, "team_A_beacon_3_y"));
			#endif
			break;

		case TEAM_B:
			#ifdef HARDCODED_CONFIG
			beacon_fixed[0] = get_vector(TEAM_B_BEACON_1_X, TEAM_B_BEACON_1_Y);
			beacon_fixed[1] = get_vector(TEAM_B_BEACON_2_X, TEAM_B_BEACON_2_Y);
			beacon_fixed[2] = get_vector(TEAM_B_BEACON_3_X, TEAM_B_BEACON_3_Y);
			#else
			beacon_fixed[0] = get_vector(config_double(config_file, "team_B_beacon_1_x"), config_double(config_file, "team_B_beacon_1_y"));
			beacon_fixed[1] = get_vector(config_double(config_file, "team_B_beacon_2_x"), config_double(config_file, "team_B_beacon_2_y"));
			beacon_fixed[2] = get_vector(config_double(config_file, "team_B_beacon_3_x"), config_double(config_file, "team_B_beacon_3_y"));
			#endif
			break;

		default:
			std::cout << "Error: unknown team ID (" << team_id << ") !" << std::endl;
			exit(EXIT_FAILURE);
	}

	tower_gamma = 0.0;
	tower_theta = 0.0;
	prev_tower_gamma = 0.0;
	prev_tower_theta = 0.0;

	nb_interp = 10;

	inc_t = 0.0;
	inc_gamma = 0.0;
	inc_theta = 0.0;
	inc_ratio = 0.0;

	cur_t  = mbs_data->tsim;
	prev_t = cur_t;

	radius_fixed = 0.04;
	radius_robot = 0.04;

	last_fixed_detect = 0;
	last_robot_detect = 0;

	reset_input(robot_input);
	reset_input(fixed_input);

	last_sign = 0;

	close_config(config_file);
}

/*! \brief destructor
 */
TowerSensor::~TowerSensor()
{
	
}

/*! \brief update the sensor
 */
void TowerSensor::update()
{
	update_incr();
	update_fixed();
	udpate_robot();
	check_laser_reset();
}

/*! \brief reset the inputs of tower
 *
 * \param[out] tower_input input to reset
 */
void TowerSensor::reset_input(TowerInStruct &tower_input)
{
	for(int i=0; i<NB_STORE_EDGE; i++)
	{
		tower_input.last_rising[i]  = 0.0;
		tower_input.last_falling[i] = 0.0;

		tower_input.last_rising_noise[i]  = 0.0;
		tower_input.last_falling_noise[i] = 0.0;
	}

	tower_input.rising_index  = NB_STORE_EDGE-1;
	tower_input.falling_index = NB_STORE_EDGE-1;

	tower_input.count_rising  = 0;
	tower_input.count_falling = 0;

	tower_input.nb_rising  = 0;
	tower_input.nb_falling = 0;
}

/*! \brief detect if the tower can see a beacon
 * 
 * \param[in] cur_tower_pos tower current position
 * \param[in] cur_beacon_pos beacon current position
 * \param[in] cur_tower_theta current tower angle [rad]
 * \param[in] beacon_radius beacon radius [m]
 * \return 1 if detected, 0 otherwise
 */
int TowerSensor::is_tower_detected(Vector2D cur_tower_pos, Vector2D cur_beacon_pos, double cur_tower_theta, double beacon_radius)
{
	double dist, semi_angle, orient_angle;

	// safety 1
	if ( (cur_beacon_pos.get_x() < MIN_X_TABLE_SAFETY) ||
		 (cur_beacon_pos.get_x() > MAX_X_TABLE_SAFETY) ||
		 (cur_beacon_pos.get_y() < MIN_Y_TABLE_SAFETY) ||
		 (cur_beacon_pos.get_y() > MAX_Y_TABLE_SAFETY) )
	{
		return 0;
	}

	dist = get_dist_points2D(cur_tower_pos, cur_beacon_pos);

	// safety 2
	if (!dist)
	{
		return 0;
	}

	semi_angle = fabs( asin(beacon_radius / dist) );

	orient_angle = get_angle(cur_tower_pos, cur_beacon_pos);

	if (fabs(limit_angle(cur_tower_theta - orient_angle)) < semi_angle)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*! \brief update the incremental values
 */
void TowerSensor::update_incr()
{
	rob = get_robot(robot_id);

	cur_t  = rob->get_cur_t();
	prev_t = rob->get_prev_t();

	tower_gamma = rob->get_tower_gamma();
	tower_theta = rob->get_tower_theta();

	prev_tower_gamma = rob->get_prev_tower_gamma();
	prev_tower_theta = rob->get_prev_tower_theta();

	tower_pos = rob->get_tower_pos();
	prev_tower_pos = rob->get_prev_tower_pos();

	inc_t = (cur_t - prev_t) / nb_interp;
	inc_ratio = 1.0 / nb_interp;
	inc_gamma = (tower_gamma - prev_tower_gamma) / nb_interp;
	inc_theta = (tower_theta - prev_tower_theta) / nb_interp;

	// safety
	if ( (tower_theta < 0.0) && (prev_tower_theta > 0.0) && (fabs(tower_theta) > M_PI*0.8) )
	{
		inc_theta = (2.0*M_PI + tower_theta - prev_tower_theta) / nb_interp;
	}
	else if ( (tower_theta > 0.0) && (prev_tower_theta < 0.0) && (fabs(tower_theta) > M_PI*0.8) )
	{
		inc_theta = (-2.0*M_PI + tower_theta - prev_tower_theta) / nb_interp;
	}

	if ( (tower_gamma < 0.0) && (prev_tower_gamma > 0.0) && (fabs(tower_gamma) > M_PI*0.8) )
	{
		inc_gamma = (2.0*M_PI + tower_gamma - prev_tower_gamma) / nb_interp;
	}
	else if ( (tower_gamma > 0.0) && (prev_tower_gamma < 0.0) && (fabs(tower_gamma) > M_PI*0.8) )
	{
		inc_gamma = (-2.0*M_PI + tower_gamma - prev_tower_gamma) / nb_interp;
	}
}

/*! \brief update the fixed beacons detection
 */
void TowerSensor::update_fixed()
{
	Vector2D cur_tower_pos;
	double cur_tower_theta, cur_tower_gamma, cur_ratio;
	int cur_detected;

	for(int i=0; i<nb_interp; i++)
	{
		cur_tower_gamma = prev_tower_gamma + i*inc_gamma;
		cur_tower_theta = prev_tower_theta + i*inc_theta;

		cur_ratio = i*inc_ratio;

		cur_tower_pos = get_interpolation_vec_2D(prev_tower_pos, tower_pos, cur_ratio);

		cur_detected = 0;

		for(int j=0; j<NB_BEACON_POS; j++)
		{
			if (is_tower_detected(cur_tower_pos, beacon_fixed[j], cur_tower_theta, radius_fixed))
			{
				cur_detected = 1;
				break;
			}
		}

		// rising or falling edge
		if (cur_detected != last_fixed_detect)
		{
			last_fixed_detect = cur_detected;

			if (cur_detected) // rising edge
			{
				fixed_input.rising_index++;
				fixed_input.rising_index = (fixed_input.rising_index >= NB_STORE_EDGE) ? 0 : fixed_input.rising_index;

				fixed_input.count_rising++;

				fixed_input.last_rising[fixed_input.rising_index] = limit_angle(cur_tower_gamma);
				fixed_input.last_rising_noise[fixed_input.rising_index] = limit_angle(cur_tower_gamma + range_noise(tower_noise));
			}
			else // falling edge
			{
				fixed_input.falling_index++;
				fixed_input.falling_index = (fixed_input.falling_index >= NB_STORE_EDGE) ? 0 : fixed_input.falling_index;

				fixed_input.count_falling++;

				fixed_input.last_falling[fixed_input.falling_index] = limit_angle(cur_tower_gamma);
				fixed_input.last_falling_noise[fixed_input.falling_index] = limit_angle(cur_tower_gamma + range_noise(tower_noise));
			}
		}
	}
}

/*! \brief update opponent robots detection
 */
void TowerSensor::udpate_robot()
{
	Vector2D cur_tower_pos, cur_opp_pos;
	double cur_tower_theta, cur_tower_gamma, cur_ratio;
	int cur_detected;
	Robot *cur_opp;

	for(int i=0; i<nb_interp; i++)
	{
		cur_tower_gamma = prev_tower_gamma + i*inc_gamma;
		cur_tower_theta = prev_tower_theta + i*inc_theta;

		cur_ratio = i*inc_ratio;

		cur_tower_pos = get_interpolation_vec_2D(prev_tower_pos, tower_pos, cur_ratio);

		cur_detected = 0;

		for(int j=0; j<NB_OPPONENTS; j++)
		{
			cur_opp = get_opponent(robot_id, j);

			if (cur_opp->is_ctrl()) // if robot in game
			{
				cur_opp_pos = get_interpolation_vec_2D(cur_opp->get_prev_tower_pos(), cur_opp->get_tower_pos(), cur_ratio);

				if (is_tower_detected(cur_tower_pos, cur_opp_pos, cur_tower_theta, radius_fixed))
				{
					cur_detected = 1;
					break;
				}
			}
		}

		// rising or falling edge
		if (cur_detected != last_robot_detect)
		{
			last_robot_detect = cur_detected;

			if (cur_detected) // rising edge
			{
				robot_input.rising_index++;
				robot_input.rising_index = (robot_input.rising_index >= NB_STORE_EDGE) ? 0 : robot_input.rising_index;

				robot_input.count_rising++;

				robot_input.last_rising[robot_input.rising_index] = limit_angle(cur_tower_gamma);
				robot_input.last_rising_noise[robot_input.rising_index] = limit_angle(cur_tower_gamma + range_noise(tower_noise));
			}
			else // falling edge
			{
				robot_input.falling_index++;
				robot_input.falling_index = (robot_input.falling_index >= NB_STORE_EDGE) ? 0 : robot_input.falling_index;

				robot_input.count_falling++;

				robot_input.last_falling[robot_input.falling_index] = limit_angle(cur_tower_gamma);
				robot_input.last_falling_noise[robot_input.falling_index] = limit_angle(cur_tower_gamma + range_noise(tower_noise));
			}
		}
	}
}

/*! \brief check the optical barrier of the tower
 */
void TowerSensor::check_laser_reset()
{
	double diff_angle;
	int cur_sign;

	diff_angle = limit_angle(tower_gamma - OPTICAL_BARRIER);

	cur_sign = (diff_angle < 0.0) ? 0 : 1;

	if (cur_sign != last_sign)
	{
		last_sign = cur_sign;

		if (fabs(diff_angle) < 0.5) // optical barrier
		{
			// fixed beacons
			fixed_input.nb_rising  = fixed_input.count_rising;
			fixed_input.nb_falling = fixed_input.count_falling;

			fixed_input.count_rising  = 0;
			fixed_input.count_falling = 0;

			// opponent robots
			robot_input.nb_rising  = robot_input.count_rising;
			robot_input.nb_falling = robot_input.count_falling;

			robot_input.count_rising  = 0;
			robot_input.count_falling = 0;
		}
	}
}
