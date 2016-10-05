/*! 
 * \author Nicolas Van der Noot
 * \file TowerSensor.hh
 * \brief TowerSensor class
 */

#ifndef _TOWER_SENSOR_HH_
#define _TOWER_SENSOR_HH_

#include "Sensor.hh"

#define NB_BEACON_POS 3

/// tower inputs
typedef struct TowerInStruct
{
	double last_rising[NB_STORE_EDGE];  ///< rotating list with the last rising edges detected [rad]
	double last_falling[NB_STORE_EDGE]; ///< rotating list with the last falling edges detected [rad]

	double last_rising_noise[NB_STORE_EDGE];  ///< 'last_rising' with noise
	double last_falling_noise[NB_STORE_EDGE]; ///< 'last_falling' with noise
	
	int rising_index;  ///< index in 'last_rising' of the last element added
	int falling_index; ///< index in 'last_falling' of the last element added

	int count_rising;  ///< number of rising edges detected since last reset
	int count_falling; ///< number of falling edges detected since last reset

	int nb_rising;  ///< number of rising edges detected during the last laser revolution
	int nb_falling; ///< number of falling edges detected during the last laser revolution
	
} TowerInStruct;

/*! \brief sensor for tower
 */
class TowerSensor: public Sensor
{
	public:
		TowerSensor(MbsData *mbs_data, int robot_id);
		virtual ~TowerSensor();

		virtual void update();

		TowerInStruct& get_robot_input() { return robot_input; }
		TowerInStruct& get_fixed_input() { return fixed_input; }

		double get_tower_gamma() const { return tower_gamma; }

	private:
		Vector2D beacon_fixed[NB_BEACON_POS]; ///< position of the fixed beacons [m]

		double radius_fixed; ///< radius of the fixed beacons
		double radius_robot; ///< radius of the robots

		int robot_id; ///< robot ID
		int team_id;  ///< team ID

		double tower_gamma; ///< tower relative position [rad]
		double tower_theta; ///< tower absolute orientation [rad]
		double prev_tower_gamma; ///< 'tower_gamma' at the previous time step [rad]
		double prev_tower_theta; ///< 'tower_gamma' at the previous time step [rad]

		double tower_noise; ///< noise on the tower beacon detection [rad]

		Vector2D tower_pos;      ///< tower position
		Vector2D prev_tower_pos; ///< 'tower_pos' at the previous time step [m]

		int last_fixed_detect; ///< last fixed beacon detection: 1 if detected, 0 otherwise
		int last_robot_detect; ///< last robot beacon detection: 1 if detected, 0 otherwise

		int nb_interp; ///< number of points for the interpolation

		double inc_t; ///< increment of time [s]
		double inc_gamma; ///< increment for 'tower_gamma'
		double inc_theta; ///< increment for 'tower_theta'
		double inc_ratio; ///< increment for the ratio related to 'tower_pos'

		double cur_t; ///< current time [s]
		double prev_t; ///< previous time (related to 'prev_' variables) [s]

		int last_sign; ///< last sign to check for optical barrier

		Robot *rob; ///< robot related to this tower

		TowerInStruct robot_input; ///< robot beacon detection inputs
		TowerInStruct fixed_input; ///< fixed beacon detection inputs

		// function prototypes
		void reset_input(TowerInStruct &tower_input);
		int is_tower_detected(Vector2D cur_tower_pos, Vector2D cur_beacon_pos, double cur_theta_tower, double beacon_radius);
		void update_incr();
		void update_fixed();
		void udpate_robot();
		void check_laser_reset();
};

#endif
