/*! 
 * \author Nicolas Van der Noot
 * \file Target.hh
 * \brief Target class
 */

#ifndef _TARGET_HH_
#define _TARGET_HH_

#include "NoContactElements.hh"
#include "Vector3D.hh"

// target states
enum {TARGET_GROUND, TARGET_FIXED, TARGET_ROBOT};

class Target: public NoContactElements
{
	public:
		Target(double T1, double T2, double T3, int T1_id, int T2_id, int T3_id, int point_value);
		virtual ~Target();

		int get_points() const { return point_value; }

		double get_x() const { return pos.get_x(); }
		double get_y() const { return pos.get_y(); }
		double get_z() const { return pos.get_z(); }

		void set_x(double value) { pos.set_x(value); }
		void set_y(double value) { pos.set_y(value); }
		void set_z(double value) { pos.set_z(value); }

		/// return 1 if on the ground, 0 otherwise
		int on_ground() const { return (state == TARGET_GROUND) || (state == TARGET_FIXED); }

		int get_state() const { return state; }

		void set_ground();
		void set_fixed();

		/// grab the target
		void grab() { state = TARGET_ROBOT; }

		/// fill the joint IDs
		void fill_T1_T2_T3(int &T1, int &T2, int &T3)
		{
			T1 = T1_id;
			T2 = T2_id;
			T3 = T3_id;
		}

		/// get the 3D position
		void get_pos(double &x, double &y, double &z)
		{
			x = get_x();
			y = get_y();
			z = get_z();
		}

		/// set the 3D position
		void set_pos(double x, double y, double z)
		{
			set_x(x);
			set_y(y);
			set_z(z);
		}

		int point_inside(double x, double y);
		int full_inside_rectangle(double x_rect, double y_rect, double angle_rect, double semi_width_rect, double semi_height_rect);

	private:
		Vector3D pos; ///< 3D position [m]

		int T1_id; ///< ID of the T1 joint in the anim .mbs file
		int T2_id; ///< ID of the T2 joint in the anim .mbs file
		int T3_id; ///< ID of the R3 joint in the anim .mbs file

		int point_value; ///< value of the targets (number of points)

		int state; ///< state of the target

		double radius; ///< radius of the target [m]
};

#endif
