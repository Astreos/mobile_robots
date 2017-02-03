#ifdef OPEN_GL

#include "MbsDirLight.hh"

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] rel_dir relative (with regard to parent joint) direction of the light
 * \param[in] parent_mat parent matrix
 * \param[in] active true if light active, 0 otherwise
 */
MbsDirLight::MbsDirLight(glm::vec3 const& rel_dir, glm::mat4 const& parent_mat, glm::vec3 const& color, float ambient, bool active):
    MbsLight(parent_mat, color, ambient, active)
{
    type = DIR_LIGHT;

    this->rel_dir = rel_dir;

    rel_dir_4 = glm::vec4(rel_dir, 0.0f);

    Update();
}

/*! \brief destructor
 */
MbsDirLight::~MbsDirLight()
{

}

/*! \brief update the light
 */
void MbsDirLight::Update()
{
    float dir_length;

    if (active)
    {
        dir_4 = parent_mat * rel_dir_4;

        dir = glm::vec3(dir_4);

        dir_length = glm::length(dir);

        norm_dir = (dir_length > 0.0) ? dir / dir_length : dir;
    }
}

}
#endif
