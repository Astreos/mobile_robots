#ifdef OPEN_GL

#include "AnchorPoint3D.hh"
#include "MbsWorld3D.hh"

#include <gtx/transform.hpp>

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] world_3d 3D world
 * \param[in] name name of the component
 * \param[in] anchor_pos anchor point position
 */
AnchorPoint3D::AnchorPoint3D(MbsWorld3D *world_3d, const char* name, glm::vec3 const& anchor_pos):
    AbstractPoint3D(world_3d, name)
{
    comp_type = COMP_TYPE_ANCHOR;

    this->anchor_pos = anchor_pos;

    rel_mat = TransMat(anchor_pos[0], anchor_pos[1], anchor_pos[2]);
}

/*! \brief destructor
 */
AnchorPoint3D::~AnchorPoint3D()
{

}

}
#endif
