/*! 
 * \author Nicolas Van der Noot
 * \file Gestion.hh
 * \brief Gestion class
 */

#ifndef __GESTION__HH__
#define __GESTION__HH__

#include <iostream>

#include "ListElements.hh"
#include "user_realtime.h"

/*! \brief Mother class of the gestion classes
 */
class Gestion
{
	public:
		Gestion(ListElements *list_elem);

		virtual ~Gestion();

		virtual void compute() = 0;

	protected:
		ListElements *list_elem; ///< list of all elements
};


#endif
