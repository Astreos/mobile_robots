/*! 
 * \author Nicolas Van der Noot
 * \file TimeGestion.hh
 * \brief TimeGestion class
 */

#ifndef _TIME_GESTION_HH_
#define _TIME_GESTION_HH_

#include "Gestion.hh"

// states of the game
enum {CALIBRATION_GAME, CHECK_INIT_GAME, RUN_GAME, CHECK_FINISH_GAME, FINISH_GAME};

/*! \brief gestion of the timing
 */
class TimeGestion: public Gestion
{
	public:
		TimeGestion(ListElements *list_elem, MbsData *mbs_data);
		virtual ~TimeGestion();

		virtual void compute();

		double get_t() const { return t; }

		int get_state() const { return state; }

	private:
		MbsData *mbs_data; ///< Robotran structure

		double t; ///< game time

		double t_start;        ///< 't' to start the game [s]
		double t_check_init;   ///< 't' to start checking the initialization [s]
		double t_check_finish; ///< 't' to start checking the finilization [s]
		double t_finish;       ///< 't' to finish (print results) [s]


		double calib_time; ///< calibration time [s]
		double game_time;  ///< game time [s]
		double check_init_time;   ///< time needed to check the initialization [s]
		double check_finish_time; ///< time needed to check the final time [s]
		double remove_time; ///< time removed from the input time sensor [s]

		int state; ///< state of the game

		// function prototype
		void check_state();
};

#endif

		