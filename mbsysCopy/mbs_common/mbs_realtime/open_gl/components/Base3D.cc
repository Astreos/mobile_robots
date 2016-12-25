#ifdef OPEN_GL

#include "Base3D.hh"
#include "MbsWorld3D.hh"

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] world_3d 3D world
 */
Base3D::Base3D(MbsWorld3D *world_3d):
    Joint3D(world_3d, "base", NO_JOINT)
{
    comp_type = COMP_TYPE_BASE;
}

/*! \brief destructor
 */
Base3D::~Base3D()
{

}

}
#endif
