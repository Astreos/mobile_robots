/*! 
 * \author Nicolas Van der Noot
 * \file config_file.cc
 * \brief This file is used to configure the project environment without parsing the 'config_file.txt' file
 *
 * To use it, you just have to uncomment 'HARDCODED_CONFIG' in 'config_file.h'.
 * This file is inspired from 'config_file.txt'. You can read the instructions there.
 */

#include "config_file.h"
#include "robot_id.h"
#include "config_functions.hh"
#include <cmath>

#define DEG_TO_RAD (M_PI / 180.0) ///< conversin from degrees to radians

/*! \brief get the targets from the config file (hard-coded version)
 * 
 * \param[out] x_vec x position vector [m]
 * \param[out] y_vec y position vector [m]
 * \param[out] mbs_T1_vec anim .mbs file T1 IDs vector
 * \param[out] mbs_T2_vec anim .mbs file T2 IDs vector
 * \param[out] mbs_T3_vec anim .mbs file T3 IDs vector
 * \param[out] value_vec target values vector
 */
void config_hard_targets(std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<int> & mbs_T1_vec, std::vector<int> & mbs_T2_vec, std::vector<int> & mbs_T3_vec, std::vector<int> & value_vec)
{
	// target 1
	x_vec.push_back(0.7);
	y_vec.push_back(-0.6);
	mbs_T1_vec.push_back(25);
	mbs_T2_vec.push_back(26);
	mbs_T3_vec.push_back(27);
	value_vec.push_back(1);

	// target 2
	x_vec.push_back(0.7);
	y_vec.push_back(0.6);
	mbs_T1_vec.push_back(28);
	mbs_T2_vec.push_back(29);
	mbs_T3_vec.push_back(30);
	value_vec.push_back(1);

	// target 3
	x_vec.push_back(-0.4);
	y_vec.push_back(-0.6);
	mbs_T1_vec.push_back(31);
	mbs_T2_vec.push_back(32);
	mbs_T3_vec.push_back(33);
	value_vec.push_back(1);

	// target 4
	x_vec.push_back(-0.4);
	y_vec.push_back(0.6);
	mbs_T1_vec.push_back(34);
	mbs_T2_vec.push_back(35);
	mbs_T3_vec.push_back(36);
	value_vec.push_back(1);

	// target 5
	x_vec.push_back(0.25);
	y_vec.push_back(-1.25);
	mbs_T1_vec.push_back(37);
	mbs_T2_vec.push_back(38);
	mbs_T3_vec.push_back(39);
	value_vec.push_back(2);

	// target 6
	x_vec.push_back(0.25);
	y_vec.push_back(1.25);
	mbs_T1_vec.push_back(40);
	mbs_T2_vec.push_back(41);
	mbs_T3_vec.push_back(42);
	value_vec.push_back(2);

	// target 7
	x_vec.push_back(0.1);
	y_vec.push_back(0.0);
	mbs_T1_vec.push_back(43);
	mbs_T2_vec.push_back(44);
	mbs_T3_vec.push_back(45);
	value_vec.push_back(2);

	// target 8
	x_vec.push_back(-0.8);
	y_vec.push_back(0.0);
	mbs_T1_vec.push_back(46);
	mbs_T2_vec.push_back(47);
	mbs_T3_vec.push_back(48);
	value_vec.push_back(3);
}

/*! \brief get the bases from the config file (hard-coded version)
 * 
 * \param[in] flag_init 1 if init bases, 0 if target bases
 * \param[in] team_ID team ID
 * \param[out] x_vec x position vector of the rectangle [m]
 * \param[out] y_vec y position vector of the rectangle [m]
 * \param[out] angle_vec orientation angle of the rectangle [m]
 * \param[out] semi_width_vec rectangle semi width [m]
 * \param[out] semi_height_vec rectangle semi height [m]
 */
void config_hard_target_bases(int flag_init, int team_ID, std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & angle_vec, std::vector<double> & semi_width_vec, std::vector<double> & semi_height_vec)
{
	if (flag_init) // initial bases
	{
		if (team_ID == TEAM_A) // team A
		{
			x_vec.push_back(0.75);
			y_vec.push_back(1.25);
			angle_vec.push_back(0.0 * DEG_TO_RAD);
			semi_width_vec.push_back(0.5 * 0.5);
			semi_height_vec.push_back(0.5 * 0.5);
		}
		else// team B
		{
			x_vec.push_back(0.75);
			y_vec.push_back(-1.25);
			angle_vec.push_back(0.0 * DEG_TO_RAD);
			semi_width_vec.push_back(0.5 * 0.5);
			semi_height_vec.push_back(0.5 * 0.5);
		}
	}
	else // target bases
	{
		if (team_ID == TEAM_A) // team A
		{
			x_vec.push_back(-0.75);
			y_vec.push_back(-1.175);
			angle_vec.push_back(0.0 * DEG_TO_RAD);
			semi_width_vec.push_back(0.5 * 0.5);
			semi_height_vec.push_back(0.65 * 0.5);
		}
		else// team B
		{
			x_vec.push_back(-0.75);
			y_vec.push_back(1.175);
			angle_vec.push_back(0.0 * DEG_TO_RAD);
			semi_width_vec.push_back(0.5 * 0.5);
			semi_height_vec.push_back(0.65 * 0.5);
		}
	}	
}

/*! \brief get the walls from the config file (hard-coded version)
 * 
 * \param[out] x_vec x position vector [m]
 * \param[out] y_vec y position vector [m]
 * \param[out] or_vec orientation vector [rad]
 * \param[out] wrapping_vec wrapping radius vector [m]
 */
