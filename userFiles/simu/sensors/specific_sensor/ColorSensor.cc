#include "ColorSensor.hh"
#include "CppInterface.hh"
#include "Robot.hh"
#include "user_model.h"
#include <cmath>

//color names short
enum {WW, GG, KK, YY, BB, RR};

#define NB_MAP_X_COLOR 20 // number of color squares along the X axis
#define NB_MAP_Y_COLOR 30 // number of color squares along the Y axis

// map with all the color along the squares
int map_color[NB_MAP_Y_COLOR][NB_MAP_X_COLOR] = {
	{YY, YY, YY, YY, YY, GG, GG, WW, GG, KK, KK, KK, GG, GG, GG, BB, BB, BB, BB, BB} ,
	{YY, YY, YY, YY, YY, GG, GG, GG, KK, WW, KK, GG, KK, KK, WW, BB, BB, BB, BB, BB} ,
	{YY, YY, YY, YY, YY, GG, KK, GG, KK, KK, WW, GG, GG, GG, KK, BB, BB, BB, BB, BB} ,
	{YY, YY, YY, YY, YY, KK, WW, GG, WW, GG, KK, KK, KK, WW, WW, BB, BB, BB, BB, BB} ,
	{YY, YY, YY, YY, YY, WW, GG, GG, GG, GG, GG, WW, GG, GG, GG, BB, BB, BB, BB, BB} ,
	{YY, YY, YY, YY, YY, KK, GG, WW, GG, WW, KK, WW, WW, WW, GG, WW, GG, KK, KK, KK} ,
	{WW, KK, WW, WW, WW, GG, KK, GG, GG, WW, WW, GG, KK, KK, WW, WW, WW, GG, KK, WW} ,
	{WW, KK, KK, WW, GG, KK, KK, GG, KK, WW, WW, GG, WW, GG, WW, GG, WW, GG, KK, KK} ,
	{KK, WW, KK, WW, KK, WW, GG, GG, GG, GG, KK, WW, KK, WW, KK, KK, GG, WW, WW, KK} ,
	{WW, WW, GG, GG, WW, WW, GG, WW, GG, GG, GG, KK, KK, KK, KK, KK, KK, WW, WW, WW} ,
	{KK, KK, WW, GG, WW, KK, GG, KK, WW, KK, KK, GG, KK, WW, GG, KK, KK, KK, KK, KK} ,
	{WW, WW, GG, KK, GG, WW, WW, GG, RR, RR, RR, RR, WW, KK, GG, GG, GG, GG, GG, KK} ,
	{KK, GG, KK, GG, KK, WW, WW, KK, RR, WW, GG, WW, KK, KK, KK, KK, KK, WW, KK, KK} ,
	{GG, GG, GG, KK, KK, GG, WW, KK, RR, GG, KK, KK, KK, KK, KK, KK, WW, WW, KK, KK} ,
	{WW, WW, GG, WW, WW, RR, RR, RR, RR, KK, WW, GG, GG, WW, KK, KK, WW, WW, KK, WW} ,
	{WW, KK, GG, KK, WW, RR, RR, RR, RR, WW, GG, KK, KK, KK, GG, KK, KK, WW, KK, KK} ,
	{KK, KK, GG, KK, KK, WW, WW, WW, RR, WW, KK, GG, KK, WW, WW, KK, KK, KK, GG, KK} ,
	{WW, KK, WW, WW, WW, KK, GG, WW, RR, KK, GG, WW, WW, WW, GG, GG, KK, WW, KK, GG} ,
	{GG, KK, KK, GG, WW, WW, KK, WW, RR, RR, RR, RR, KK, WW, KK, KK, GG, WW, KK, WW} ,
	{WW, GG, KK, WW, KK, GG, GG, WW, GG, GG, GG, GG, KK, WW, GG, GG, GG, WW, KK, KK} ,
	{GG, WW, KK, GG, WW, KK, KK, GG, KK, WW, GG, WW, GG, WW, WW, GG, KK, GG, KK, WW} ,
	{KK, GG, WW, WW, KK, GG, KK, WW, GG, WW, GG, GG, KK, WW, KK, KK, WW, WW, KK, KK} ,
	{GG, WW, KK, KK, GG, GG, KK, GG, KK, GG, WW, WW, GG, KK, GG, GG, KK, WW, WW, GG} ,
	{KK, GG, KK, KK, WW, KK, KK, WW, GG, KK, WW, GG, GG, GG, GG, WW, WW, WW, KK, WW} ,
	{BB, BB, BB, BB, BB, WW, KK, KK, GG, WW, KK, KK, GG, GG, GG, GG, GG, KK, GG, GG} ,
	{BB, BB, BB, BB, BB, WW, GG, WW, KK, KK, WW, KK, WW, GG, GG, YY, YY, YY, YY, YY} ,
	{BB, BB, BB, BB, BB, KK, WW, GG, GG, WW, GG, WW, WW, KK, GG, YY, YY, YY, YY, YY} ,
	{BB, BB, BB, BB, BB, KK, GG, GG, WW, KK, WW, WW, WW, GG, KK, YY, YY, YY, YY, YY} ,
	{BB, BB, BB, BB, BB, GG, WW, KK, WW, GG, GG, GG, KK, GG, KK, YY, YY, YY, YY, YY} ,
	{BB, BB, BB, BB, BB, GG, GG, KK, GG, GG, WW, KK, KK, WW, KK, YY, YY, YY, YY, YY}
};

