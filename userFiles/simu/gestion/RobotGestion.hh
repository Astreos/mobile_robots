/*! 
 * \author Nicolas Van der Noot
 * \file RobotGestion.hh
 * \brief RobotGestion class
 */

#ifndef _ROBOT_GESTION_HH_
#define _ROBOT_GESTION_HH_

#include "Gestion.hh"
#include "mbs_data.h"
#include "ActuatorGestion.hh"

/*! \brief gestion of the robots
 */
class RobotGestion: public Gestion
{
	public:
		RobotGestion(ListElements *list_elem, MbsData *mbs_data, ActuatorGestion *actuators_gestion);

		virtual ~RobotGestion();

		virtual void compute();

		void close_controllers();

	private:
		ListElements *list_elem; ///< list of all elements
		ListPhysicElements *list_phys_elem; ///< list of physic elements
		MbsData *mbs_data; ///< Robotran structure

		int nb_robot; ///< number of robots
		int flag_init; ///< 1 if initialization done, 0 otherwise

		ActuatorGestion *actuators_gestion; ///< robot actuators

		void set_inputs(int robot_id);
		void set_outputs(int robot_id);
};


#endif
