/*! 
 * \author Nicolas Van der Noot
 * \file ListPhysicInput.hh
 * \brief ListPhysicInput class
 */

#ifndef _LIST_PHYSIC_INPUT_HH_
#define _LIST_PHYSIC_INPUT_HH_

#include "ListPhysicElements.hh"

/*! \brief list of physical elements defined from the config file
 */
class ListPhysicInput: public ListPhysicElements
{
	public:
		ListPhysicInput(MbsData *mbs_data);
		virtual ~ListPhysicInput();

	private:
};

#endif
