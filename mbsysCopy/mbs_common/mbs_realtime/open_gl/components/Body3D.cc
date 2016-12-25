#ifdef OPEN_GL

#include "Body3D.hh"
#include "MbsWorld3D.hh"
#include "AnchorPoint3D.hh"
#include <iostream>

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] world_3d 3D world
 * \param[in] name name of the component
 * \param[in] abs_mat absolute matrix position
 */
Body3D::Body3D(MbsWorld3D *world_3d, const char* name, glm::mat4 const& abs_mat):
    Mbs3DComponent(world_3d, name, abs_mat)
{
    comp_type = COMP_TYPE_BODY;
}

/*! \brief destructor
 */
Body3D::~Body3D()
{

}

/*! \brief update the absolute matrix of the 3D shape
 */
void Body3D::UpdateShapes3DMat()
{
    for(unsigned int i=0; i<shape_3d_list.size(); i++)
    {
        shape_3d_list[i]->SetModelMat(abs_mat);
    }
}

/*! \brief add an anchor point
 * 
 * \param[in] name name of the new anchor point
 * \param[in] anchor_pos new anchor point position
 * \return pointer to the new anchor point
 */
AnchorPoint3D* Body3D::AddAnchor(const char* name, glm::vec3 const& anchor_pos)
{
    AnchorPoint3D *new_anchor = new AnchorPoint3D(world_3d, name, anchor_pos);

    anchor_child_list.push_back(new_anchor);
    mbs_3d_child_list.push_back(new_anchor);

    return new_anchor;
}

/*! \brief update the absolute matrix for the whole tree starting at this component (recursively)
 */
void Body3D::UpdateAbsMatTree()
{
    UpdateShapes3DMat();

    for(unsigned int i=0; i<anchor_child_list.size(); i++)
    {
        anchor_child_list[i]->AbsMatCompute(abs_mat);
    }

    // must be called at the end
    Mbs3DComponent::UpdateAbsMatTree();
}

/*! \brief print the whole tree, starting at this component
 * 
 * \param[in] level recursion level of the current component
 */
void Body3D::PrintTree(int level)
{
    PrintName(level);

    for(unsigned int i=0; i<shape_3d_list.size(); i++)
    {
        for(int j=0; j<level+1; j++)
        {
            std::cout << "    ";
        }

        std::cout << "-> MbsShape3D : " << shape_3d_list[i] << std::endl;
    }

    PrintChildList(level);
}

}
#endif
