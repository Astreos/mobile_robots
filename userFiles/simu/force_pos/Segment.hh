/*! 
 * \author Nicolas Van der Noot
 * \file Segment.hh
 * \brief Segment class
 */

#ifndef _SEGEMENT_HH_
#define _SEGEMENT_HH_

#include "Vector2D.hh"
#include "Orientation.hh"

/*! \brief Segment description (position and velocity)
 */
class Segment
{
	public:
		Segment();
		Segment(double x1, double y1, double x2, double y2);

		~Segment();

		void set_vector(Vector2D const& XA, Vector2D const& XB, Vector2D const& XA_P, Vector2D const& XB_P);
		void set_vector(double x1, double y1, double x2, double y2, double x1_p, double y1_p, double x2_p, double y2_p);
		void set_slope_offset();

		void set_min_max();

		int get_flag_vert()   const { return flag_vert; }
		double get_slope()    const { return slope;     }
		double get_offset()   const { return offset;    }
		double get_slope_p()  const { return slope_p;   }
		double get_offset_p() const { return offset_p;  }

		double get_min_x() const { return min_x; }
		double get_max_x() const { return max_x; }
		double get_min_y() const { return min_y; }
		double get_max_y() const { return max_y; }

		double get_x1() const { return X1.get_vec(0); }
		double get_y1() const { return X1.get_vec(1); }
		double get_x2() const { return X2.get_vec(0); }
		double get_y2() const { return X2.get_vec(1); }

		Vector2D get_center() const { return center; }
		double get_wrapping_radius() const { return wrapping_radius; }

		void compute_center();
		void compute_wrapping_center();

	private:
		Vector2D X1; ///< first point position
		Vector2D X2; ///< second point position

		Vector2D X1_P; ///< first point velocity
		Vector2D X2_P; ///< second point velocity

		int flag_vert; ///< 1 if vertical segment, 0 otherwise

		double slope; ///< slope of the segment
		double offset; ///< offset of the segment
		double slope_p; ///< derivative of the slope
		double offset_p; ///< derivative of the offset

		double min_x; ///< minimal x coordinate
		double max_x; ///< maximal x coordinate
		double min_y; ///< minimal y coordinate
		double max_y; ///< maximal y coordinate

		Vector2D center; ///< center of the segment
		double wrapping_radius; ///< wrapping radius of the body
};

int is_intersection(Segment const& a, Segment const& b);
int is_intersection(Segment const& a, Segment *seg_list, int list_size);
int is_intersection(Segment const& a, double radius, Vector2D const& center);
int is_intersection(Segment *seg_list, int list_size, double radius, Vector2D const& center);

int is_line_intersection(Segment const& a, double radius, Vector2D const& center);

int get_two_seg_list_two_points(Segment *seg_list_1, int list_size_1, Segment *seg_list_2, int list_size_2, Vector2D & X1, Vector2D & X2, Vector2D & X1_P, Vector2D & X2_P);
int get_intersection(Segment const& a, Segment const& b, double & x, double & y, double & x_p, double & y_p);
int get_line_intersection(Segment const& a, Segment const& b, double & x, double & y, double & x_p, double & y_p);

int get_circle_seg_list_two_points(Segment *seg_list, int list_size, double radius, Vector2D const& center, Vector2D const& center_p, Vector2D & X1, Vector2D & X2, Vector2D & X1_P, Vector2D & X2_P);
int get_circle_intersection(Segment const& a, double radius, Vector2D const& center, Vector2D const& center_p, double & x1, double & y1, double & x2, double & y2, double & x1_p, double & y1_p, double & x2_p, double & y2_p);
int get_circle_line_intersection(Segment const& a, double radius, Vector2D const& center, Vector2D const& center_p, double & x1, double & y1, double & x2, double & y2, double & x1_p, double & y1_p, double & x2_p, double & y2_p);

int get_circle_intersection(Segment const& a, double radius, Vector2D const& center, double & x1, double & y1, double & x2, double & y2);
int get_circle_line_intersection(Segment const& a, double radius, Vector2D const& center, double & x1, double & y1, double & x2, double & y2);

int get_wall_seg_list_two_points(Segment *seg_list, int list_size, Vector2D const& wall_center, Orientation const& wall_or, Vector2D & X1, Vector2D & X2, Vector2D & X1_P, Vector2D & X2_P);
int get_wall_intersection(Segment const& a, Vector2D const& wall_center, Orientation const& wall_or, double & x, double & y, double & x_p, double & y_p);
int get_wall_line_intersection(Segment const& a, Vector2D const& wall_center, Orientation const& wall_or, double & x, double & y, double & x_p, double & y_p);

#endif
