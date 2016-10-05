/*! 
 * \author Nicolas Van der Noot
 * \file ListBases.hh
 * \brief ListBases class
 */

#ifndef _BASES_LIST_HH_
#define _BASES_LIST_HH_

#include "Basis.hh"
#include <vector>

/*! \brief list of all the bases of the teams
 */
class ListBases
{
	public:
		ListBases();
		~ListBases();

		int target_full_inside_team(Target *cur_target, int team_id);
		int target_full_inside_any(Target *cur_target);

	private:
		std::vector<Basis*> team_A_bases;
		std::vector<Basis*> team_B_bases;
};

#endif
