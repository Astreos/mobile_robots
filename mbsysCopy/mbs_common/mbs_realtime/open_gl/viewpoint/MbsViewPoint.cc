#ifdef OPEN_GL

#include "MbsViewPoint.hh"
#include "MbsWorld3D.hh"

#include <gtx/transform.hpp>

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] world_3d 3D world
 * \param[in] pers_flag true for perspective, false for parallel view
 * \param[in] parent_mat parent matrix
 */
MbsViewPoint::MbsViewPoint(MbsWorld3D *world_3d, bool pers_flag, glm::mat4 const& parent_mat):
    position(glm::vec3(1.0, 1.0, 1.0)),
    look_at(glm::vec3(0.0f)),
    z_up(glm::vec3(0.0, 0.0, 1.0)),
    rotMat(1.0),
    posMat(1.0),
    view_mat(1.0),
    proj_mat(1.0)
{
    this->world_3d = world_3d;

    this->parent_mat = parent_mat;

    this->pers_flag = pers_flag;

    win = world_3d->getWin();

    near = 0.1f;
    far  = 100.0f;

    FoV_rad = 0.785f;
    FoV_deg = glm::degrees(FoV_rad);

    UpdateScale(0.2f);

    SetWinSize(world_3d->GetXWidth(), world_3d->GetYWidth());
}

/*! \brief destructor
 */
MbsViewPoint::~MbsViewPoint()
{

}

/*! \brief set the windows size
 * 
 * \param[in] x_width width (along the X axis) in pixels
 * \param[in] y_width width (along the Y axis) in pixels
 */
void MbsViewPoint::SetWinSize(int x_width, int y_width)
{
    this->x_width = x_width;
    this->y_width = y_width;

    x_width_f = (float) x_width;
    y_width_f = (float) y_width;

    semi_x_width_f = x_width_f / 2.0f;
    semi_y_width_f = y_width_f / 2.0f;

    UpdateProjection();
}

/*! \brief update the projection matrix
 */
void MbsViewPoint::UpdateProjection()
{
    // perspective projection
    if (pers_flag)
    {
        proj_mat = glm::perspective(FoV_rad, x_width_f / y_width_f, near, far);
    }
    // parallel projection (seems not working)
    else
    {
        proj_mat = glm::ortho(
            -semi_x_width_f*par_fact, semi_x_width_f*par_fact,
            -semi_y_width_f*par_fact, semi_y_width_f*par_fact,
            near, far);
    }
}

/*! \brief update all the matrices
 */
void MbsViewPoint::UpdateMatrices()
{
    UpdateProjection();

    UpdateView();
}

/*! \brief update the scaling value (for parallel view)
 *
 * \param[in] scale new scaling value
 */
void MbsViewPoint::UpdateScale(float scale)
{
    this->scale = scale;
    par_fact = (scale > 0.0f) ? 5.0e-4f / scale : 5.0e-4f;
}

/*! \brief update the scaling value with a factor (for parallel view)
 *
 * \param[in] factor factor to apply to scaling
 */
void MbsViewPoint::ScaleFactor(float factor)
{
    UpdateScale(scale * factor);
}

}
#endif
