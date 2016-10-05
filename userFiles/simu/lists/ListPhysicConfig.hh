/*! 
 * \author Nicolas Van der Noot
 * \file ListPhysicConfig.hh
 * \brief ListPhysicConfig class
 */

#ifndef _LIST_PHYSIC_CONFIG_HH_
#define _LIST_PHYSIC_CONFIG_HH_

#include "mbs_data.h"
#include "ListPhysicElements.hh"

/*! \brief reading the index lists initialization class from the config file
 * 
 * Detailed class description
 */
class ListPhysicConfig: public ListPhysicElements
{
	public:
		ListPhysicConfig(MbsData *mbs_data);
		virtual ~ListPhysicConfig();
};

#endif
