
#include "UnionShape.hh"
#include "simu_functions.hh"

/// constructor
UnionShape::UnionShape(double wrapping_radius)
{
	this->wrapping_radius = wrapping_radius;

	index_type = UNION_TYPE;
}

/// destructor
UnionShape::~UnionShape()
{
	for (unsigned int i=0; i<list.size(); i++)
	{
		delete list[i];	
	}
}

/*! \brief Add a geometry to the union of geometry shapes
 * 
 * \param[in] new_geom new geometry to add
 */
void UnionShape::add_geometry(GeometryShape *new_geom) 
{
	// same contact for child
	if (flag_contact)
	{
		new_geom->set_contact(1);
	}
	else
	{
		new_geom->set_contact(0);
	}

	// same micro-switch for child
	if (flag_uSwitch)
	{
		new_geom->set_uSwitch(1);
	}
	else
	{
		new_geom->set_uSwitch(0);
	}

	// same index_elem for child
	new_geom->set_index_elem(index_elem);

	if (!list.size())
	{
		pose.set_new_pose(new_geom->get_pose());
	}

	list.push_back(new_geom);
}

/*! \brief Update the segments state of all bodies (recursively)
 */
void UnionShape::update_segments()
{
	if ( is_fixed() || ( (!is_contact()) && (!is_uSwitch()) ) )
	{
		return;
	}

	for (unsigned int i=0; i<list.size(); i++)
	{
		list[i]->update_segments();
	}
}

/*! \brief update the pose of all bodies recursively
 * 
 * \param[in] old_ref_pose old reference pose
 * \param[in] new_ref_pose new reference pose
 */
void UnionShape::update_list_pose(Pose & old_ref_pose, Pose & new_ref_pose)
{
	if (!list.size())
	{
		return;
	}

	// first body (reference)
	list[0]->update_new_pose(new_ref_pose);

	// following bodies
	for (unsigned int i=1; i<list.size(); i++)
	{
		list[i]->update_rigid_pose(old_ref_pose, new_ref_pose);
	}
}

/*! \brief update all the poses from position
 * 
 * \param[in] vec (x, y) positions [m]
 * \param[in] angle orientation [rad]
 */
void UnionShape::update_from_pos(Vector2D const& vec, double angle)
{
	Pose old_ref_pose, new_ref_pose;

	old_ref_pose = pose;

	pose.update_from_pos(vec, angle);

	new_ref_pose = pose;

	update_list_pose(old_ref_pose, new_ref_pose);
}

/*! \brief update all the poses from velocity
 * 
 * \param[in] vec_p (x, y) velocities [m/s]
 * \param[in] angle_p orientation derivative [rad/s]
 * \param[in] delta_t time step [s]
 */
void UnionShape::update_from_vel(Vector2D const& vec_p, double angle_p, double delta_t)
{
	Pose old_ref_pose, new_ref_pose;

	old_ref_pose = pose;

	pose.update_from_vel(vec_p, angle_p, delta_t);

	new_ref_pose = pose;

	update_list_pose(old_ref_pose, new_ref_pose);
}

/*! \brief update all the poses from acceleration
 * 
 * \param[in] vec_pp (x, y) accelerations [m/s^2]
 * \param[in] angle_pp orientation second derivative [rad/s^2]
 * \param[in] delta_t time step [s]
 */
void UnionShape::update_from_acc(Vector2D const& vec_pp, double angle_pp, double delta_t)
{
	Pose old_ref_pose, new_ref_pose;

	old_ref_pose = pose;

	pose.update_from_acc(vec_pp, angle_pp, delta_t);

	new_ref_pose = pose;

	update_list_pose(old_ref_pose, new_ref_pose);
}

/*! \brief update all the poses from new pose
 * 
 * \param[in] new_pose new pose
 */
void UnionShape::update_new_pose(Pose const& new_pose)
{
	Pose old_ref_pose, new_ref_pose;

	old_ref_pose = pose;

	pose.set_new_pose(new_pose);

	new_ref_pose = pose;

	update_list_pose(old_ref_pose, new_ref_pose);
}

