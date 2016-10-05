#include "Gr11Ctrl.hh"
#include "ctrl_main_gr11.h"
#include <iostream>

using namespace ctrlGr11;

/*! \brief constructor
 */
Gr11Ctrl::Gr11Ctrl(int robot_id, int nb_opp): Controller(GR11_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr11Ctrl::~Gr11Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr11Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr11Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr11Ctrl::close_controller()
{
	controller_finish(cvs);
}
