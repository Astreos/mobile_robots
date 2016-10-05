#include "Gr20Ctrl.hh"
#include "ctrl_main_gr20.h"
#include <iostream>

using namespace ctrlGr20;

/*! \brief constructor
 */
Gr20Ctrl::Gr20Ctrl(int robot_id, int nb_opp): Controller(GR20_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr20Ctrl::~Gr20Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr20Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr20Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr20Ctrl::close_controller()
{
	controller_finish(cvs);
}
