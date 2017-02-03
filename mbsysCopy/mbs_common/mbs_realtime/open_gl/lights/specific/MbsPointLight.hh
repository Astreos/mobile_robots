/*! 
 * \author Nicolas Van der Noot
 * \file MbsPointLight.hh
 * \brief MbsPointLight class
 */
#ifdef OPEN_GL

#ifndef _MBS_POINT_LIGHT_HH_
#define _MBS_POINT_LIGHT_HH_

#include "MbsLight.hh"

namespace OpenGLMbs{

class MbsPointLight: public MbsLight
{
    public:
        MbsPointLight(glm::vec3 const& rel_pos = glm::vec3(0.0f),
            glm::vec3 const& attenuation = glm::vec3(1.0f, 0.0f, 0.0f),
            glm::mat4 const& parent_mat = glm::mat4(1.0f),
            glm::vec3 const& color = glm::vec3(1.0f),
            float ambient = 0.0f,
            bool active = true);
        virtual ~MbsPointLight();

        virtual void Update();

        /// get the light position
        glm::vec3 GetPos() const { return pos; }

        /// get the attenuation
        glm::vec3 GetAttenuation() const { return attenuation; }

        /// update the relative position
        void UpdateRelPos(glm::vec3 const& rel_pos)
        {
            this->rel_pos = rel_pos;

            Update();
        }

        /// update the parent position
        virtual void UpdateParentPos(glm::mat4 const& parent_mat)
        {
            MbsLight::UpdateParentPos(parent_mat);

            Update();
        }

    protected:
        glm::vec3 attenuation; ///< attenuation

        glm::vec3 pos; ///< absolute position
        glm::vec3 rel_pos; ///< position relative to parent

        glm::mat4 mat_pos; ///< absolute position as a matrix
        glm::mat4 mat_rel; ///< relative position as a matrix
};

}
#endif
#endif
