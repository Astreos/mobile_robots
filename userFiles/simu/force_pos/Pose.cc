
#include "Pose.hh"
#include "simu_functions.hh"

/// constructor
Pose::Pose(): pos(0.0, 0.0), pos_p(0.0, 0.0), orient(0.0), orient_p(0.0)
{

}

/// constructor
Pose::Pose(Vector2D const& init_pos): pos(init_pos), pos_p(0.0, 0.0), orient(0.0), orient_p(0.0)
{

}

/// constructor
Pose::Pose(Vector2D const& init_pos, double alpha): pos(init_pos), pos_p(0.0, 0.0), orient(alpha), orient_p(0.0)
{

}

/// constructor
Pose::Pose(double x_pos, double y_pos, double alpha): pos(x_pos, y_pos), pos_p(0.0, 0.0), orient(alpha), orient_p(0.0)
{

}

/// constructor
Pose::Pose(double x_pos, double y_pos, double x_p_pos, double y_p_pos, double alpha, double alpha_p): pos(x_pos, y_pos), pos_p(x_p_pos, y_p_pos), orient(alpha), orient_p(alpha_p)
{

}

/// destructor
Pose::~Pose()
{
	
}

/*! \brief Set a new orientation
 * 
 * \param[in] x x position
 * \param[in] y y position
 * \param[in] x_p x velocity
 * \param[in] y_p y velocity
 * \param[in] alpha angle describing the orientation
 * \param[in] alpha_p angle velocity describing the orientation
 */
void Pose::set_values(double x, double y, double x_p, double y_p, double alpha, double alpha_p)
{
	pos.set_vector(x, y);
	pos_p.set_vector(x_p, y_p);

	orient.set_orientation(alpha);
	orient_p.set_orientation(alpha_p);
}

/*! \brief update the pose given the old and new pose of a reference
 * 
 * \param[in] old_ref_pose old reference pose
 * \param[in] new_ref_pose new reference pose
 */
void Pose::update_rigid_pose(Pose & old_ref_pose, Pose & new_ref_pose)
{
	double alpha_ref_diff, new_alpha, new_alpha_p;

	Vector2D old_diff_vec, new_diff_vec;

	// computation
	alpha_ref_diff = limit_angle(new_ref_pose.get_alpha() - old_ref_pose.get_alpha());

	old_diff_vec = get_pos() - old_ref_pose.get_pos();

	new_diff_vec = get_rotated_vector(old_diff_vec, alpha_ref_diff);

	// new pose (without velocity)
	pos = new_ref_pose.get_pos() + new_diff_vec;

	new_alpha = limit_angle(orient.get_alpha() + alpha_ref_diff);

	orient.set_orientation(new_alpha);

	// new pose (velocity)
	new_alpha_p = new_ref_pose.get_alpha_p();

	orient_p.set_orientation(new_alpha_p);

	pos_p = new_ref_pose.get_pos_p() + get_omega_vector(new_diff_vec, new_alpha_p);
}

/*! \brief Set a new pose
 * 
 * \param[in] new_pose new pose to set
 */
void Pose::set_new_pose(Pose const& new_pose)
{
	pos   = new_pose.get_pos();
	pos_p = new_pose.get_pos_p();

	orient   = new_pose.get_orient();
	orient_p = new_pose.get_orient_p();
}

/*! \brief Update the pose from a given position
 * 
 * \param[in] vec new position coordinates (x, y) [m]
 * \param[in] angle new position orientation [rad]
 */
void Pose::update_from_pos(Vector2D const& vec, double angle)
{
	pos = vec;
	orient.set_orientation(angle);
}

/*! \brief Update the pose from a given velocity
 * 
 * \param[in] vec_p new velocity (x, y) [m/s]
 * \param[in] angle_p new orientation velocity [rad/s]
 * \param[in] delta_t time step [s]
 */
void Pose::update_from_vel(Vector2D const& vec_p, double angle_p, double delta_t)
{
	Vector2D inc_vec = mult_vector(vec_p, delta_t);

	pos_p = vec_p;
	orient_p.set_orientation(angle_p);

	pos.inc_vector(inc_vec);
	orient.inc_orientation(angle_p*delta_t);
}

/*! \brief Update the pose from a given acceleration
 * 
 * \param[in] vec_pp new acceleration (x, y) [m/s^2]
 * \param[in] angle_pp new orientation acceleration [rad/s^2]
 * \param[in] delta_t time step [s]
 */
void Pose::update_from_acc(Vector2D const& vec_pp, double angle_pp, double delta_t)
{
	double mult_pos_fact = 0.5*delta_t*delta_t;
	
	Vector2D inc_vel = mult_vector(vec_pp, delta_t);
	Vector2D inc_pos = mult_vector(vec_pp, mult_pos_fact) + mult_vector(pos_p, delta_t);

	pos_p.inc_vector(inc_vel);
	orient_p.inc_orientation(angle_pp*delta_t);

	pos.inc_vector(inc_pos);
	orient.inc_orientation(angle_pp*mult_pos_fact + orient_p.get_alpha()*delta_t);
}
