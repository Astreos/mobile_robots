#include "ListTargets.hh"
#include "config_functions.hh"
#include <iostream>
#include <stdlib.h>

/*! \brief constructor
 */
ListTargets::ListTargets()
{
	FILE *config_file;
	std::vector<double> x_vec, y_vec;
	std::vector<int> mbs_T1_vec, mbs_T2_vec, mbs_T3_vec, value_vec;

	config_file = open_config();

	config_targets(config_file, x_vec, y_vec, mbs_T1_vec, mbs_T2_vec, mbs_T3_vec, value_vec);

	for(unsigned int i=0; i<value_vec.size(); i++)
	{
		target_list.push_back(new Target( x_vec[i], y_vec[i] , 0.0, mbs_T1_vec[i], mbs_T2_vec[i], mbs_T3_vec[i], value_vec[i]));
	}

	close_config(config_file);
}

/*! \brief destructor
 */
ListTargets::~ListTargets()
{
	for(unsigned int i=0; i<target_list.size(); i++)
	{
		delete target_list[i];
	}
}

/*! \brief fill the IDs of T1, T2 and T3
 * 
 * \param[in] target_id ID of the target
 * \param[out] T1 T1 joint ID
 * \param[out] T2 T2 joint ID
 * \param[out] T3 T3 joint ID
 */
void ListTargets::fill_T1_T2_T3(int target_id, int &T1, int &T2, int &T3)
{
	// safety
	if (target_id >= target_list.size())
	{
		std::cout << "Error: target_id (" << target_id << ") >= target_list.size() (" << target_list.size() << ") !" << std::endl;
		exit(EXIT_FAILURE);
	}

	target_list[target_id]->fill_T1_T2_T3(T1, T2, T3);
}

/*! \brief fill the 3D position
 * 
 * \param[in] target_id ID of the target
 * \param[out] x x coordinate [m]
 * \param[out] y y coordinate [m]
 * \param[out] z z coordinate [m]
 */
void ListTargets::get_pos(int target_id, double &x, double &y, double &z)
{
	// safety
	if (target_id >= target_list.size())
	{
		std::cout << "Error: target_id (" << target_id << ") >= target_list.size() (" << target_list.size() << ") !" << std::endl;
		exit(EXIT_FAILURE);
	}

	target_list[target_id]->get_pos(x, y, z);
}
