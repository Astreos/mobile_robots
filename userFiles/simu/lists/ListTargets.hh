/*! 
 * \author Nicolas Van der Noot
 * \file ListTargets.hh
 * \brief ListTargets class
 */

#ifndef _LIST_TARGETS_HH_
#define _LIST_TARGETS_HH_

#include "Target.hh"
#include <vector>

/*! \brief list of targets
 */
class ListTargets
{
	public:
		ListTargets();
		~ListTargets();

		int get_nb_targets() { return target_list.size(); }

		void fill_T1_T2_T3(int target_id, int &T1, int &T2, int &T3);
		void get_pos(int target_id, double &x, double &y, double &z);

		Target* get_target(int target_id) { return target_list[target_id]; }

	private:
		std::vector<Target*> target_list;
};

#endif
