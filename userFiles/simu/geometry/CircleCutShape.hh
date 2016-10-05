/*! 
 * \author Nicolas Van der Noot
 * \file CircleCutShape.hh
 * \brief CircleCutShape class
 */
#ifndef _CIRCLE_CUT_HH_
#define _CIRCLE_CUT_HH_

#include "BasicShape.hh"
#include "RectangleShape.hh"
#include "CircleShape.hh"

/*! \brief CircleCutShape geometry
 */
class CircleCutShape: public BasicShape
{
	public:
		CircleCutShape(Vector2D const& init_center, double angle, double radius, double cut_length);

		virtual ~CircleCutShape();

		virtual void update_segments();

		double get_radius() { return radius; }

		virtual int contains_point(Vector2D const& point);
		virtual int intersects_segment(Segment const& seg);

		virtual void set_index_elem(int index_value);
		virtual void set_contact(int flag_value);
		virtual void set_uSwitch(int flag_value);

		virtual void reset_force_torque();

		int intersects_line(Segment const& seg);

		RectangleShape* get_rect_cut() { return &rect_cut; }
		CircleShape *get_circ_full() { return &circ_full; }

		void inc_circ_full_torque(double x_cont, double y_cont);

		void set_force_torque_circle();
		void set_force_torque_rectangle();
		void set_all_force_torque();

		virtual void print_identity() { std::cout << "CircleCutShape" << std::endl; }

	protected:
		double radius; ///< radius of the circle [m]

		double cut_length; ///< cutted length along a radius [m]
		double cut_center; ///< length between the center of the circle and the cutted border [m]

		double gamma; ///< special angle describing the cutted edge [rad]

		double dist_rect; ///< distance of the rectangle center

		RectangleShape rect_cut; ///< small rectangle representing the cutted segment
		CircleShape circ_full; ///< circle representing the non-cutted circle-cut

		Vector2D rect_center; ///< center of the rectangle
		Pose rect_pose; ///< pose of the rectangle
};

#endif
