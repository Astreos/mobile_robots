/*! 
 * \author Nicolas Van der Noot
 * \file GlobalGestion.hh
 * \brief GlobalGestion class
 */

#ifndef _GLOBAL_GESTION_HH_
#define _GLOBAL_GESTION_HH_

#include <vector>
#include <iostream>

#include "mbs_data.h"
#include "user_realtime.h"
#include "Gestion.hh"
#include "ListElements.hh"
#include "ActuatorGestion.hh"
#include "ContactGestion.hh"
#include "PositionGestion.hh"

enum{TIME_GESTION, ROBOT_GESTION, TARGET_GESTION, VISU_GESTION, SCORE_GESTION, NB_GESTION};

/*! \brief Global gestion of the code
 */
class GlobalGestion
{
	public:
		GlobalGestion(MbsData *mbs_data, ListElements *list_elem, ActuatorGestion *actuators_gestion);
		virtual ~GlobalGestion();

		void compute();

		ListElements* get_list_elem() const { return list_elem; }
		Gestion* get_gestion(int i) const { return gestion_tab[i]; }

		PositionGestion* get_pos_gestion() { return pos_gestion; }
		ContactGestion* get_contact_gestion() { return contact_gestion; }

	private:
		MbsData *mbs_data; ///< Robotran structure

		std::vector<Gestion*> gestion_tab; ///< list with all gestion modules

		ListElements *list_elem; ///< list with all elements

		PositionGestion *pos_gestion; ///< position gestion
		ContactGestion *contact_gestion; ///< contact gestion
};

#endif
