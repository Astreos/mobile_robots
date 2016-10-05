#include "Gr15Ctrl.hh"
#include "ctrl_main_gr15.h"
#include <iostream>

using namespace ctrlGr15;

/*! \brief constructor
 */
Gr15Ctrl::Gr15Ctrl(int robot_id, int nb_opp): Controller(GR15_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr15Ctrl::~Gr15Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr15Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr15Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr15Ctrl::close_controller()
{
	controller_finish(cvs);
}
