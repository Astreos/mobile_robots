#include "Gr12Ctrl.hh"
#include "ctrl_main_gr12.h"
#include <iostream>

using namespace ctrlGr12;

/*! \brief constructor
 */
Gr12Ctrl::Gr12Ctrl(int robot_id, int nb_opp): Controller(GR12_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr12Ctrl::~Gr12Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr12Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr12Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr12Ctrl::close_controller()
{
	controller_finish(cvs);
}
