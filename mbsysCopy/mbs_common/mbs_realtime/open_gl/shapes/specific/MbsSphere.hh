/*! 
 * \author Nicolas Van der Noot
 * \file MbsSphere.hh
 * \brief MbsSphere class
 */
#ifndef _MBS_SPHERE_HH_
#define _MBS_SPHERE_HH_

#include "MbsShape3D.hh"

namespace OpenGLMbs{

class MbsSphere: public MbsShape3D
{
    public:
        MbsSphere(double r, glm::vec3 const& color,
            std::string vertex_file, std::string fragment_file, MbsWorld3D *world_3d,
            float transparency = 0.0,
            glm::vec3 const& trans_pos   = glm::vec3(0.0f),
            glm::vec3 const& trans_scale = glm::vec3(1.0, 1.0, 1.0),
            glm::vec3 const& trans_rot   = glm::vec3(0.0f),
            const char* name = "none");
        virtual ~MbsSphere();

    private:
        double r; ///< radius [m]
};

}
#endif
