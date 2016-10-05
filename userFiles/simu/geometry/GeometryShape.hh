/*! 
 * \author Nicolas Van der Noot
 * \file GeometryShape.hh
 * \brief GeometryShape class
 */

#ifndef _GEOMETRY_SHAPE_HH_
#define _GEOMETRY_SHAPE_HH_


#include "Vector2D.hh"
#include "Pose.hh"
#include "Segment.hh"

#include <iostream>

/// type of geometry
enum{WALL_TYPE, CIRCLE_TYPE, RECTANGLE_TYPE, CIRCLE_CUT_TYPE, UNION_TYPE};

/// type of element (physical)
enum{FIXED_TYPE, MOVING_TYPE, ROBOT_TYPE};

/*! \brief Mother class of the different geometries
 */
class GeometryShape
{
	public:
		GeometryShape();
		virtual ~GeometryShape();

		virtual int is_union() = 0;

		virtual void update_segments() = 0;

		virtual void reset_force_torque()  = 0;
		virtual void update_force_torque() = 0;

		virtual void update_from_pos(Vector2D const& vec   , double angle) = 0;
		virtual void update_from_vel(Vector2D const& vec_p , double angle_p, double delta_t)  = 0;
		virtual void update_from_acc(Vector2D const& vec_pp, double angle_pp, double delta_t) = 0;

		virtual void update_new_pose(Pose const& new_pose) = 0;
		virtual void update_new_pose(double x, double y, double x_p, double y_p, double orient, double orient_p) = 0;
		virtual void update_rigid_pose(Pose & old_ref_pos, Pose & new_ref_pose) = 0;

		virtual void set_index_elem(int index_value) = 0;
		virtual void set_contact(int flag_value)     = 0;
		virtual void set_uSwitch(int flag_value)     = 0;

		virtual int contains_point(Vector2D const& point)  = 0;
		virtual int intersects_segment(Segment const& seg) = 0;

		virtual void print_identity() = 0;

		void print_pos();

		/// modify the geometry pose
		void set_new_pose(Pose const& new_pose) { pose.set_new_pose(new_pose); }
		
		/// modify the external force applied to the geometry body
		void set_force(Vector2D const& value2D) { force.set_vector(value2D); }

		/// modify the external torque applied to the geometry body
		void set_torque(double value) { torque = value; }

		/// increment the external force applied to the geometry body
		void inc_force(Vector2D const& value2D)  { force.inc_vector(value2D); }

		/// increment the external torque applied to the geometry body
		void inc_torque(double value) { torque += value; }

		/// return the external force applied to the geometry body
		Vector2D get_force() { return force;  }

		/// return the external torque applied to the geometry body
		double get_torque()  { return torque; }

		/// return the body pose
		Pose get_pose()      { return pose;   }

		/// return the body position [m]
		Vector2D get_pos()   { return pose.get_pos(); }

		/// return the body x coordinate [m]
		double get_x() { return pose.get_x(); }

		/// return the body y coordinate [m]
		double get_y() { return pose.get_y(); }

		/// return the body x coordinate derivative [m/s]
		double get_x_p() { return pose.get_x_p(); }

		/// return the body y coordinate derivative [m/s]
		double get_y_p() { return pose.get_y_p(); }

		/// return the body orientation [rad]
		double get_alpha() { return pose.get_alpha(); }

		/// return the body orientation cosine [rad]
		double get_c_alpha() { return pose.get_c_alpha(); }

		/// return the body orientation sine [rad]
		double get_s_alpha() { return pose.get_s_alpha(); }

		/// return the body orientation derivative [rad/s]
		double get_alpha_p() { return pose.get_alpha_p(); }

		/// return the body orientation class
		Orientation get_orient()   const { return pose.get_orient();   }

		/// return the body orientation derivative class
		Orientation get_orient_p() const { return pose.get_orient_p(); }

		/// return the index about the type of geometry
		int get_index_type() { return index_type; }

		/// return the index about the type of physical element
		int get_index_elem() { return index_elem; }

		/// return the wrapping radius [m]
		double get_wrapping_radius() { return wrapping_radius; }

		/// 1 if body producing contacts, 0 otherwise
		int is_contact() { return flag_contact; }

		/// 1 if fixed possible to detect with micro-switches, 0 otherwise
		int is_uSwitch() { return flag_uSwitch; }

		/// get the absolute speed [m/s]
		double get_abs_speed() { return pose.get_abs_speed(); }

		/// return 1 if fixed elem
		int is_fixed() const { return (index_elem == FIXED_TYPE); }

		/// return 1 if moving elem
		int is_moving() const { return (index_elem == MOVING_TYPE); }

		/// return 1 if robot elem
		int is_robot() const { return (index_elem == ROBOT_TYPE); }
		
	protected:
		Pose pose; ///< pose (position, orientation and derivatives) of this geometry [m], [rad], [m/s], [rad/s]

		Vector2D force; ///< external force applied to the body [N]
		double torque; ///< external torque applied to the body [Nm]

		double wrapping_radius; ///< wrapping radius of the body [m]

		int index_type; ///< index of the type of geometry
		int index_elem; ///< index of the type of physical element

		int flag_contact; ///< 1 if body producing contacts, 0 otherwise
		int flag_uSwitch; ///< 1 if fixed possible to detect with micro-switches, 0 otherwise
};

/*! \brief compute the distance between two geometry shapes
 * 
 * \param[in] geom_1 first shape
 * \param[in] geom_2 second shape
 * \return distance between their centers
 */
inline double get_dist_geom(GeometryShape* geom_1, GeometryShape* geom_2)
{
	double delta_x = geom_2->get_x() - geom_1->get_x();
	double delta_y = geom_2->get_y() - geom_1->get_y();

	return sqrt(delta_x*delta_x + delta_y*delta_y);
}

#endif
