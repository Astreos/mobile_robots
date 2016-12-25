#ifdef OPEN_GL

#include "MbsViewPointMouse.hh"
#include "MbsWorld3D.hh"

#include <iostream>
#include <stdlib.h>
#include <cmath>

#include <gtx/rotate_vector.hpp>
#include <glfw3.h>

namespace OpenGLMbs{

#define THRES_MOUSE 0.1 ///< threshold to sense mouse scrolling

// mouse states
enum {MOUSE_NO_STATE, MOUSE_LEFT_STATE, MOUSE_RIGHT_STATE, MOUSE_CENTER_STATE};

/// compute the index of the biggest value
inline int bigger_three(double a, double b, double c) { return (a >= b && a >= c) ? 0 : ((b >= c) ? 1 : 2); }

// global variable to get the mouse scrolling wheel
double y_scroll;

/*! \brief call when scrolling is applied
 * 
 * \param[in] window window
 * \param[in] x scrolling value in X
 * \param[in] y scrolling value in Y
 */
void scroll_callback(GLFWwindow* window, double x, double y)
{
    if (fabs(y) > THRES_MOUSE)
    {
        y_scroll = y;
    }
}

/*! \brief constructor
 *
 * \param[in] rel_pos relative position (with regard to the parent joint)
 * \param[in] rel_look_at relative look direction (with regard to the parent joint)
 * \param[in] up up vector
 * \param[in] world_3d 3D world
 * \param[in] pers_flag true for perspective, false for parallel view
 * \param[in] FoV_scale field of view (perspective [rad]) or scale (parallel [-])
 * \param[in] near near value for screen projection
 * \param[in] far far value for screen projection
 * \param[in] parent_mat parent matrix
 */
MbsViewPointMouse::MbsViewPointMouse(glm::vec3 const&rel_pos, glm::vec3 const&rel_look_at,
    glm::vec3 const&z_up, MbsWorld3D *world_3d,
    bool pers_flag, float FoV_scale, float near, float far, glm::mat4 const& parent_mat):
    MbsViewPoint(world_3d, pers_flag, parent_mat)
{
    glm::vec3 parent_pos(parent_mat[3][0], parent_mat[3][1], parent_mat[3][2]);

    position = rel_pos + parent_pos;
    look_at  = rel_look_at + parent_pos;

    this->z_up = z_up;

    if (pers_flag)
    {
        FoV_rad = FoV_scale;
        FoV_deg = glm::degrees(FoV_rad);
    }
    else
    {
        UpdateScale(FoV_scale);
    }

    this->near = near;
    this->far  = far;
    this->pers_flag = pers_flag;

    near = 0.1f;
    far  = 100.0f;

    // vector from position to point 'look_at'
    look_vec = look_at - position;

    // zoom factor
    move_fact = 2.0e-3f;
    zoom_fact = 1.0e-2f;
    scroll_fact = 0.15f;

    scroll_time = 1.0e-2;

    // intial mouse position
    init_x = 0.0;
    init_y = 0.0;

    // initial state
    state = MOUSE_NO_STATE;

    // initialize the view matrix using the look at function
    view_mat = glm::lookAt(position, look_at, z_up);
    
    // retrieve the rotation and position matrices from the lookAt matrix
    posMat = glm::translate(glm::mat4(1.0f), glm::vec3( view_mat[3][0], view_mat[3][1], view_mat[3][2]));
    rotMat = glm::mat4(1.0f);

    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            rotMat[i][j] = view_mat[i][j];
        }
    }

    // scroll
    last_y_scroll   = 0.0;
    t_rise_y_scroll = 0.0;
}

/*! \brief destructor
 */
MbsViewPointMouse::~MbsViewPointMouse()
{
    
}

/*! \brief apply parent to relative view
 * 
 * \param[in] child_look_at relative matrix for camera position
 * \param[in] parent_joint parent joint matrix absolute position
 * \return absolute matrix for camera position
 */
