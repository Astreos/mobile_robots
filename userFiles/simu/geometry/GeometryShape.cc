
#include "GeometryShape.hh"

/// constructor
GeometryShape::GeometryShape(): force(0.0, 0.0)
{
	torque = 0.0;

	flag_contact = 1;
	flag_uSwitch = 1;

	index_elem = FIXED_TYPE;
}

/// destructor
GeometryShape::~GeometryShape()
{

}

/*! \brief print the position
 */
void GeometryShape::print_pos()
{
	std::cout << "x: " << pose.get_x() << " ; y: " << pose.get_y() << "" << std::endl;

}