void config_hard_walls(std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & or_vec, std::vector<double> & wrapping_vec)
{
	// wall 1
	x_vec.push_back(-1.0);
	y_vec.push_back(0.0);
	or_vec.push_back(0.0 * DEG_TO_RAD);
	wrapping_vec.push_back(1.5);

	// wall 1
	x_vec.push_back(1.0);
	y_vec.push_back(0.0);
	or_vec.push_back(180.0 * DEG_TO_RAD);
	wrapping_vec.push_back(1.5);

	// wall 1
	x_vec.push_back(0.0);
	y_vec.push_back(-1.5);
	or_vec.push_back(90.0 * DEG_TO_RAD);
	wrapping_vec.push_back(1.0);

	// wall 1
	x_vec.push_back(0.0);
	y_vec.push_back(1.5);
	or_vec.push_back(-90.0 * DEG_TO_RAD);
	wrapping_vec.push_back(1.0);
}

/*! \brief get the fixed circles from the config file (hard-coded version)
 * 
 * \param[out] x_vec x position vector [m]
 * \param[out] y_vec y position vector [m]
 * \param[out] r_vec radius vector [m]
 */
void config_hard_fixed_circles(std::vector<double> & x_vec, std::vector<double> & y_vec, std::vector<double> & r_vec)
{

}

/*! \brief get the fixed rectangles from the config file (hard-coded version)
 * 
 * \param[out] x_vec x position vector [m]
 * \param[out] y_vec y position vector [m]
 * \param[out] or_vec orientation vector [rad]
 * \param[out] semi_width_vec semi width vector [m]
 * \param[out] semi_height_vec semi height vector [m]
 */
void config_hard_fixed_rectangles(std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & or_vec, std::vector<double> & semi_width_vec, std::vector<double> & semi_height_vec)
{
	// initial bases
	x_vec.push_back(0.49);
	y_vec.push_back(-1.25);
	or_vec.push_back(0.0 * DEG_TO_RAD);
	semi_width_vec.push_back(0.02 * 0.5);
	semi_height_vec.push_back(0.5 * 0.5);

	x_vec.push_back(0.49);
	y_vec.push_back(1.25);
	or_vec.push_back(0.0 * DEG_TO_RAD);
	semi_width_vec.push_back(0.02 * 0.5);
	semi_height_vec.push_back(0.5 * 0.5);

	// target baskets
	x_vec.push_back(-0.75);
	y_vec.push_back(-0.84);
	or_vec.push_back(0.0 * DEG_TO_RAD);
	semi_width_vec.push_back(0.5 * 0.5);
	semi_height_vec.push_back(0.02 * 0.5);

	x_vec.push_back(-0.75);
	y_vec.push_back(0.84);
	or_vec.push_back(0.0 * DEG_TO_RAD);
	semi_width_vec.push_back(0.5 * 0.5);
	semi_height_vec.push_back(0.02 * 0.5);


	// middle obstacle
	x_vec.push_back(0.05);
	y_vec.push_back(0.35);
	or_vec.push_back(0.0 * DEG_TO_RAD);
	semi_width_vec.push_back(0.3 * 0.5);
	semi_height_vec.push_back(0.1 * 0.5);

	x_vec.push_back(0.05);
	y_vec.push_back(-0.35);
	or_vec.push_back(0.0 * DEG_TO_RAD);
	semi_width_vec.push_back(0.3 * 0.5);
	semi_height_vec.push_back(0.1 * 0.5);

	x_vec.push_back(-0.15);
	y_vec.push_back(0.0);
	or_vec.push_back(0.0 * DEG_TO_RAD);
	semi_width_vec.push_back(0.1 * 0.5);
	semi_height_vec.push_back(0.8 * 0.5);

	x_vec.push_back(-0.35);
	y_vec.push_back(0.0);
	or_vec.push_back(0.0 * DEG_TO_RAD);
	semi_width_vec.push_back(0.3 * 0.5);
	semi_height_vec.push_back(0.2 * 0.5);
}

/*! \brief get the moving circles from the config file (hard-coded version)
 * 
 * \param[out] x_vec x position vector [m]
 * \param[out] y_vec y position vector [m]
 * \param[out] r_vec radius vector [m]
 * \param[out] mbs_T1_vec anim .mbs file T1 IDs vector
 * \param[out] mbs_T2_vec anim .mbs file T2 IDs vector
 * \param[out] mbs_R3_vec anim .mbs file R3 IDs vector
 */
void config_hard_moving_circles(std::vector<double> & x_vec, std::vector<double> & y_vec, std::vector<double> & r_vec,
	std::vector<int> & mbs_T1_vec, std::vector<int> & mbs_T2_vec, std::vector<int> & mbs_R3_vec)
{

}

/*! \brief get the moving rectangles from the config file (hard-coded version)
 * 
 * \param[out] x_vec x position vector [m]
 * \param[out] y_vec y position vector [m]
 * \param[out] or_vec orientation vector [rad]
 * \param[out] semi_width_vec semi width vector [m]
 * \param[out] semi_height_vec semi height vector [m]
 * \param[out] mbs_T1_vec anim .mbs file T1 IDs vector
 * \param[out] mbs_T2_vec anim .mbs file T2 IDs vector
 * \param[out] mbs_R3_vec anim .mbs file R3 IDs vector
 */
void config_hard_moving_rectangles(std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & or_vec, std::vector<double> & semi_width_vec, std::vector<double> & semi_height_vec,
	std::vector<int> & mbs_T1_vec, std::vector<int> & mbs_T2_vec, std::vector<int> & mbs_R3_vec)
{

}
