#include "config_functions.hh"
#include "cmake_config.h"
#include "robot_id.h"
#include "config_file.h"
#include <cstring>
#include <cmath>

#define DEG_TO_RAD (M_PI / 180.0)

#define MAX_STR_LENGTH 128
#define MAX_LINE_LENGTH 1024

/*! \brief open the config file
 * 
 * \return the config file open
 */
FILE* open_config()
{
	#ifndef HARDCODED_CONFIG
	FILE *config_file;

	config_file = fopen(PROJECT_SOURCE_DIR"/../userFiles/config/config_file.txt", "r");

	if (config_file == NULL)
	{
		std::cout << "Error: cannot open " << PROJECT_SOURCE_DIR << "/../userFiles/config/config_file.txt !" << std::endl;
		exit(EXIT_FAILURE);
	}

	return config_file;
	#else
	return NULL;
	#endif
}

/*! \brief close the configuration file
 * 
 * \param[out] config_file file (FILE* format) to close
 */
void close_config(FILE *config_file)
{
	#ifndef HARDCODED_CONFIG
	if (config_file != NULL)
	{
		fclose(config_file);
	}
	#endif
}

/*! \brief check if a string can be a float
 * 
 * \param[in] str_float string (char*) to check
 * \return 1 if possible float, 0 otherwise
 */
int possible_float(char *str_float)
{
	int cur_int;
	char cur_char;

	// loop on all the characters
	for(int i=0; cur_char=str_float[i]; i++)
	{
		if ((cur_char == '-') || (cur_char == '.') || (cur_char == ','))
		{
			continue;
		}
		else
		{
			cur_int = (int) (cur_char - '0');

			if (cur_int < 0 || cur_int > 9)
			{
				return 0;
			}
		}
	}

	return 1;
}

/*! \brief convert a string to a float (double)
 * 
 * \param[in] str_float string (char*) to convert to a float
 * \return requested float (double)
 */
double char_to_float(char *str_float)
{
	// variables declaration
	char cur_char;
	int cur_int, dec_count;
	int flag_neg, flag_comma;
	double sum_float;

	// variables initialization
	flag_neg   = 0;
	flag_comma = 0;

	dec_count = 0;

	sum_float = 0.0;
	
	// loop on all the characters
	for(int i=0; cur_char=str_float[i]; i++)
	{
		if (cur_char == '-') // negative sign
		{
			if (flag_neg)
			{
				std::cout << "config_functions error: 2 negative signs !" << std::endl;
				exit(EXIT_FAILURE);
			}
			else
			{
				flag_neg = 1;
			}
		}
		else if ((cur_char == '.') || (cur_char == ',')) // comma
		{
			if (flag_comma)
			{
				std::cout << "config_functions error: 2 commas !" << std::endl;
				exit(EXIT_FAILURE);
			}
			else
			{
				flag_comma = 1;
			}
		}
		else // character to convert to an 'int'
		{
			cur_int = (int) (cur_char - '0');

			if (cur_int < 0 || cur_int > 9)
			{
				std::cout << "config_functions error: integer from char should be in [0 ; 9], and not " << cur_int << " !" << std::endl;
				exit(EXIT_FAILURE);
			}

			if (!flag_comma)
			{
				sum_float = 10.0*sum_float + cur_int;
			}
			else
			{
				dec_count++;
				sum_float += pow(10.0, -dec_count) * cur_int;
			}
		}
	}

	// add negative contribution
	if (flag_neg)
	{
		sum_float = -sum_float;
	}

	return sum_float;
}

/*! \brief get the int value associated with a field name
 * 
 * \param[in] config_file configuration file 'config_file.txt'
 * \param[in] name name of the requested field
 * \return requested int value
 */
int config_int(FILE *config_file, char const *name)
{
	#ifndef HARDCODED_CONFIG
	int cur_flag, result;
	char str[MAX_STR_LENGTH];

	do // loop on all the lines of the file
	{
		cur_flag = fscanf(config_file, "%s : %d", str, &result);

		if ((cur_flag == 2) && !strcmp(str, name))
		{
			rewind(config_file);
			return result;
		}
	} while (cur_flag != EOF);

	std::cout << "Error: int value of " << name << " not found !" << std::endl;
	exit(EXIT_FAILURE);
	#else
	return 0;
	#endif
}