/*! \brief contructor
 * 
 * \param[in] mbs_data Robotran structure
 * \param[in] robot_id ID of the robot
 */
ColorSensor::ColorSensor(MbsData *mbs_data, int robot_id): Sensor(mbs_data)
{
	flag_init = 0;
	this->robot_id = robot_id;

	// map dimensions
	map_x = 2.0;
	map_y = 3.0;

	map_semi_x = map_x / 2.0;
	map_semi_y = map_y / 2.0;

	color_seen = MAP_OUT_COLOR;
}

/*! \brief destructor
 */
ColorSensor::~ColorSensor()
{
	
}

/*! \brief update the sensor
 */
void ColorSensor::update()
{
	// get robot
	if (!flag_init)
	{
		flag_init = 1;
		rob = get_robot(robot_id);
	}

	// new robot position
	color_seen = compute_color(rob->get_pose_x(), rob->get_pose_y());
}

/*! \brief compute the color, depending on the map position
 *
 * \param[in] x map X position [m]
 * \param[in] y map Y position [m]
 * \return color, according to code
 */
int ColorSensor::compute_color(double x, double y)
{
	// variables declaration
	int x_id, y_id;
	double x_ratio, y_ratio;

	// check inside map
	if (x < -map_semi_x || x > map_semi_x || y < -map_semi_y || y > map_semi_y)
	{
		return MAP_OUT_COLOR;
	}

	// brown obstacles
	if (x < -0.5)
	{
		if ((0.83 < y && y < 0.85) || (-0.85 < y && y < -0.83))
		{
			return MAP_BROWN;
		}
	}

	if (y < -1.0 || y > 1.0)
	{
		if (0.48 < x && x < 0.5)
		{
			return MAP_BROWN;
		}
	}

	// start bases
	if (x >= 0.5)
	{
		if (y >= 1.0)
		{
			return MAP_BLUE;
		}

		if (y <= -1.0)
		{
			return MAP_YELLOW;
		}
	}

	// target bases
	if (x <= -0.5)
	{
		if (y >= 0.85)
		{
			return MAP_YELLOW;
		}

		if (y <= -0.85)
		{
			return MAP_BLUE;
		}
	}

	// position ratio (limited in the [0;1] range)
	x_ratio = (x + map_semi_x) / map_x;
	y_ratio = 1.0 - ((y + map_semi_y) / map_y);

	// limits
	if (x_ratio <= 0.0)
	{
		x_id = 0;
	}
	else if(x_ratio >= 1.0)
	{
		x_id = NB_MAP_X_COLOR - 1;
	}

	if (y_ratio <= 0.0)
	{
		y_id = 0;
	}
	else if(y_ratio >= 1.0)
	{
		y_id = NB_MAP_Y_COLOR - 1;
	}

	// map_color IDs
	x_id = floor(x_ratio * NB_MAP_X_COLOR);
	y_id = floor(y_ratio * NB_MAP_Y_COLOR);

	// return color
	switch (map_color[y_id][x_id])
	{
		case WW : return MAP_WHITE;
		case GG : return MAP_GRAY;
		case KK : return MAP_BLACK;
		case YY : return MAP_YELLOW;
		case BB : return MAP_BLUE;
		case RR : return MAP_RED;
	
		default:
			std::cout << "ColorSensor error: unknown tabular color (" << map_color[y_id][x_id] << ") !" << std::endl;
			exit(EXIT_FAILURE);
	}

	// should not be reached
	return MAP_OUT_COLOR;
}
