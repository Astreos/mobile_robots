/*! 
 * \author Nicolas Van der Noot
 * \file Pose.hh
 * \brief Pose class
 */

#ifndef _POSE_HH_
#define _POSE_HH_

#include "Vector2D.hh"
#include "Orientation.hh"

/*! \brief Pose description
 * 
 * Pose: position (x, y, orientation) and velocity of a point
 */
class Pose
{
	public:
		Pose();
		Pose(Vector2D const& init_pos);
		Pose(Vector2D const& init_pos, double alpha);
		Pose(double x_pos, double y_pos, double alpha);
		Pose(double x_pos, double y_pos, double x_p_pos, double y_p_pos, double alpha, double alpha_p);

		~Pose();

		void set_values(double x, double y, double x_p, double y_p, double orient, double orient_p);

		void set_new_pose(Pose const& new_pose);

		void update_from_pos(Vector2D const& vec   , double angle);
		void update_from_vel(Vector2D const& vec_p , double angle_p, double delta_t);
		void update_from_acc(Vector2D const& vec_pp, double angle_pp, double delta_t);

		void update_rigid_pose(Pose & old_ref_pose, Pose & new_ref_pose);

		/// get the position [m]
		Vector2D get_pos()   const { return pos;   }

		/// get the velocity [m/s]
		Vector2D get_pos_p() const { return pos_p; }

		/// get the Orientation
		Orientation get_orient()   const { return orient;   }

		/// get the Orientation derivative
		Orientation get_orient_p() const { return orient_p; }

		/// get the x coordinate
		double get_x() { return pos.get_vec(0); }

		/// get the y coordinate
		double get_y() { return pos.get_vec(1); }

		/// get the x velocity
		double get_x_p() { return pos_p.get_vec(0); }

		/// get the y velocity
		double get_y_p() { return pos_p.get_vec(1); }

		/// get the orientation [rad]
		double get_alpha() { return orient.get_alpha(); }

		/// get the orientation cosine [rad]
		double get_c_alpha() { return orient.get_c_alpha(); }

		/// get the orientation sine [rad]
		double get_s_alpha() { return orient.get_s_alpha(); }

		/// get the orientation velocity [rad/s]
		double get_alpha_p() { return orient_p.get_alpha(); }

		/// set a new position [rad]
		void set_pos(Vector2D const& new_pos) { pos = new_pos; }

		/// set a new velocity [rad/s]
		void set_pos_p(Vector2D const& new_pos_p) { pos_p = new_pos_p; }

		/// get the absolute speed [m/s]
		double get_abs_speed() { return pos_p.get_dist(); }

	private:
		Vector2D pos;   ///< position (x, y) [m]
		Vector2D pos_p; ///< velocity (x, y) [m/s]

		Orientation orient;   ///< orientation
		Orientation orient_p; ///< orientation derivative
};

/// 
/*! \brief get the distance between two poses
 * 
 * \param[in] pose_1 pose of the first body
 * \return distance between the two bodies [m]
 */
inline double get_dist_pose(Pose const& pose_1, Pose const& pose_2)
{
	return get_dist_points2D(pose_1.get_pos(), pose_2.get_pos());
}

/// 
/*! \brief get the velocity of a point given a reference pose
 * 
 * \param[in] init_pose pose of a reference attached rigidly attached to the requested point
 * \param[in] request_pos positoion of the requested point
 * \return velocity of the requested point [m/s]
 */
inline Vector2D get_rigid_pos_p(Pose & init_pose, Vector2D const& request_pos)
{
	Vector2D result, vec_diff;

	vec_diff = request_pos - init_pose.get_pos();

	result = init_pose.get_pos_p() + cross_product(init_pose.get_alpha_p(), vec_diff);

	return result;
}

#endif
