#include "Gr19Ctrl.hh"
#include "ctrl_main_gr19.h"
#include <iostream>

using namespace ctrlGr19;

/*! \brief constructor
 */
Gr19Ctrl::Gr19Ctrl(int robot_id, int nb_opp): Controller(GR19_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr19Ctrl::~Gr19Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr19Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr19Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr19Ctrl::close_controller()
{
	controller_finish(cvs);
}