/*! \brief get the double value associated with a field name
 * 
 * \param[in] config_file configuration file 'config_file.txt'
 * \param[in] name name of the requested field
 * \return requested double value
 */
double config_double(FILE *config_file, char const *name)
{
	#ifndef HARDCODED_CONFIG
	int cur_flag;
	double result;
	char str[MAX_STR_LENGTH];

	do // loop on all the lines of the file
	{
		cur_flag = fscanf(config_file, "%s : %lf", str, &result);

		if ((cur_flag == 2) && !strcmp(str, name))
		{
			rewind(config_file);
			return result;
		}
	} while (cur_flag != EOF);

	std::cout << "Error: double value of " << name << " not found !" << std::endl;
	exit(EXIT_FAILURE);
	#else
	return 0.0;
	#endif
}

/*! \brief get the char* value associated with a field name
 * 
 * \param[in] config_file configuration file 'config_file.txt'
 * \param[in] name name of the requested field
 * \return requested char* value
 *
 * Attention: the char* returned must be freed to avoid memory loss.
 */
char* config_char(FILE *config_file, char const *name)
{
	#ifndef HARDCODED_CONFIG
	int cur_flag;
	char str[MAX_STR_LENGTH], value[MAX_STR_LENGTH];
	char *result;

	do // loop on all the lines of the file
	{
		cur_flag = fscanf(config_file, "%s : %s", str, value);

		if ((cur_flag == 2) && !strcmp(str, name))
		{
			result = (char*) malloc(sizeof(value));
			strcpy(result, value);

			rewind(config_file);
			return result;
		}
	} while (cur_flag != EOF);

	std::cout << "Error: char* value of " << name << " not found !" << std::endl;
	exit(EXIT_FAILURE);
	#else
	return NULL;
	#endif
}

/*! \brief get the targets from the config file
 * 
 * \param[in] config_file configuration file 'config_file.txt'
 * \param[out] x_vec x position vector [m]
 * \param[out] y_vec y position vector [m]
 * \param[out] mbs_T1_vec anim .mbs file T1 IDs vector
 * \param[out] mbs_T2_vec anim .mbs file T2 IDs vector
 * \param[out] mbs_T3_vec anim .mbs file T3 IDs vector
 * \param[out] value_vec target values vector
 */
void config_targets(FILE *config_file, std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<int> & mbs_T1_vec, std::vector<int> & mbs_T2_vec, std::vector<int> & mbs_T3_vec, std::vector<int> & value_vec)
{
	#ifndef HARDCODED_CONFIG
	int cur_flag;
	int cur_T1, cur_T2, cur_T3, cur_value;
	char str_x[MAX_STR_LENGTH], str_y[MAX_STR_LENGTH];
	char str[MAX_STR_LENGTH];

	do // loop on all the lines of the file
	{
		cur_flag = fscanf(config_file, "%s : position( %s ; %s ) ; mbs_joints( %d ; %d ; %d ) ; value( %d )",
			str, str_x, str_y, &cur_T1, &cur_T2, &cur_T3, &cur_value);

		if ((cur_flag == 7) && !strcmp(str, "target") && possible_float(str_x) && possible_float(str_y))
		{
			x_vec.push_back(char_to_float(str_x));
			y_vec.push_back(char_to_float(str_y));

			mbs_T1_vec.push_back(cur_T1);
			mbs_T2_vec.push_back(cur_T2);
			mbs_T3_vec.push_back(cur_T3);

			value_vec.push_back(cur_value);
		}

	} while (cur_flag != EOF);

	rewind(config_file);
	#else
	config_hard_targets(x_vec, y_vec, mbs_T1_vec, mbs_T2_vec, mbs_T3_vec, value_vec);
	#endif
}

