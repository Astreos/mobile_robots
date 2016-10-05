/*! 
 * \author Nicolas Van der Noot
 * \file ScoreGestion.hh
 * \brief ScoreGestion class
 */

#ifndef _SCORE_GESTION_HH_
#define _SCORE_GESTION_HH_

#include "Gestion.hh"
#include "mbs_data.h"
#include "robot_id.h"
#include "TimeGestion.hh"
#include "RectangleShape.hh"
#include "Target.hh"
#include "ListBases.hh"

/*! \brief gestion of the game score
 */
class ScoreGestion: public Gestion
{
	public:
		ScoreGestion(ListElements *list_elem, MbsData *mbs_data);
		virtual ~ScoreGestion();

		virtual void compute();

		void set_contact(GeometryShape* geom_1, GeometryShape* geom_2, int robot_id_1, int robot_id_2);
		void add_target(Target *target, ListBases *bases);

	private:
		int flag_init; ///< 1 if initialization done, 0 otherwise

		TimeGestion *time_gestion; ///< time gestion
		MbsData *mbs_data; ///< Robotran structure

		ListPhysicElements *list_phys_elem; ///< list of physical elements
		ListTargets *list_targets; ///< list of targets
		ListBases *list_bases; ///< list of bases

		int score_A; ///< score of the team A
		int score_B; ///< score of the team B

		int nb_robot; ///< number of robots
		int final_printed; ///< 1 if final score printed, 0 otherwise

		double speed_motion; ///< threshold to detect the motion

		double last_t_penalty; ///< last time a penalty was given [s]
		double no_contact_time; ///< time when no contact is taken into account [s]

		int penalty_init_A; ///< initial penalty for the team A
		int penalty_init_B; ///< initial penalty for the team B

		int penalty_finish_A; ///< final penalty for the team A
		int penalty_finish_B; ///< final penalty for the team B

		RectangleShape *init_basis_A; ///< initial basis for the team A
		RectangleShape *init_basis_B; ///< initial basis for the team B

		// function prototypes
		void check_init();
		void check_end();
		void print_final();
};

#endif
