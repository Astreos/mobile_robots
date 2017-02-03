/*! 
 * \author Nicolas Van der Noot
 * \file MbsSpotLight.hh
 * \brief MbsSpotLight class
 */
#ifdef OPEN_GL

#ifndef _MBS_SPOT_LIGHT_HH_
#define _MBS_SPOT_LIGHT_HH_

#include "MbsPointLight.hh"

namespace OpenGLMbs{

/*! \brief light acting like a spot (point with restricted cone angle)
 */
class MbsSpotLight: public MbsPointLight
{
    public:
        MbsSpotLight(glm::vec3 const& rel_pos = glm::vec3(0.0f),
            glm::vec3 const& attenuation = glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3 const& rel_dir = glm::vec3(0.0f, 0.0f, -1.0f),
            glm::mat4 const& parent_mat = glm::mat4(1.0f),
            glm::vec3 const& color = glm::vec3(1.0f),
            float ambient = 0.0f,
            float spread_angle = 3.1415927f,
            float concentration = 0.0f,
            bool active = true);
        virtual ~MbsSpotLight();

        virtual void Update();

        /// get the direction
        glm::vec3 GetNormDir() const { return norm_dir; }

        /// get the spread angle
        float GetSpreadAngle()   const { return spread_angle;  }

        /// get the concentration
        float GetConcentration() const { return concentration; }

        /// update the relative direction
        void UpdateRelDir(glm::vec3 const& rel_dir)
        {
            this->rel_dir = rel_dir;
            rel_dir_4 = glm::vec4(rel_dir, 0.0f);

            Update();
        }

        /// update the parent position
        virtual void UpdateParentPos(glm::mat4 const& parent_mat)
        {
            MbsPointLight::UpdateParentPos(parent_mat);

            Update();
        }
        
    private:
        glm::vec3 dir;   ///< direction
        glm::vec4 dir_4; ///< direction (vec4)
        glm::vec3 norm_dir; ///< norm of the direction

        glm::vec3 rel_dir;   ///< relative direction
        glm::vec4 rel_dir_4; ///< relative direction (vec4)

        float spread_angle;  ///< spread angle [rad]
        float concentration; ///< concentration value
};

}
#endif
#endif