/*! \brief get the bases from the config file
 * 
 * \param[in] config_file configuration file 'config_file.txt'
 * \param[in] flag_init 1 if init bases, 0 if target bases
 * \param[in] team_ID team ID
 * \param[out] x_vec x position vector of the rectangle [m]
 * \param[out] y_vec y position vector of the rectangle [m]
 * \param[out] angle_vec orientation angle of the rectangle [m]
 * \param[out] semi_width_vec rectangle semi width [m]
 * \param[out] semi_height_vec rectangle semi height [m]
 */
void config_target_bases(FILE *config_file, int flag_init, int team_ID, std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & angle_vec, std::vector<double> & semi_width_vec, std::vector<double> & semi_height_vec)
{
	#ifndef HARDCODED_CONFIG
	int cur_flag;
	char str_x_rect[MAX_STR_LENGTH], str_y_rect[MAX_STR_LENGTH], str_angle_rect[MAX_STR_LENGTH], str_width_rect[MAX_STR_LENGTH], str_height_rect[MAX_STR_LENGTH];
	char str[MAX_STR_LENGTH];

	do // loop on all the lines of the file
	{
		cur_flag = fscanf(config_file, "%s : position( %s ; %s ) ; orientation( %s ) ; width( %s ) ; height( %s )",
			str, str_x_rect, str_y_rect, str_angle_rect, str_width_rect, str_height_rect);

		if (!possible_float(str_x_rect) || !possible_float(str_y_rect) || !possible_float(str_angle_rect) || !possible_float(str_width_rect) || !possible_float(str_height_rect))
		{
			continue;
		}

		if ((flag_init && ((cur_flag == 6) && ( (!strcmp(str, "init_basis_team_A") && (team_ID == TEAM_A)) || (!strcmp(str, "init_basis_team_B") && (team_ID == TEAM_B)) ))) // init bases
			|| ((!flag_init) && ((cur_flag == 6) && ( (!strcmp(str, "target_basis_team_A") && (team_ID == TEAM_A)) || (!strcmp(str, "target_basis_team_B") && (team_ID == TEAM_B)) )))) // target bases
		{
			x_vec.push_back(char_to_float(str_x_rect));
			y_vec.push_back(char_to_float(str_y_rect));
			angle_vec.push_back(char_to_float(str_angle_rect) * DEG_TO_RAD);
			semi_width_vec.push_back(char_to_float(str_width_rect) * 0.5);
			semi_height_vec.push_back(char_to_float(str_height_rect) * 0.5);
		}

	} while (cur_flag != EOF);

	rewind(config_file);
	#else
	config_hard_target_bases(flag_init, team_ID, x_vec, y_vec, angle_vec, semi_width_vec, semi_height_vec);
	#endif
}

/*! \brief get the walls from the config file
 * 
 * \param[in] config_file configuration file 'config_file.txt'
 * \param[out] x_vec x position vector [m]
 * \param[out] y_vec y position vector [m]
 * \param[out] or_vec orientation vector [rad]
 * \param[out] wrapping_vec wrapping radius vector [m]
 */
void config_walls(FILE *config_file, std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & or_vec, std::vector<double> & wrapping_vec)
{
	#ifndef HARDCODED_CONFIG
	int cur_flag;
	char str_x[MAX_STR_LENGTH], str_y[MAX_STR_LENGTH], str_or[MAX_STR_LENGTH], str_wrap[MAX_STR_LENGTH];
	char str[MAX_STR_LENGTH];

	do // loop on all the lines of the file
	{
		cur_flag = fscanf(config_file, "%s : position( %s ; %s ) ; orientation( %s ) ; wrapping( %s )",
			str, str_x, str_y, str_or, str_wrap);

		if (!possible_float(str_x) || !possible_float(str_y) || !possible_float(str_or) || !possible_float(str_wrap))
		{
			continue;
		}

		if ((cur_flag == 5) && !strcmp(str, "wall"))
		{
			x_vec.push_back(char_to_float(str_x));
			y_vec.push_back(char_to_float(str_y));

			or_vec.push_back(char_to_float(str_or) * DEG_TO_RAD);
			wrapping_vec.push_back(char_to_float(str_wrap));
		}

	} while (cur_flag != EOF);

	rewind(config_file);
	#else
	config_hard_walls(x_vec, y_vec, or_vec, wrapping_vec);
	#endif
}

