/*! 
 * \author Nicolas Van der Noot
 * \file AbstractPoint3D.hh
 * \brief AbstractPoint3D class
 */
#ifdef OPEN_GL

#ifndef _ABSTRACT_POINT_3D_HH_
#define _ABSTRACT_POINT_3D_HH_

#include "Mbs3DComponent.hh"

namespace OpenGLMbs{

/*! \brief Abstract point 3D component
 */
class AbstractPoint3D: public Mbs3DComponent
{
    public:
        AbstractPoint3D(MbsWorld3D *world_3d, const char* name, glm::mat4 const& rel_mat = glm::mat4(1.0f));
        virtual ~AbstractPoint3D();

        void AbsMatCompute(glm::mat4 const& parent_mat);

    protected:
        glm::mat4 rel_mat; ///< relative position matrix
};

}
#endif
#endif
