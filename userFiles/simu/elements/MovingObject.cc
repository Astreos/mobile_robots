
#include "MovingObject.hh"

/// constructor
MovingObject::MovingObject(GeometryShape *geometry): PhysicElements(geometry)
{

}

/// destructor
MovingObject::~MovingObject()
{

}

/*! \brief check if it is a robot
 * 
 * \param[out] robot_index index of the robot (if robot)
 * \return 1 if robot, 0 otherwise
 */
int MovingObject::is_robot(int &robot_index)
{
	robot_index = -1;
	return 0;
}
