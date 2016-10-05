#include "Gr9Ctrl.hh"
#include "ctrl_main_gr9.h"
#include <iostream>

using namespace ctrlGr9;

/*! \brief constructor
 */
Gr9Ctrl::Gr9Ctrl(int robot_id, int nb_opp): Controller(GR9_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr9Ctrl::~Gr9Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr9Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr9Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr9Ctrl::close_controller()
{
	controller_finish(cvs);
}
