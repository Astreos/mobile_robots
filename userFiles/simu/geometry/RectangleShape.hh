/*! 
 * \author Nicolas Van der Noot
 * \file RectangleShape.hh
 * \brief RectangleShape class
 */

#ifndef __RECTANGLE__HH__
#define __RECTANGLE__HH__


#include "BasicShape.hh"
#include "Segment.hh"

/*! \brief RectangleShape geometry shape
 */
class RectangleShape: public BasicShape
{
	public:
		RectangleShape();
		RectangleShape(Vector2D const& init_center, double angle, double semi_width, double semi_height);

		virtual ~RectangleShape();

		virtual void update_segments();

		virtual void set_rectangle(Vector2D const& center, double angle, double semi_width, double semi_height);

		inline Segment  get_segment(int index) const { return segment_list[index]; }
		inline Vector2D get_point(int index)   const { return point_list[index];   }

		inline Segment* get_segment_list() { return segment_list; }

		inline int get_list_size() { return list_size; }

		virtual int contains_point(Vector2D const& point);
		virtual int intersects_segment(Segment const& seg);

		virtual void print_identity() { std::cout << "RectangleShape" << std::endl; }

		double get_semi_width()  const { return semi_width;  }
		double get_semi_height() const { return semi_height; }

	protected:
		int list_size; ///< number of segments (4 for a rectangle)

		double semi_width;  ///< semi width of the rectangle [m]
		double semi_height; ///< semi height of the rectangle [m]

		Segment segment_list[4]; ///< lists of the four segments
		Vector2D point_list[4];  ///< lists of the four points (corners of the rectangle)

		// function prototypes
		void update_seg();
};

#endif
