
#include "BasicShape.hh"

/// constructor
BasicShape::BasicShape()
{

}

/// destructor
BasicShape::~BasicShape()
{

}

/*! \brief Update the pose from a given position
 * 
 * \param[in] vec new position coordinates (x, y) [m]
 * \param[in] angle new position orientation [rad]
 */
void BasicShape::update_from_pos(Vector2D const& vec, double angle)
{
	pose.update_from_pos(vec, angle);
}

/*! \brief Update the pose from a given velocity
 * 
 * \param[in] vec_p new velocity (x, y) [m/s]
 * \param[in] angle_p new orientation velocity [rad/s]
 * \param[in] delta_t time step [s]
 */
void BasicShape::update_from_vel(Vector2D const& vec_p, double angle_p, double delta_t)
{
	pose.update_from_vel(vec_p, angle_p, delta_t);
}

/*! \brief Update the pose from a given acceleration
 * 
 * \param[in] vec_pp new acceleration (x, y) [m/s^2]
 * \param[in] angle_pp new orientation acceleration [rad/s^2]
 * \param[in] delta_t time step [s]
 */
void BasicShape::update_from_acc(Vector2D const& vec_pp, double angle_pp, double delta_t)
{
	pose.update_from_acc(vec_pp, angle_pp, delta_t);
}

/*! \brief Set a new pose
 * 
 * \param[in] new_pose new pose to set
 */
void BasicShape::update_new_pose(Pose const& new_pose)
{
	pose.set_new_pose(new_pose);
}

/*! \brief Set a new orientation
 * 
 * \param[in] x x position
 * \param[in] y y position
 * \param[in] x_p x velocity
 * \param[in] y_p y velocity
 * \param[in] orient angle describing the orientation
 * \param[in] orient_p angle velocity describing the orientation
 */
void BasicShape::update_new_pose(double x, double y, double x_p, double y_p, double orient, double orient_p)
{
	pose.set_values(x, y, x_p, y_p, orient, orient_p);
}

/*! \brief update the pose given the old and new pose of a reference
 * 
 * \param[in] old_ref_pos old reference pose
 * \param[in] new_ref_pos new reference pose
 */
void BasicShape::update_rigid_pose(Pose & old_ref_pos, Pose & new_ref_pose)
{
	pose.update_rigid_pose(old_ref_pos, new_ref_pose);
}

/*! \brief update the force and torques
 * 
 * nothing happens for basic shapes
 */
void BasicShape::update_force_torque()
{
	
}

/*! \brief set the forces and torques to 0
 */
void BasicShape::reset_force_torque()
{
	force.reset();
	torque = 0.0;
}
