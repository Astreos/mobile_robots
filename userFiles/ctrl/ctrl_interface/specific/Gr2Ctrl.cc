#include "Gr2Ctrl.hh"
#include "ctrl_main_gr2.h"
#include <iostream>

using namespace ctrlGr2;

/*! \brief constructor
 */
Gr2Ctrl::Gr2Ctrl(int robot_id, int nb_opp): Controller(GR2_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr2Ctrl::~Gr2Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr2Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr2Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr2Ctrl::close_controller()
{
	controller_finish(cvs);
}
