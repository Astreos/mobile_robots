/*! 
 * \author Nicolas Van der Noot
 * \file NoContactElements.hh
 * \brief NoContactElements class
 */

#ifndef __NO_CONTACT_ELEMENTS__HH__
#define __NO_CONTACT_ELEMENTS__HH__


#include "Elements.hh"

/*! \brief Physical object which can not produce contact forces
 */
class NoContactElements: public Elements
{
	public:
		NoContactElements();
		virtual ~NoContactElements();
};


#endif