/*! \brief get the fixed circles from the config file
 * 
 * \param[in] config_file configuration file 'config_file.txt'
 * \param[out] x_vec x position vector [m]
 * \param[out] y_vec y position vector [m]
 * \param[out] r_vec radius vector [m]
 */
void config_fixed_circles(FILE *config_file, std::vector<double> & x_vec, std::vector<double> & y_vec, std::vector<double> & r_vec)
{
	#ifndef HARDCODED_CONFIG
	int cur_flag;
	char str_x[MAX_STR_LENGTH], str_y[MAX_STR_LENGTH], str_r[MAX_STR_LENGTH];
	char str[MAX_STR_LENGTH];

	do // loop on all the lines of the file
	{
		cur_flag = fscanf(config_file, "%s : position( %s ; %s ) ; radius( %s )",
			str, str_x, str_y, str_r);

		if (!possible_float(str_x) || !possible_float(str_y) || !possible_float(str_r))
		{
			continue;
		}

		if ((cur_flag == 4) && !strcmp(str, "fixed_circle"))
		{
			x_vec.push_back(char_to_float(str_x));
			y_vec.push_back(char_to_float(str_y));

			r_vec.push_back(char_to_float(str_r));
		}

	} while (cur_flag != EOF);

	rewind(config_file);
	#else
	config_hard_fixed_circles(x_vec, y_vec, r_vec);
	#endif
}

/*! \brief get the fixed rectangles from the config file
 * 
 * \param[in] config_file configuration file 'config_file.txt'
 * \param[out] x_vec x position vector [m]
 * \param[out] y_vec y position vector [m]
 * \param[out] or_vec orientation vector [rad]
 * \param[out] semi_width_vec semi width vector [m]
 * \param[out] semi_height_vec semi height vector [m]
 */
void config_fixed_rectangles(FILE *config_file, std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & or_vec, std::vector<double> & semi_width_vec, std::vector<double> & semi_height_vec)
{
	#ifndef HARDCODED_CONFIG
	int cur_flag;
	char str_x[MAX_STR_LENGTH], str_y[MAX_STR_LENGTH], str_or[MAX_STR_LENGTH], str_width[MAX_STR_LENGTH], str_height[MAX_STR_LENGTH];
	char str[MAX_STR_LENGTH];

	do // loop on all the lines of the file
	{
		cur_flag = fscanf(config_file, "%s : position( %s ; %s ) ; orientation( %s ) ; width( %s ) ; height( %s )",
			str, str_x, str_y, str_or, str_width, str_height);

		if (!possible_float(str_x) || !possible_float(str_y) || !possible_float(str_or) || !possible_float(str_width) || !possible_float(str_height))
		{
			continue;
		}

		if ((cur_flag == 6) && !strcmp(str, "fixed_rectangle"))
		{
			x_vec.push_back(char_to_float(str_x));
			y_vec.push_back(char_to_float(str_y));

			or_vec.push_back(char_to_float(str_or) * DEG_TO_RAD);
			
			semi_width_vec.push_back(char_to_float(str_width) * 0.5);
			semi_height_vec.push_back(char_to_float(str_height) * 0.5);
		}

	} while (cur_flag != EOF);

	rewind(config_file);
	#else
	config_hard_fixed_rectangles(x_vec, y_vec, or_vec, semi_width_vec, semi_height_vec);
	#endif
}

/*! \brief get the moving circles from the config file
 * 
 * \param[in] config_file configuration file 'config_file.txt'
 * \param[out] x_vec x position vector [m]
 * \param[out] y_vec y position vector [m]
 * \param[out] r_vec radius vector [m]
 * \param[out] mbs_T1_vec anim .mbs file T1 IDs vector
 * \param[out] mbs_T2_vec anim .mbs file T2 IDs vector
 * \param[out] mbs_R3_vec anim .mbs file R3 IDs vector
 */
