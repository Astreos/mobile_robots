#include "robot_init_pos.hh"
#include "config_functions.hh"
#include "Noise.hh"
#include "user_all_id.h"
#include "robot_id.h"
#include "config_file.h"
#include <cmath>

#define DEG_TO_RAD (M_PI / 180.0)

/*! \brief set the robot initial position
 * 
 * \param[out] q vector to fill with the robot positions
 */
void robot_init_pos(double *q)
{
	FILE *config_file;
	double T1_uncert, T2_uncert, R3_uncert;

	config_file = open_config();

	#ifdef HARDCODED_CONFIG
	T1_uncert = T1_UNCERT;
	T2_uncert = T2_UNCERT;
	R3_uncert = R3_UNCERT;

	// blue robot
	q[UX_T1_B] =  BLUE_T1 + range_noise(T1_uncert);
	q[UX_T2_B] =  BLUE_T2 + range_noise(T2_uncert);
	q[UX_R3_B] = (BLUE_R3 + range_noise(R3_uncert)) * DEG_TO_RAD;

	// red robot
	q[UX_T1_R] =  RED_T1 + range_noise(T1_uncert);
	q[UX_T2_R] =  RED_T2 + range_noise(T2_uncert);
	q[UX_R3_R] = (RED_R3 + range_noise(R3_uncert)) * DEG_TO_RAD;

	// yellow robot
	q[UX_T1_Y] =  YELLOW_T1 + range_noise(T1_uncert);
	q[UX_T2_Y] =  YELLOW_T2 + range_noise(T2_uncert);
	q[UX_R3_Y] = (YELLOW_R3 + range_noise(R3_uncert)) * DEG_TO_RAD;

	// white robot
	q[UX_T1_W] =  WHITE_T1 + range_noise(T1_uncert);
	q[UX_T2_W] =  WHITE_T2 + range_noise(T2_uncert);
	q[UX_R3_W] = (WHITE_R3 + range_noise(R3_uncert)) * DEG_TO_RAD;
	#else
	T1_uncert = config_double(config_file, "T1_uncert");
	T2_uncert = config_double(config_file, "T2_uncert");
	R3_uncert = config_double(config_file, "R3_uncert");

	// blue robot
	q[UX_T1_B] =  config_double(config_file, "blue_T1") + range_noise(T1_uncert);
	q[UX_T2_B] =  config_double(config_file, "blue_T2") + range_noise(T2_uncert);
	q[UX_R3_B] = (config_double(config_file, "blue_R3") + range_noise(R3_uncert)) * DEG_TO_RAD;

	// red robot
	q[UX_T1_R] =  config_double(config_file, "red_T1") + range_noise(T1_uncert);
	q[UX_T2_R] =  config_double(config_file, "red_T2") + range_noise(T2_uncert);
	q[UX_R3_R] = (config_double(config_file, "red_R3") + range_noise(R3_uncert)) * DEG_TO_RAD;

	// yellow robot
	q[UX_T1_Y] =  config_double(config_file, "yellow_T1") + range_noise(T1_uncert);
	q[UX_T2_Y] =  config_double(config_file, "yellow_T2") + range_noise(T2_uncert);
	q[UX_R3_Y] = (config_double(config_file, "yellow_R3") + range_noise(R3_uncert)) * DEG_TO_RAD;

	// white robot
	q[UX_T1_W] =  config_double(config_file, "white_T1") + range_noise(T1_uncert);
	q[UX_T2_W] =  config_double(config_file, "white_T2") + range_noise(T2_uncert);
	q[UX_R3_W] = (config_double(config_file, "white_R3") + range_noise(R3_uncert)) * DEG_TO_RAD;
	#endif

	close_config(config_file);
}
