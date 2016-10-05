#include "Gr4Ctrl.hh"
#include "ctrl_main_gr4.h"
#include <iostream>

using namespace ctrlGr4;

/*! \brief constructor
 */
Gr4Ctrl::Gr4Ctrl(int robot_id, int nb_opp): Controller(GR4_CTRL, robot_id, nb_opp)
{
	cvs = init_CtrlStruct(inputs, outputs);
}

/*! \brief destructor
 */
Gr4Ctrl::~Gr4Ctrl()
{
	free_CtrlStruct(cvs);
}

/*! \brief controller initialization
 */
void Gr4Ctrl::init_controller()
{
	controller_init(cvs);
}

/*! \brief controller loop
 */
void Gr4Ctrl::loop_controller()
{
	controller_loop(cvs);
}

/*! \brief controller close operations
 */
void Gr4Ctrl::close_controller()
{
	controller_finish(cvs);
}
