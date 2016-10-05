/*! 
 * \author Nicolas Van der Noot
 * \file FixedObject.hh
 * \brief FixedObject class
 */

#ifndef __FIXED_OBJECT__HH__
#define __FIXED_OBJECT__HH__


#include "PhysicElements.hh"

/*! \brief Fixed object
 * 
 * Objects that can interact with moving objects, but that cannot move
 */
class FixedObject: public PhysicElements
{
	public:
		FixedObject(GeometryShape *geometry);
		virtual ~FixedObject();

		virtual int is_robot(int &robot_index);
};


#endif
