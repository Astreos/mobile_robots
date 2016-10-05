#include "TimeGestion.hh"

/*! \brief constructor
 * 
 * \param[in] list_elem list of elements
 * \param[in] mbs_data Robotran structure
 */
TimeGestion::TimeGestion(ListElements *list_elem, MbsData *mbs_data): Gestion(list_elem)
{
	this->mbs_data = mbs_data;

	calib_time = 14.9;
	game_time  = 90.0;

	check_init_time   = 0.1;
	check_finish_time = 10.0;

	remove_time = 0.0 - (calib_time + check_init_time);
	t = mbs_data->tsim + remove_time;

	t_start = 0.0;
	t_check_init   = t_start - check_init_time;
	t_check_finish = t_start + game_time;
	t_finish = t_check_finish + check_finish_time;

	check_state();
}

/*! \brief destructor
 */
TimeGestion::~TimeGestion()
{

}

/*! \brief update the time control
 */
void TimeGestion::compute()
{
	t = mbs_data->tsim + remove_time;

	check_state();
}

/*! \brief check the game state
 */
void TimeGestion::check_state()
{
	if (t < t_check_init)
	{
		state = CALIBRATION_GAME;
	}
	else if (t < t_start)
	{
		state = CHECK_INIT_GAME;
	}
	else if (t < t_check_finish)
	{
		state = RUN_GAME;
	}
	else if (t < t_finish)
	{
		state = CHECK_FINISH_GAME;
	}
	else
	{
		state = FINISH_GAME;
	}
}
