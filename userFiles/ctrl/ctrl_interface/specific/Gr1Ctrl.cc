#include "Gr1Ctrl.hh"
#include "ctrl_main_gr1.h"
#include <iostream>

using namespace ctrlGr1;

/*! \brief constructor
 */
Gr1Ctrl::Gr1Ctrl(int robot_id, int nb_opp): Controller(GR1_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr1Ctrl::~Gr1Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr1Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr1Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr1Ctrl::close_controller()
{
	controller_finish(cvs);
}
