
#include "ListPhysicConfig.hh"
#include "WallShape.hh"
#include "RectangleShape.hh"
#include "UnionShape.hh"
#include "Vector2D.hh"
#include "MovingObject.hh"
#include "FixedObject.hh"
#include "Robot.hh"
#include "CppInterface.hh"
#include "robot_id.h"
#include "config_functions.hh"
#include <iostream>

/*! \brief constructor
 * 
 * \param[in] mbs_data Robotran structure
 */
ListPhysicConfig::ListPhysicConfig(MbsData *mbs_data): ListPhysicElements(mbs_data)
{
	Vector2D init_pos;
	FILE *config_file;
	std::vector<int> mbs_T1_vec, mbs_T2_vec, mbs_R3_vec;
	std::vector<double> x_vec, y_vec, or_vec, wrapping_vec, r_vec, semi_width_vec, semi_height_vec;
	
	config_file = open_config();

	// walls
	config_walls(config_file, x_vec, y_vec, or_vec, wrapping_vec);

	for(unsigned int i=0; i<x_vec.size(); i++)
	{
		init_pos.set_vector(x_vec[i], y_vec[i]);
		add_wall(init_pos, or_vec[i], wrapping_vec[i]);
	}

	x_vec.clear();
	y_vec.clear();
	or_vec.clear();
	wrapping_vec.clear();

	// fixed circles
	config_fixed_circles(config_file, x_vec, y_vec, r_vec);

	for(unsigned int i=0; i<x_vec.size(); i++)
	{
		init_pos.set_vector(x_vec[i], y_vec[i]);
		add_fixed_circle(init_pos, r_vec[i]);
	}

	x_vec.clear();
	y_vec.clear();
	r_vec.clear();

	// fixed rectangles
	config_fixed_rectangles(config_file, x_vec, y_vec, or_vec, semi_width_vec, semi_height_vec);

	for(unsigned int i=0; i<x_vec.size(); i++)
	{
		init_pos.set_vector(x_vec[i], y_vec[i]);
		add_fixed_rect(init_pos, or_vec[i], semi_width_vec[i], semi_height_vec[i]);
	}

	x_vec.clear();
	y_vec.clear();
	or_vec.clear();
	semi_width_vec.clear();
	semi_height_vec.clear();

	// moving circles
	config_moving_circles(config_file, x_vec, y_vec, r_vec, mbs_T1_vec, mbs_T2_vec, mbs_R3_vec);

	for(unsigned int i=0; i<x_vec.size(); i++)
	{
		init_pos.set_vector(x_vec[i], y_vec[i]);
		add_moving_circle(init_pos, r_vec[i], mbs_T1_vec[i], mbs_T2_vec[i], mbs_R3_vec[i]);
	}

	x_vec.clear();
	y_vec.clear();
	r_vec.clear();
	mbs_T1_vec.clear();
	mbs_T2_vec.clear();
	mbs_R3_vec.clear();

	// moving rectangles
	config_moving_rectangles(config_file, x_vec, y_vec, or_vec, semi_width_vec, semi_height_vec, mbs_T1_vec, mbs_T2_vec, mbs_R3_vec);

	for(unsigned int i=0; i<x_vec.size(); i++)
	{
		init_pos.set_vector(x_vec[i], y_vec[i]);
		add_moving_rect(init_pos, or_vec[i], semi_width_vec[i], semi_height_vec[i],mbs_T1_vec[i], mbs_T2_vec[i], mbs_R3_vec[i]);
	}

	x_vec.clear();
	y_vec.clear();
	or_vec.clear();
	semi_width_vec.clear();
	semi_height_vec.clear();
	mbs_T1_vec.clear();
	mbs_T2_vec.clear();
	mbs_R3_vec.clear();
	
	close_config(config_file);
}

/*! \brief destructor
 */
ListPhysicConfig::~ListPhysicConfig()
{

}
