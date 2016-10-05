/*! 
 * \author Nicolas Van der Noot
 * \file ListElements.hh
 * \brief ListElements class
 */
#ifndef __LIST_ELEMENTS__HH__
#define __LIST_ELEMENTS__HH__

#include "ListPhysicElements.hh"
#include "ListTargets.hh"
#include "ListBases.hh"
#include "mbs_data.h"


/*! \brief List with all elements
 */
class ListElements
{
	public:
		ListElements(MbsData *mbs_data);
		virtual ~ListElements();

		ListPhysicElements* get_list_phys_elem() { return list_phys_elem; }

		ListTargets* get_list_targets() { return list_targets; }

		ListBases* get_list_bases() { return list_bases; }

	protected:
		ListPhysicElements *list_phys_elem; ///< list with all physic elements
		ListTargets *list_targets; ///< list with the targets
		ListBases *list_bases; ///< list with bases

		MbsData *mbs_data; ///< Robotran structure
};

#endif
