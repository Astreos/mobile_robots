/*! 
 * \author Nicolas Van der Noot
 * \file MbsLight.hh
 * \brief MbsLight class
 */
#ifdef OPEN_GL

#ifndef _MBS_LIGHT_HH_
#define _MBS_LIGHT_HH_

#ifndef degreesToRadians
    #define degreesToRadians(x) x*(3.141592f/180.0f)
#endif

#ifndef GLM_FORCE_RADIANS
    #define GLM_FORCE_RADIANS
#endif

#include <glm.hpp>

namespace OpenGLMbs{

// types of lights
enum {POINT_LIGHT, SPOT_LIGHT, DIR_LIGHT, NB_LIGHT_TYPES};

/*! \brief light element
 */
class MbsLight
{
    public:
        MbsLight(glm::mat4 const& parent_mat = glm::mat4(1.0f), glm::vec3 const& color = glm::vec3(1.0f),
            float ambient = 0.0f, bool active = true);
        virtual ~MbsLight();

        virtual void Update() = 0;

        /// update the parent position
        virtual void UpdateParentPos(glm::mat4 const& parent_mat) { this->parent_mat = parent_mat; }

        /// return true if current light active
        bool IsActive() const { return active; }

        /// get the type of light
        int GetType() const { return type; }

        /// get color
        glm::vec3 GetColor() const { return color; }

        // get ambient coefficient
        float GetAmbient() const { return ambient; }

    protected:
        glm::mat4 parent_mat; ///< parent position

        glm::vec3 color; ///< light color

        float ambient; ///< ambient coefficient

        int type; ///< type of light

        bool active; ///< true if active, false otherwise
};

}
#endif
#endif
