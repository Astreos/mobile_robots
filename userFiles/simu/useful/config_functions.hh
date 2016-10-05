/*! 
 * \author Nicolas Van der Noot
 * \file config_functions.hh
 * \brief config file reading functions
 */

#ifndef _CONFIG_FUNCTIONS_HH_
#define _CONFIG_FUNCTIONS_HH_

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

FILE* open_config();
void close_config(FILE *config_file);
int possible_float(char *str_float);
double char_to_float(char *str_float);
int config_int(FILE *config_file, char const *name);
double config_double(FILE *config_file, char const *name);
char* config_char(FILE *config_file, char const *name);

// not hardcoded
void config_targets(FILE *config_file, std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<int> & mbs_T1_vec, std::vector<int> & mbs_T2_vec, std::vector<int> & mbs_T3_vec, std::vector<int> & value_vec);
void config_target_bases(FILE *config_file, int flag_init, int team_ID, std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & angle_vec, std::vector<double> & semi_width_vec, std::vector<double> & semi_height_vec);
void config_walls(FILE *config_file, std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & or_vec, std::vector<double> & wrapping_vec);
void config_fixed_circles(FILE *config_file, std::vector<double> & x_vec, std::vector<double> & y_vec, std::vector<double> & r_vec);
void config_fixed_rectangles(FILE *config_file, std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & or_vec, std::vector<double> & semi_width_vec, std::vector<double> & semi_height_vec);
void config_moving_circles(FILE *config_file, std::vector<double> & x_vec, std::vector<double> & y_vec, std::vector<double> & r_vec,
	std::vector<int> & mbs_T1_vec, std::vector<int> & mbs_T2_vec, std::vector<int> & mbs_R3_vec);
void config_moving_rectangles(FILE *config_file, std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & or_vec, std::vector<double> & semi_width_vec, std::vector<double> & semi_height_vec,
	std::vector<int> & mbs_T1_vec, std::vector<int> & mbs_T2_vec, std::vector<int> & mbs_R3_vec);

// hardcoded
void config_hard_targets(std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<int> & mbs_T1_vec, std::vector<int> & mbs_T2_vec, std::vector<int> & mbs_T3_vec, std::vector<int> & value_vec);
void config_hard_target_bases(int flag_init, int team_ID, std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & angle_vec, std::vector<double> & semi_width_vec, std::vector<double> & semi_height_vec);
void config_hard_walls(std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & or_vec, std::vector<double> & wrapping_vec);
void config_hard_fixed_circles(std::vector<double> & x_vec, std::vector<double> & y_vec, std::vector<double> & r_vec);
void config_hard_fixed_rectangles(std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & or_vec, std::vector<double> & semi_width_vec, std::vector<double> & semi_height_vec);
void config_hard_moving_circles(std::vector<double> & x_vec, std::vector<double> & y_vec, std::vector<double> & r_vec,
	std::vector<int> & mbs_T1_vec, std::vector<int> & mbs_T2_vec, std::vector<int> & mbs_R3_vec);
void config_hard_moving_rectangles(std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & or_vec, std::vector<double> & semi_width_vec, std::vector<double> & semi_height_vec,
	std::vector<int> & mbs_T1_vec, std::vector<int> & mbs_T2_vec, std::vector<int> & mbs_R3_vec);

#endif
