/*! 
 * \author Nicolas Van der Noot
 * \file BasicShape.hh
 * \brief BasicShape class
 */

#ifndef __BASIC_SHAPE__HH__
#define __BASIC_SHAPE__HH__


#include "GeometryShape.hh"

/*! \brief Basic geometry shape
 */
class BasicShape: public GeometryShape
{
	public:
		BasicShape();
		virtual ~BasicShape();

		virtual int is_union() { return 0; }

		virtual void update_from_pos(Vector2D const& vec   , double angle);
		virtual void update_from_vel(Vector2D const& vec_p , double angle_p, double delta_t);
		virtual void update_from_acc(Vector2D const& vec_pp, double angle_pp, double delta_t);		

		virtual void update_new_pose(Pose const& new_pose);
		virtual void update_new_pose(double x, double y, double x_p, double y_p, double orient, double orient_p);

		virtual void update_rigid_pose(Pose & old_ref_pos, Pose & new_ref_pose);

		virtual void reset_force_torque();
		virtual void update_force_torque();

		/// modify the 'index_elem' value
		virtual void set_index_elem(int index_value) { index_elem = index_value; }

		/// modify the 'flag_contact' value
		virtual void set_contact(int flag_value) { flag_contact = flag_value; }

		/// modify the 'flag_uSwitch' value
		virtual void set_uSwitch(int flag_value) { flag_uSwitch = flag_value; }

		virtual void print_identity() { std::cout << "BasicShape" << std::endl; }
};

#endif