/*! \brief update all the poses from new pose
 * 
 * \param[in] x x coordinate [m]
 * \param[in] y y coordinate [m]
 * \param[in] x_p x coordinate derivative [m/s]
 * \param[in] y_p y coordinate derivative [m/s]
 * \param[in] orient orientation [rad]
 * \param[in] orient orientation derivative [rad/s]
 */
void UnionShape::update_new_pose(double x, double y, double x_p, double y_p, double orient, double orient_p)
{
	Pose old_ref_pose, new_ref_pose;

	old_ref_pose = pose;

	pose.set_values(x, y, x_p, y_p, orient, orient_p);

	new_ref_pose = pose;

	update_list_pose(old_ref_pose, new_ref_pose);
}

/*! \brief update the pose relative to a reference attached to the same rigid body
 * 
 * \param[in] old_ref_pose old reference pose
 * \param[in] new_ref_pose new reference pose
 */
void UnionShape::update_rigid_pose(Pose & old_ref_pose, Pose & new_ref_pose)
{
	Pose old_ref_pose_2, new_ref_pose_2;

	old_ref_pose_2 = pose;

	pose.update_rigid_pose(old_ref_pose, new_ref_pose);

	new_ref_pose_2 = pose;

	update_list_pose(old_ref_pose_2, new_ref_pose_2);
}

/*! \brief update the forces and torques, summing them recursively
 */
void UnionShape::update_force_torque()
{
	force.reset();
	torque = 0.0;

	for (unsigned int i=0; i<list.size(); i++)
	{
		list[i]->update_force_torque();

		force.inc_vector(list[i]->get_force());

		torque += list[i]->get_torque();
		torque += cross_product(list[i]->get_pos() - get_pos(), list[i]->get_force());
	}
}

/*! \brief set the forces and torques to 0
 */
void UnionShape::reset_force_torque()
{
	force.reset();
	torque = 0.0;

	for (unsigned int i=0; i<list.size(); i++)
	{
		list[i]->reset_force_torque();
	}
}

/// modify the 'index_elem' value
void UnionShape::set_index_elem(int index_value)
{
	index_elem = index_value;

	for (unsigned int i=0; i<list.size(); i++)
	{
		list[i]->set_index_elem(index_value);
	}
}

/// modify the 'flag_contact' value
void UnionShape::set_contact(int flag_value)
{
	flag_contact = flag_value;

	for (unsigned int i=0; i<list.size(); i++)
	{
		list[i]->set_contact(flag_value);
	}
}

/// modify the 'flag_uSwitch' value
void UnionShape::set_uSwitch(int flag_value)
{
	flag_uSwitch = flag_value;

	for (unsigned int i=0; i<list.size(); i++)
	{
		list[i]->set_uSwitch(flag_value);
	}
}

/*! \brief check if it contains a point
 * 
 * \param[in] point point to check if it belongs to this UnionShape geometry
 * \return 1 if the point belongs to the UnionShape geometry, 0 otherwise
 */
int UnionShape::contains_point(Vector2D const& point)
{
	Vector2D diff_vec = point - get_pos();

	if (diff_vec.get_dist() > wrapping_radius)
	{
		return 0;
	}
	
	for (unsigned int i=0; i<list.size(); i++)
	{
		if (list[i]->contains_point(point))
		{
			return 1;
		}
	}
	
	return 0;
}

/*! \brief check if it intersects a segment
 * 
 * \param[in] seg segment to check if it intersects this geometry
 * \return 1 if the point belongs to the geometry, 0 otherwise
 */
int UnionShape::intersects_segment(Segment const& seg)
{
	Vector2D diff_vec = seg.get_center() - get_pos();

	if (diff_vec.get_dist() >= wrapping_radius + seg.get_wrapping_radius())
	{
		return 0;
	}

	for (unsigned int i=0; i<list.size(); i++)
	{
		if (list[i]->intersects_segment(seg))
		{
			return 1;
		}
	}
	
	return 0;
}
