/*! 
 * \author Nicolas Van der Noot
 * \file ColorSensor.hh
 * \brief ColorSensor class
 */

#ifndef _COLOR_SENSOR_HH_
#define _COLOR_SENSOR_HH_

#include "Sensor.hh"
#include "TimeGestion.hh"

/// colors definition for the map
enum {MAP_WHITE, MAP_GRAY, MAP_BLACK, MAP_YELLOW, MAP_BLUE, MAP_RED, MAP_BROWN, MAP_OUT_COLOR, NB_MAP_COLORS};

/*! \brief sensor for color detection
 */
class ColorSensor: public Sensor
{
	public:
		ColorSensor(MbsData *mbs_data, int robot_id);
		virtual ~ColorSensor();

		virtual void update();

		/// get the color currently seen
		int get_color_seen() { return color_seen; }

	private:
		Robot *rob; ///< robot related to this sensor
		
		int flag_init;  ///< 1 if initialization done, 0 otherwise
		int robot_id;   ///< ID of the robot
		int color_seen; ///< color currently seen

		double map_x; ///< map width [m]
		double map_y; ///< map height [m]

		double map_semi_x; ///< map semi width [m]
		double map_semi_y; ///< map semi height [m]

		// function prototype
		int compute_color(double x, double y);
};

#endif
