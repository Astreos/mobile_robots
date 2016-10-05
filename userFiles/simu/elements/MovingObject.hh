/*! 
 * \author Nicolas Van der Noot
 * \file MovingObject.hh
 * \brief Moving object class
 */

#ifndef __MOVING_OBJECT__HH__
#define __MOVING_OBJECT__HH__


#include "PhysicElements.hh"
#include "Vector2D.hh"

/*! \brief Moving object
 * 
 * Object that can move but which isn't a robot
 */
class MovingObject: public PhysicElements
{
	public:
		MovingObject(GeometryShape *geometry);
		virtual ~MovingObject();

		virtual int is_robot(int &robot_index);
};


#endif