glm::mat4 MbsViewPointMouse::ParentApply(glm::mat4 const& child_look_at, glm::mat4 const& parent_joint)
{
    glm::mat4 new_look_at;
    glm::mat3 look_at_3x3;
    glm::vec3 trans_vec;
    glm::vec3 s, u, f;
    glm::vec4 s_rot, u_rot, f_rot;

    // translation done by parent
    trans_vec = glm::vec3(parent_joint[3][0], parent_joint[3][1], parent_joint[3][2]);

    // s, u, f computed by inverting the effect of 'glm::lookAt', and rotated according to parent_joint
    s_rot = parent_joint * glm::vec4( child_look_at[0][0],  child_look_at[1][0],  child_look_at[2][0], 0.0f);
    u_rot = parent_joint * glm::vec4( child_look_at[0][1],  child_look_at[1][1],  child_look_at[2][1], 0.0f);
    f_rot = parent_joint * glm::vec4(-child_look_at[0][2], -child_look_at[1][2], -child_look_at[2][2], 0.0f);

    // only keeping the first three components
    s = glm::vec3(s_rot[0], s_rot[1], s_rot[2]);
    u = glm::vec3(u_rot[0], u_rot[1], u_rot[2]);
    f = glm::vec3(f_rot[0], f_rot[1], f_rot[2]);

    // copy old matrix
    new_look_at = child_look_at;

    // new rotation
    new_look_at[0][0] = s.x;
    new_look_at[1][0] = s.y;
    new_look_at[2][0] = s.z;
    new_look_at[0][1] = u.x;
    new_look_at[1][1] = u.y;
    new_look_at[2][1] = u.z;
    new_look_at[0][2] =-f.x;
    new_look_at[1][2] =-f.y;
    new_look_at[2][2] =-f.z;

    // update position
    new_look_at[3][0] -= glm::dot(s, trans_vec);
    new_look_at[3][1] -= glm::dot(u, trans_vec);
    new_look_at[3][2] += glm::dot(f, trans_vec);

    // get eye position
    look_at_3x3 = glm::mat3(new_look_at);

    if (glm::determinant(look_at_3x3))
    {
        position = -glm::inverse(look_at_3x3) * glm::vec3(new_look_at[3][0], new_look_at[3][1], new_look_at[3][2]);
    }

    // return result
    return new_look_at;
}

/*! \brief update the view matrix
 */
void MbsViewPointMouse::UpdateView()
{
    view_mat = ParentApply(posMat * rotMat, parent_mat);
}

/*! \brief update the viewpoint
 */
void MbsViewPointMouse::Update()
{
    double cur_x, cur_y;
    float diff_x, diff_y;

    glm::vec3 right, localup;
    glm::mat4 utilityMat;

    // mouse current position
    glfwGetCursorPos(win, &cur_x, &cur_y);

    // mouse shift since last new state
    diff_x = cur_x - init_x;
    diff_y = cur_y - init_y;

    // scrolling wheel
    if (fabs(y_scroll) > THRES_MOUSE && fabs(last_y_scroll) < THRES_MOUSE)
    {
        t_rise_y_scroll = world_3d->GetT();
    }
    else if (fabs(y_scroll) > THRES_MOUSE &&
        fabs(last_y_scroll) > THRES_MOUSE &&
        world_3d->GetT() - t_rise_y_scroll > scroll_time)
    {
        y_scroll = 0.0;
    }

    last_y_scroll = y_scroll;

    // get scrolling value
    glfwSetScrollCallback(win, scroll_callback);

    // adapt view (scrolling)
    posMat = glm::translate(posMat, glm::vec3(0.0f, 0.0f , scroll_fact * y_scroll));

    if (!pers_flag)
    {
        ScaleFactor(1.0f + scroll_fact * y_scroll);
    }

    switch (state)
    {
        // no mouse button pressed
        case MOUSE_NO_STATE:
            // start pressing left button
            if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
            {
                state = MOUSE_LEFT_STATE;
            }
            // start pressing right button
            else if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
            {
                state = MOUSE_RIGHT_STATE;
            }
            // start pressing center button
            else if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
            {
                state = MOUSE_CENTER_STATE;
            }
            break;

        // left button pressed
        case MOUSE_LEFT_STATE:
            
            // adapt the rotation matrix
            utilityMat = glm::rotate(utilityMat, move_fact * diff_y, glm::vec3( 1.0f, 0.0f, 0.0f));
            utilityMat = glm::rotate(utilityMat, move_fact * diff_x, glm::vec3( 0.0f, 1.0f, 0.0f));
            rotMat = utilityMat * rotMat;

            // release left button
            if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE)
            {
                state = MOUSE_NO_STATE;
            }
            break;

        // right button pressed
        case MOUSE_RIGHT_STATE:

            // adapt view
            posMat = glm::translate(posMat, glm::vec3(move_fact * diff_x, -move_fact * diff_y, 0.0f));

            // release right button
            if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE)
            {
                state = MOUSE_NO_STATE;
            }
            break;

        // center button pressed
        case MOUSE_CENTER_STATE:

            // adapt view
            posMat = glm::translate(posMat, glm::vec3(0.0f, 0.0f , zoom_fact * diff_y));

            // release center button
            if (glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_3) == GLFW_RELEASE)
            {
                state = MOUSE_NO_STATE;
            }

            if (!pers_flag)
            {
                ScaleFactor(1.0f + zoom_fact * diff_y);
            }
            break;
    
        default:
            std::cout << "Error: unknown mouse state (" << state << ") !" << std::endl;
            exit(EXIT_FAILURE);
    }

    init_x = cur_x;
    init_y = cur_y;

    // update matrices
    UpdateMatrices();
}

}
#endif
