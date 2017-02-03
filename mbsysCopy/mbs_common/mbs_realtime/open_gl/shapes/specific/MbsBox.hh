/*! 
 * \author Nicolas Van der Noot
 * \file MbsBox.hh
 * \brief MbsBox class
 */
#ifndef _MBS_BOX_HH_
#define _MBS_BOX_HH_

#include "MbsShape3D.hh"

namespace OpenGLMbs{

class MbsBox: public MbsShape3D
{
    public:
        MbsBox(double d, double w, double h, glm::vec3 const& color,
            std::string vertex_file, std::string fragment_file, MbsWorld3D *world_3d,
            float transparency = 0.0,
            glm::vec3 const& trans_pos   = glm::vec3(0.0f),
            glm::vec3 const& trans_scale = glm::vec3(1.0, 1.0, 1.0),
            glm::vec3 const& trans_rot   = glm::vec3(0.0f),
            const char* name = "none");
        virtual ~MbsBox();

    private:
        double d; ///< depth  (size along the X axis) [m]
        double w; ///< width  (size along the Y axis) [m]
        double h; ///< height (size along the Z axis) [m]
};

}
#endif
