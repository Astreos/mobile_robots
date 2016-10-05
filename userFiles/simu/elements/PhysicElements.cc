
#include "PhysicElements.hh"

#define MU_GCM 0.9   ///< friction coeff (mu) [-]
#define GRAVITY 9.81 ///< gravitational acceleration [m/s^2]

/// constructor
PhysicElements::PhysicElements(GeometryShape *geometry)
{
	this->geometry = geometry;

	mass    = 1.0;
	inertia = 1.0;

	set_kinetic_friction();
}

/// destructor
PhysicElements::~PhysicElements()
{
	delete geometry;
}

/*! \brief Change the kinetic friction coefficient
 */
void PhysicElements::set_kinetic_friction()
{
	kinetic_friction = GRAVITY * mass * MU_GCM;
}
