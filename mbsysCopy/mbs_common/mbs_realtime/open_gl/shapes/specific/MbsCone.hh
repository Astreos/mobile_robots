/*! 
 * \author Nicolas Van der Noot
 * \file MbsCone.hh
 * \brief MbsCone class
 */
#ifndef _MBS_CONE_HH_
#define _MBS_CONE_HH_

#include "MbsShape3D.hh"

namespace OpenGLMbs{

class MbsCone: public MbsShape3D
{
    public:
        MbsCone(double r, double h, glm::vec3 const& color,
            std::string vertex_file, std::string fragment_file, MbsWorld3D *world_3d,
            float transparency = 0.0,
            glm::vec3 const& trans_pos   = glm::vec3(0.0f),
            glm::vec3 const& trans_scale = glm::vec3(1.0, 1.0, 1.0),
            glm::vec3 const& trans_rot   = glm::vec3(0.0f),
            const char* name = "none");
        virtual ~MbsCone();

    private:
        double r; ///< radius [m]
        double h; ///< height [m]
};

}
#endif
