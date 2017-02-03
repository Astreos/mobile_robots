#ifdef OPEN_GL

#include "MbsLight.hh"

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] parent_mat parent matrix
 * \param[in] active true if light active, 0 otherwise
 */
MbsLight::MbsLight(glm::mat4 const& parent_mat, glm::vec3 const& color, float ambient, bool active)
{
    this->parent_mat = parent_mat;
    this->active  = active;
    this->ambient = ambient;
    this->color   = color;
}

/*! \brief destructor
 */
MbsLight::~MbsLight()
{

}

}
#endif
