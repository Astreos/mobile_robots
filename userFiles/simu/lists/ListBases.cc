#include "ListBases.hh"
#include <iostream>
#include <stdlib.h>
#include "config_functions.hh"

/*! \brief constructor
 */
ListBases::ListBases()
{
	FILE *config_file;
	std::vector<double> x_vec_A, y_vec_A, angle_vec_A, semi_width_vec_A, semi_height_vec_A;
	std::vector<double> x_vec_B, y_vec_B, angle_vec_B, semi_width_vec_B, semi_height_vec_B;

	config_file = open_config();

	config_target_bases(config_file, 0, TEAM_A, x_vec_A, y_vec_A, angle_vec_A, semi_width_vec_A, semi_height_vec_A);
	config_target_bases(config_file, 0, TEAM_B, x_vec_B, y_vec_B, angle_vec_B, semi_width_vec_B, semi_height_vec_B);

	for(unsigned int i=0; i<x_vec_A.size(); i++)
	{
		team_A_bases.push_back(new Basis(x_vec_A[i], y_vec_A[i], angle_vec_A[i], semi_width_vec_A[i], semi_height_vec_A[i], TEAM_A));
	}

	for(unsigned int i=0; i<x_vec_B.size(); i++)
	{
		team_B_bases.push_back(new Basis(x_vec_B[i], y_vec_B[i], angle_vec_B[i], semi_width_vec_B[i], semi_height_vec_B[i], TEAM_B));
	}

	close_config(config_file);
}

/*! \brief destructor
 */
ListBases::~ListBases()
{
	for(unsigned int i=0; i<team_A_bases.size(); i++)
	{
		delete team_A_bases[i];
	}

	for(unsigned int i=0; i<team_B_bases.size(); i++)
	{
		delete team_B_bases[i];
	}
}

/*! \brief check if the target is fully inside a basis of a given team
 * 
 * \param[in] cur_target current target to check
 * \param[in] team_id team ID
 * \return 1 if fully inside any basis of this team, 0 otherwise
 */
int ListBases::target_full_inside_team(Target *cur_target, int team_id)
{
	switch (team_id)
	{
		case TEAM_A:
			for(unsigned int i=0; i<team_A_bases.size(); i++)
			{
				if (team_A_bases[i]->target_full_inside(cur_target))
				{
					return 1;
				}
			}
			break;

		case TEAM_B:
			for(unsigned int i=0; i<team_B_bases.size(); i++)
			{
				if (team_B_bases[i]->target_full_inside(cur_target))
				{
					return 1;
				}
			}
			break;
	
		default:
			std::cout << "Error: unknown team ID (" << team_id << ") !" << std::endl;
			exit(EXIT_FAILURE);
	}

	return 0;
}

/*! \brief check if the target is fully inside any basis
 * 
 * \param[in] cur_target current target to check
 * \return 1 if fully inside any basis, 0 otherwise
 */
int ListBases::target_full_inside_any(Target *cur_target)
{
	if (target_full_inside_team(cur_target, TEAM_A))
	{
		return 1;
	}
	else if (target_full_inside_team(cur_target, TEAM_B))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
