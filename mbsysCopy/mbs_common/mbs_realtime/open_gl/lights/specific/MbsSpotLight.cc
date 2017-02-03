#ifdef OPEN_GL

#include "MbsSpotLight.hh"

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] rel_pos relative (with regard to parent joint) position of the light
 * \param[in] attenuation attenuation of the light
 * \param[in] rel_dir relative (with regard to parent joint) direction of the light
 * \param[in] parent_mat parent matrix
 * \param[in] spread_angle sprend angle of the light [rad]
 * \param[in] concentration concentration value
 * \param[in] active true if light active, 0 otherwise
 */
MbsSpotLight::MbsSpotLight(glm::vec3 const& rel_pos,
    glm::vec3 const& attenuation,
    glm::vec3 const& rel_dir,
    glm::mat4 const& parent_mat,
    glm::vec3 const& color,
    float ambient, float spread_angle, float concentration, bool active):
    MbsPointLight(rel_pos, attenuation, parent_mat, color, ambient, active)
{
    type = SPOT_LIGHT;

    this->rel_dir = rel_dir;
    this->spread_angle = spread_angle;

    rel_dir_4 = glm::vec4(rel_dir, 0.0f);

    Update();
}

/*! \brief destructor
 */
MbsSpotLight::~MbsSpotLight()
{

}

/*! \brief update the light
 */
void MbsSpotLight::Update()
{
    float dir_length;

    if (active)
    {
        MbsPointLight::Update();

        dir_4 = parent_mat * rel_dir_4;

        dir = glm::vec3(dir_4);

        dir_length = glm::length(dir);

        norm_dir = (dir_length > 0.0) ? dir / dir_length : dir;
    }
}

}
#endif
