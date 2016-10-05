/*! 
 * \author Nicolas Van der Noot
 * \file VisuGestion.hh
 * \brief VisuGestion class
 */

#ifndef _VISU_GESTION_HH_
#define _VISU_GESTION_HH_

#include "Gestion.hh"
#include "mbs_data.h"
#include "ListPhysicElements.hh"
#include <vector>

/*! \brief gestion of the visualization
 */
class VisuGestion: public Gestion
{
	public:
		VisuGestion(ListElements *list_elem, MbsData *mbs_data);
		virtual ~VisuGestion();

		virtual void compute();

		void fill_q_vec(int *nb_q, double **q_vec);

	private:
		MbsData *mbs_data; ///< Robotran structure
		ListPhysicElements *list_phys_elem; ///< list of physical elements
		ListTargets *list_targets; ///< list with the targets

		std::vector<int> main_mbs_id; ///< IDs of the joints in the main MBS file
		std::vector<int> main_mbs_anim_id; ///< IDs in the animation mbs, corresponding to 'main_mbs_id'

		std::vector<double> q_vector; ///< vector corresponding to the 'q_vec' vector of 'user_realtime_visu.cc'

		// function prototypes
		void main_mbs();
		void moving_obj();
		void targets();
};

#endif
