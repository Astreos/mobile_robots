#ifdef OPEN_GL

#include "MbsViewPointFPS.hh"
#include "MbsWorld3D.hh"

#include <gtx/transform.hpp>

#include <glfw3.h>
#include <cmath>

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] position position
 * \param[in] look_at look direction
 * \param[in] z_up up vector
 * \param[in] FoV_rad field of view (perspective [rad])
 * \param[in] world_3d 3D world
 */
MbsViewPointFPS::MbsViewPointFPS(glm::vec3 const& position, glm::vec3 const& look_at, glm::vec3 const& z_up,
    float FoV_rad, MbsWorld3D *world_3d):
    MbsViewPoint(world_3d)
{
    this->position = position;
    this->look_at  = look_at;
    this->z_up     = z_up;

    this->FoV_rad = FoV_rad;
    FoV_deg = glm::degrees(FoV_rad);

    key_speed   = 3.0;
    mouse_speed = 1.0e-3;

    hor_angle  = M_PI;
    vert_angle = 0.0;

    lastTime = glfwGetTime();
}

/*! \brief destructor
 */
MbsViewPointFPS::~MbsViewPointFPS()
{
    
}

/*! \brief update the view matrix
 */
void MbsViewPointFPS::UpdateView()
{
    view_mat = glm::lookAt(position, look_at, z_up);
}

/*! \brief update the viewpoint
 */
void MbsViewPointFPS::Update()
{
    float dt;
    double x_pos, y_pos;

    // time elapsed since last call
    dt = world_3d->GetT() - lastTime;

    lastTime = world_3d->GetT();

    // mouse position
    glfwGetCursorPos(win, &x_pos, &y_pos);

    // reset mouse position
    glfwSetCursorPos(win, x_width/2, y_width/2);

    // new orientation
    hor_angle  += mouse_speed * float(x_width/2 - x_pos);
    vert_angle += mouse_speed * float(y_width/2 - y_pos);

    // direction : spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
        cos(vert_angle) * sin(hor_angle), 
        sin(vert_angle),
        cos(vert_angle) * cos(hor_angle)
    );
    
    // right vector
    glm::vec3 right = glm::vec3(
        sin(hor_angle - 3.14/2.0), 
        0.0,
        cos(hor_angle - 3.14/2.0)
    );
    
    // up vector
    z_up = glm::cross( right, direction );

    // move forward
    if (glfwGetKey( win, GLFW_KEY_UP ) == GLFW_PRESS)
    {
        position += direction * dt * key_speed;
    }
    // move backward
    if (glfwGetKey( win, GLFW_KEY_DOWN ) == GLFW_PRESS)
    {
        position -= direction * dt * key_speed;
    }
    // strafe right
    if (glfwGetKey( win, GLFW_KEY_RIGHT ) == GLFW_PRESS)
    {
        position += right * dt * key_speed;
    }
    // strafe left
    if (glfwGetKey( win, GLFW_KEY_LEFT ) == GLFW_PRESS)
    {
        position -= right * dt * key_speed;
    }

    // look-at direction
    look_at = position + direction;

    // update matrices
    UpdateMatrices();
}

}
#endif
