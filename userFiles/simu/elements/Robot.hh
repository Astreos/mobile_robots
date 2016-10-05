/*! 
 * \author Nicolas Van der Noot
 * \file Robot.hh
 * \brief Robot class
 */

#ifndef _ROBOT_HH_
#define _ROBOT_HH_

#include "PhysicElements.hh"
#include "RobotSensors.hh"
#include "ListElements.hh"
#include "Vector2D.hh"
#include "Vector3D.hh"
#include "mbs_data.h"
#include "Controller.hh"
#include "ctrl_io.h"
#include "Target.hh"
#include "MiniBotShape.hh"
#include "ScoreGestion.hh"
#include "TimeGestion.hh"
#include <vector>

/*! \brief Robot object
 */
class Robot: public PhysicElements
{
	public:
		Robot(GeometryShape *geometry, MbsData *mbs_data, Controller *ctrl, int robot_id, int T1_id, int T2_id, int R3_id, int RW_id, int LW_id, int TW_id);
		virtual ~Robot();

		double get_cur_t()  const { return cur_t;  }
		double get_prev_t() const { return prev_t; }

		double get_tower_gamma() const { return tower_gamma; }
		double get_tower_theta() const { return tower_theta; }
		double get_prev_tower_gamma() const { return prev_tower_gamma; }
		double get_prev_tower_theta() const { return prev_tower_theta; }

		Vector2D& get_tower_pos() { return tower_pos; }
		Vector2D& get_prev_tower_pos() { return prev_tower_pos; }

		/// return the body orientation [rad]
		double get_alpha() const { return geometry->get_alpha(); }

		double get_robot_id() const { return robot_id; }

		RobotSensors* get_sensors() const { return sensors; }

		Controller* get_ctrl() const { return ctrl; }

		int get_team() const { return team_id; }

		/// return 1 if active robot, 0 otherwise
		int is_ctrl() const { return ctrl->is_ctrl(); }

		virtual int is_robot(int &robot_index);

		void update_tower();

		int get_T1_id() const { return T1_id; }
		int get_T2_id() const { return T2_id; }
		int get_R3_id() const { return R3_id; }
		int get_RW_id() const { return RW_id; }
		int get_LW_id() const { return LW_id; }

		void update_sensors();
		void fill_inputs(CtrlIn *inputs);

		int get_nb_targets() const { return nb_targets; }

		Target* get_target(int i) { return targets_carried[i]; }

		void release_all_targets();
		void grab_target(Target *cur_target);
		void update_target_seen(Target *new_target);
		void update_all_targets_position();

		MiniBotShape* get_minibot_shape() { return static_cast<MiniBotShape*>(geometry); }

	private:
		int T1_id; ///< floating base T1 joint ID
		int T2_id; ///< floating base T2 joint ID
		int R3_id; ///< floating base R3 joint ID
		int RW_id; ///< right wheel joint ID
		int LW_id; ///< left wheel joint ID
		int TW_id; ///< tower ID

		int robot_id; ///< ID of the robot

		int team_id; ///< team index

		double tower_dist;  ///< distance from the robot center to the tower [m]
		double tower_angle; ///< orientation angle from the robot center to the tower [rad]
		
		double tower_gamma; ///< tower relative position [rad]
		double tower_theta; ///< tower absolute orientation [rad]
		double prev_tower_gamma; ///< 'tower_gamma' at the previous time step [rad]
		double prev_tower_theta; ///< 'tower_gamma' at the previous time step [rad]

		double cur_t; ///< current time [s]
		double prev_t; ///< previous time (related to 'prev_' variables) [s]

		int nb_targets; ///< number of targets carried by the robot
		int nb_max_targets; ///< maximla number of targets carried by the robot

		double t_target_seen; ///< time a new target was seen [s]
		double delta_t_grab; ///< time needed to grab a target [s]

		std::vector<Target*> targets_carried; ///< pointers to the targets carried
		Target *last_target_seen; ///< last target seen (NULL if no target currently seen)

		Vector2D tower_pos;      ///< tower position
		Vector2D prev_tower_pos; ///< 'tower_pos' at the previous time step [m]

		Controller *ctrl; ///< controller

		RobotSensors *sensors; ///< sensors of the robot

		MbsData *mbs_data; ///< Robotran structure

		// function prototypes
		Vector3D get_abs_vector(Vector3D const& vec_rel_robot);
		Vector3D get_rel_vector(Vector3D const& vec_abs_robot);

		ListElements* get_list_elem();
		ScoreGestion* get_score_gestion();
		TimeGestion* get_time_gestion();
};

#endif
