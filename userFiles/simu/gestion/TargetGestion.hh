/*! 
 * \author Nicolas Van der Noot
 * \file TargetGestion.hh
 * \brief TargetGestion class
 */

#ifndef _TARGET_GESTION_HH_
#define _TARGET_GESTION_HH_

#include "Gestion.hh"
#include "mbs_data.h"
#include "ListTargets.hh"
#include "ListPhysicElements.hh"

/*! \brief gestion of the targets
 */
class TargetGestion: public Gestion
{
	public:
		TargetGestion(ListElements *list_elem, MbsData *mbs_data);
		virtual ~TargetGestion();

		virtual void compute();

	private:
		MbsData *mbs_data; ///< Robotran structure
		ListTargets *targets; ///< targets
		ListPhysicElements *list_phys_elem; ///< list of physical elements

		int nb_robot; ///< number of robots
};

#endif
