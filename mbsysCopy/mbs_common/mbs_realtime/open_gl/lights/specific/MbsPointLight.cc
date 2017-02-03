#ifdef OPEN_GL

#include "MbsPointLight.hh"

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] rel_pos relative (with regard to parent joint) position of the light
 * \param[in] attenuation attenuation of the light
 * \param[in] parent_mat parent matrix
 * \param[in] active true if light active, 0 otherwise
 */
MbsPointLight::MbsPointLight(glm::vec3 const& rel_pos,
    glm::vec3 const& attenuation,
    glm::mat4 const& parent_mat,
    glm::vec3 const& color,
    float ambient, bool active):
    MbsLight(parent_mat, color, ambient, active)
{
    type = POINT_LIGHT;

    this->rel_pos = rel_pos;
    this->attenuation = attenuation;

    Update();
}

/*! \brief destructor
 */
MbsPointLight::~MbsPointLight()
{

}

/*! \brief update the light
 */
void MbsPointLight::Update()
{
    if (active)
    {
        mat_rel = glm::mat4(1.0f);

        for(int i=0; i<3; i++)
        {
            mat_rel[3][i] = rel_pos[i];
        }

        mat_pos = parent_mat * mat_rel;

        for(int i=0; i<3; i++)
        {
            pos[i] = mat_pos[3][i];
        }
    }
}

}
#endif
