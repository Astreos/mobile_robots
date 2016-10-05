/*! 
 * \author Nicolas Van der Noot
 * \file UnionShape.hh
 * \brief UnionShape class
 */

#ifndef __UNION_SHAPE__HH__
#define __UNION_SHAPE__HH__


#include "GeometryShape.hh"
#include <vector>

/*! \brief Unions of different GeometryShape bodies
 */
class UnionShape: public GeometryShape
{
	public:
		UnionShape(double wrapping_radius);

		virtual ~UnionShape();

		void add_geometry(GeometryShape *new_geom);

		void update_list_pose(Pose & old_ref_pose, Pose & new_ref_pose);

		virtual int is_union() { return 1; }

		virtual void update_from_pos(Vector2D const& vec   , double angle);
		virtual void update_from_vel(Vector2D const& vec_p , double angle_p, double delta_t);
		virtual void update_from_acc(Vector2D const& vec_pp, double angle_pp, double delta_t);

		virtual void update_new_pose(Pose const& new_pose);
		virtual void update_new_pose(double x, double y, double x_p, double y_p, double orient, double orient_p);

		virtual void update_rigid_pose(Pose & old_ref_pose, Pose & new_ref_pose);

		virtual void update_force_torque();

		virtual int contains_point(Vector2D const& point);
		virtual int intersects_segment(Segment const& seg);

		GeometryShape* get_item(int index) { return list[index]; }
		
		GeometryShape* get_last()
		{
			if (list.size())
			{
				return list[list.size()-1];
			}
			else
			{
				return NULL;
			}
		}

		int nb_items() { return list.size(); }

		virtual void reset_force_torque();

		virtual void update_segments();

		virtual void set_index_elem(int index_value);
		virtual void set_contact(int flag_value);
		virtual void set_uSwitch(int flag_value);

		virtual void print_identity() { std::cout << "UnionShape" << std::endl; }

	protected:
		std::vector<GeometryShape*> list; ///< list with all the GeometryShape bodies
};


#endif
