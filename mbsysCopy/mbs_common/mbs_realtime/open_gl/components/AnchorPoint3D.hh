/*! 
 * \author Nicolas Van der Noot
 * \file AnchorPoint3D.hh
 * \brief AnchorPoint3D class
 */
#ifdef OPEN_GL

#ifndef _ANCHOR_POINT_3D_HH_
#define _ANCHOR_POINT_3D_HH_

#include "AbstractPoint3D.hh"

namespace OpenGLMbs{

/*! \brief Anchor point 3D component
 */
class AnchorPoint3D: public AbstractPoint3D
{
    public:
        AnchorPoint3D(MbsWorld3D *world_3d, const char* name, glm::vec3 const& anchor_pos = glm::vec3(0.0f));
        virtual ~AnchorPoint3D();

    private:
        glm::vec3 anchor_pos; ///< anchor point position
};

}
#endif
#endif
