/*! 
 * \author Nicolas Van der Noot
 * \file MbsInline.hh
 * \brief MbsInline class
 */
#ifndef _MBS_INLINE_HH_
#define _MBS_INLINE_HH_

#include "MbsShape3D.hh"

namespace OpenGLMbs{

class MbsInline: public MbsShape3D
{
    public:
        MbsInline(std::string cad_file,
            std::string vertex_file, std::string fragment_file, MbsWorld3D *world_3d,
            float transparency = 0.0,
            glm::vec3 const& trans_pos   = glm::vec3(0.0f),
            glm::vec3 const& trans_scale = glm::vec3(1.0, 1.0, 1.0),
            glm::vec3 const& trans_rot   = glm::vec3(0.0f),
            const char* name = "none");
        virtual ~MbsInline();

    private:
        // OBJ
        bool LoadOBJ(std::string cad_file, std::string dir_file);
};

}
#endif
