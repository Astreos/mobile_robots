#include "Gr5Ctrl.hh"
#include "ctrl_main_gr5.h"
#include <iostream>

using namespace ctrlGr5;

/*! \brief constructor
 */
Gr5Ctrl::Gr5Ctrl(int robot_id, int nb_opp): Controller(GR5_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr5Ctrl::~Gr5Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr5Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr5Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr5Ctrl::close_controller()
{
	controller_finish(cvs);
}
