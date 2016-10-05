#include "Gr6Ctrl.hh"
#include "ctrl_main_gr6.h"
#include <iostream>

using namespace ctrlGr6;

/*! \brief constructor
 */
Gr6Ctrl::Gr6Ctrl(int robot_id, int nb_opp): Controller(GR6_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr6Ctrl::~Gr6Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr6Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr6Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr6Ctrl::close_controller()
{
	controller_finish(cvs);
}
