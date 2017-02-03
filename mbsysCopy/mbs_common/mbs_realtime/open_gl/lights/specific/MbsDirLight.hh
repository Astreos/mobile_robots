/*! 
 * \author Nicolas Van der Noot
 * \file MbsDirLight.hh
 * \brief MbsDirLight class
 */
#ifdef OPEN_GL

#ifndef _MBS_DIR_LIGHT_HH_
#define _MBS_DIR_LIGHT_HH_

#include "MbsLight.hh"

namespace OpenGLMbs{

class MbsDirLight: public MbsLight
{
    public:
        MbsDirLight(glm::vec3 const& rel_dir = glm::vec3(0.0f, 0.0f, -1.0f),
            glm::mat4 const& parent_mat = glm::mat4(1.0f),
            glm::vec3 const& color = glm::vec3(1.0f),
            float ambient = 0.0f,
            bool active = true);
        virtual ~MbsDirLight();

        virtual void Update();

        /// get the direction
        glm::vec3 GetNormDir() const { return norm_dir; }

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
            MbsLight::UpdateParentPos(parent_mat);

            Update();
        }

    private:
        glm::vec3 dir;   ///< direction
        glm::vec4 dir_4; ///< direction (vec4)
        glm::vec3 norm_dir; ///< norm of the direction

        glm::vec3 rel_dir;   ///< relative direction
        glm::vec4 rel_dir_4; ///< relative direction (vec4)
};

}
#endif
#endif
