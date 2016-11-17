#include "path_regulation_gr18.h"
#include "useful_gr18.h"
#include "speed_regulation_gr18.h"
#include "init_pos_gr18.h"
#include <math.h>

NAMESPACE_INIT(ctrlGr18);

/*! \brief follow a given path
 *
 * \param[in,out] cvs controller main structure
 */
void follow_path(CtrlStruct *cvs)
{

}

int turn(CtrlStruct *cvs, double theta_ref, int sens)
{
    double dt;
    int team_id;
    int dir;

    // variables declaration
    RobotPosition *rob_pos;
    CtrlIn *inputs;
    CtrlOut *outputs;
    PosRegulation *pos_reg;

    // variables initialization
    inputs  = cvs->inputs;
    outputs = cvs->outputs;
    rob_pos = cvs->rob_pos;
    team_id = cvs->team_id;
    pos_reg  = cvs->pos_reg;

    // time
    dt = inputs->t - pos_reg->last_t; // time interval since last call

    if (sens == 0)
    {
        dir = team(team_id);
    }
    else
    {
        dir = team(team_id)*sens;
    }

    // ----- Wheels regulation computation start ----- //

    int Kp = 20; //Kc = 90
    float Ti = 100; //Tosc = 0.0057143 s

    pos_reg->int_error_r = (theta_ref - rob_pos->theta)*dt + pos_reg->int_error_r;
    pos_reg->int_error_l = -(theta_ref - rob_pos->theta)*dt + pos_reg->int_error_l;

    speed_regulation(cvs, (Kp*(theta_ref - rob_pos->theta) + (Kp/Ti)*limit_range(pos_reg->int_error_r, -2.0*M_PI, 2.0*M_PI))*dir, -(Kp*(theta_ref - rob_pos->theta) + (Kp/Ti)*limit_range(pos_reg->int_error_r, -2.0*M_PI, 2.0*M_PI))*dir);

    // ----- Wheels regulation computation end ----- //

    if (fabs(theta_ref - rob_pos->theta) < 0.001)
    {
        return 1;
    }
    else
    {
        return 0;
    }

    // last update time
    pos_reg->last_t = inputs->t;
}

int run_x(CtrlStruct *cvs, double x_ref)
{
    double dt;

    // variables declaration
    RobotPosition *rob_pos;
    CtrlIn *inputs;
    CtrlOut *outputs;
    PosRegulation *pos_reg;

    // variables initialization
    inputs  = cvs->inputs;
    outputs = cvs->outputs;
    rob_pos = cvs->rob_pos;
    pos_reg  = cvs->pos_reg;

    // time
    dt = inputs->t - pos_reg->last_t; // time interval since last call

    // ----- Wheels regulation computation start ----- //

    int Kp = 50;
    float Ti = 100;

    pos_reg->int_error_r = (x_ref - rob_pos->x)*dt + pos_reg->int_error_r;
    pos_reg->int_error_l = (x_ref - rob_pos->x)*dt + pos_reg->int_error_l;

    speed_regulation(cvs, Kp*(x_ref - rob_pos->x) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -3.0, 3.0), Kp*(x_ref - rob_pos->x) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -3.0, 3.0));

    // ----- Wheels regulation computation end ----- //

    if (fabs(x_ref - rob_pos->x) < 0.0001)
    {
        return 1;
    }
    else
    {
        return 0;
    }

    // last update time
    pos_reg->last_t = inputs->t;
}

int run_y(CtrlStruct *cvs, double y_ref)
{
    double dt;
    int flag = 0;

    // variables declaration
    RobotPosition *rob_pos;
    CtrlIn *inputs;
    CtrlOut *outputs;
    PosRegulation *pos_reg;

    // variables initialization
    inputs  = cvs->inputs;
    outputs = cvs->outputs;
    rob_pos = cvs->rob_pos;
    pos_reg  = cvs->pos_reg;

    // time
    dt = inputs->t - pos_reg->last_t; // time interval since last call

    // ----- Wheels regulation computation start ----- //

    int Kp = 50;
    float Ti = 100;

    pos_reg->int_error_r = (y_ref - rob_pos->y)*dt + pos_reg->int_error_r;
    pos_reg->int_error_l = (y_ref - rob_pos->y)*dt + pos_reg->int_error_l;

    speed_regulation(cvs, -Kp*(y_ref - rob_pos->y) - (Kp/Ti)*limit_range(pos_reg->int_error_r, -3.0, 3.0), -Kp*(y_ref - rob_pos->y) - (Kp/Ti)*limit_range(pos_reg->int_error_l, -3.0, 3.0));

    // ----- Wheels regulation computation end ----- //

    if (fabs(y_ref - rob_pos->y) < 0.0001)
    {
        return 1;
    }
    else
    {
        return 0;
    }

    // last update time
    pos_reg->last_t = inputs->t;
}

NAMESPACE_CLOSE();
