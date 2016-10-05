/*! 
 * \author Nicolas Van der Noot
 * \file ListPhysicHard.hh
 * \brief ListPhysicHard class
 */

#ifndef _LIST_PHYSIC_HARD_HH_
#define _LIST_PHYSIC_HARD_HH_

#include "mbs_data.h"
#include "ListPhysicElements.hh"

/*! \brief Hardcoding the index lists initialization class
 * 
 * Detailed class description
 */
class ListPhysicHard: public ListPhysicElements
{
	public:
		ListPhysicHard(MbsData *mbs_data);
		virtual ~ListPhysicHard();
};

#endif
