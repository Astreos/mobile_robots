#ifdef OPEN_GL

#include "AbstractPoint3D.hh"
#include "MbsWorld3D.hh"

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] world_3d 3D world
 * \param[in] name name of the component
 * \param[in] rel_mat relative matrix position
 */
AbstractPoint3D::AbstractPoint3D(MbsWorld3D *world_3d, const char* name, glm::mat4 const& rel_mat):
    Mbs3DComponent(world_3d, name)
{
    comp_type = COMP_TYPE_ABSTRACT;

    this->rel_mat = rel_mat;
}

/*! \brief destructor
 */
AbstractPoint3D::~AbstractPoint3D()
{

}

/*! \brief comopute the absolute matrix
 * 
 * \param[in] parent_mat matrix of the parent
 */
void AbstractPoint3D::AbsMatCompute(glm::mat4 const& parent_mat)
{
    abs_mat = parent_mat * rel_mat;
}

}
#endif