void config_moving_circles(FILE *config_file, std::vector<double> & x_vec, std::vector<double> & y_vec, std::vector<double> & r_vec,
	std::vector<int> & mbs_T1_vec, std::vector<int> & mbs_T2_vec, std::vector<int> & mbs_R3_vec)
{
	#ifndef HARDCODED_CONFIG
	int cur_flag, cur_T1, cur_T2, cur_R3;
	char str_x[MAX_STR_LENGTH], str_y[MAX_STR_LENGTH], str_r[MAX_STR_LENGTH];
	char str[MAX_STR_LENGTH];

	do // loop on all the lines of the file
	{
		cur_flag = fscanf(config_file, "%s : position( %s ; %s ) ; radius( %s ) ; mbs_joints( %d ; %d ; %d )",
			str, str_x, str_y, str_r, &cur_T1, &cur_T2, &cur_R3);

		if (!possible_float(str_x) || !possible_float(str_y) || !possible_float(str_r))
		{
			continue;
		}

		if ((cur_flag == 7) && !strcmp(str, "moving_circle"))
		{
			x_vec.push_back(char_to_float(str_x));
			y_vec.push_back(char_to_float(str_y));

			r_vec.push_back(char_to_float(str_r));

			mbs_T1_vec.push_back(cur_T1);
			mbs_T2_vec.push_back(cur_T2);
			mbs_R3_vec.push_back(cur_R3);
		}

	} while (cur_flag != EOF);

	rewind(config_file);
	#else
	config_hard_moving_circles(x_vec, y_vec, r_vec,	mbs_T1_vec, mbs_T2_vec, mbs_R3_vec);
	#endif
}

/*! \brief get the moving rectangles from the config file
 * 
 * \param[in] config_file configuration file 'config_file.txt'
 * \param[out] x_vec x position vector [m]
 * \param[out] y_vec y position vector [m]
 * \param[out] or_vec orientation vector [rad]
 * \param[out] semi_width_vec semi width vector [m]
 * \param[out] semi_height_vec semi height vector [m]
 * \param[out] mbs_T1_vec anim .mbs file T1 IDs vector
 * \param[out] mbs_T2_vec anim .mbs file T2 IDs vector
 * \param[out] mbs_R3_vec anim .mbs file R3 IDs vector
 */
void config_moving_rectangles(FILE *config_file, std::vector<double> & x_vec, std::vector<double> & y_vec,
	std::vector<double> & or_vec, std::vector<double> & semi_width_vec, std::vector<double> & semi_height_vec,
	std::vector<int> & mbs_T1_vec, std::vector<int> & mbs_T2_vec, std::vector<int> & mbs_R3_vec)
{
	#ifndef HARDCODED_CONFIG
	int cur_flag, cur_T1, cur_T2, cur_R3;
	char str_x[MAX_STR_LENGTH], str_y[MAX_STR_LENGTH], str_or[MAX_STR_LENGTH], str_width[MAX_STR_LENGTH], str_height[MAX_STR_LENGTH];
	char str[MAX_STR_LENGTH];

	do // loop on all the lines of the file
	{
		cur_flag = fscanf(config_file, "%s : position( %s ; %s ) ; orientation( %s ) ; width( %s ) ; height( %s ) ; mbs_joints( %d ; %d ; %d )",
			str, str_x, str_y, str_or, str_width, str_height, &cur_T1, &cur_T2, &cur_R3);

		if (!possible_float(str_x) || !possible_float(str_y) || !possible_float(str_or) || !possible_float(str_width) || !possible_float(str_height))
		{
			continue;
		}

		if ((cur_flag == 9) && !strcmp(str, "moving_rectangle"))
		{
			x_vec.push_back(char_to_float(str_x));
			y_vec.push_back(char_to_float(str_y));

			or_vec.push_back(char_to_float(str_or) * DEG_TO_RAD);
			
			semi_width_vec.push_back(char_to_float(str_width) * 0.5);
			semi_height_vec.push_back(char_to_float(str_height) * 0.5);

			mbs_T1_vec.push_back(cur_T1);
			mbs_T2_vec.push_back(cur_T2);
			mbs_R3_vec.push_back(cur_R3);
		}

	} while (cur_flag != EOF);

	rewind(config_file);
	#else
	config_hard_moving_rectangles(x_vec, y_vec, or_vec, semi_width_vec, semi_height_vec, mbs_T1_vec, mbs_T2_vec, mbs_R3_vec);
	#endif
}
