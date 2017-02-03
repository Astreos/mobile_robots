#ifdef OPEN_GL

#include "Joint3D.hh"
#include "MbsWorld3D.hh"
#include "Body3D.hh"

#include <iostream>
#include <stdlib.h>

#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] world_3d 3D world
 * \param[in] name name of the component
 * \param[in] joint_type type of joint
 * \param[in] joint_value value of a joint
 */
Joint3D::Joint3D(MbsWorld3D *world_3d, const char* name, int joint_type, float joint_value):
    AbstractPoint3D(world_3d, name)
{
    comp_type = COMP_TYPE_JOINT;

    this->joint_type = joint_type;

    UpdateRelMat(joint_value);

    body_3d_child = NULL;
}

/*! \brief destructor
 */
Joint3D::~Joint3D()
{

}

/*! \brief add a body component
 * 
 * \param[in] name new body name
 * \return pointer to the new body
 */
Body3D* Joint3D::AddBody(const char* name)
{
    // safety
    if (body_3d_child != NULL)
    {
        std::cout << "Joint3D warning: only one body can be added to a joint !" << std::endl;
        return NULL;
    }

    body_3d_child = new Body3D(world_3d, name, abs_mat);

    mbs_3d_child_list.push_back(body_3d_child);

    return body_3d_child;
}

/*! \brief update the relative matrix
 * 
 * \param[in,out] joint_value value of the new joint ([m] or [rad])
 */
void Joint3D::UpdateRelMat(float joint_value)
{
    this->joint_value = joint_value;

    switch (joint_type)
    {
        case JOINT_T1:
            rel_mat = TransMat(joint_value, 0.0f, 0.0f);
            break;

        case JOINT_T2:
            rel_mat = TransMat(0.0f, joint_value, 0.0f);
            break;

        case JOINT_T3:
            rel_mat = TransMat(0.0f, 0.0f, joint_value);
            break;

        case JOINT_R1:
            rel_mat = RotMatX(joint_value);
            break;

        case JOINT_R2:
            rel_mat = RotMatY(joint_value);
            break;

        case JOINT_R3:
            rel_mat = RotMatZ(joint_value);
            break;

        case NO_JOINT:
            break;
    
        default:
            std::cout << "Joint3D error: unknown joint type (" << joint_type << ") !" << std::endl;
            exit(EXIT_FAILURE);
    }
}

/*! \brief update the lights
 */
void Joint3D::UpdateLights()
{
    for(unsigned int i=0; i<light_list.size(); i++)
    {
        light_list[i]->UpdateParentPos(abs_mat);
    }
}

/*! \brief update the viewpoints
 */
void Joint3D::UpdateViewPoints()
{
    for(unsigned int i=0; i<view_list.size(); i++)
    {
        view_list[i]->UpdateParentPos(abs_mat);
    }
}

/*! \brief update the absolute matrix for the whole tree starting at this component (recursively)
 */
void Joint3D::UpdateAbsMatTree()
{
    UpdateLights();

    UpdateViewPoints();

    if (body_3d_child != NULL)
    {
        body_3d_child->SetAbsMat(abs_mat);
    }

    // must be called at the end
    Mbs3DComponent::UpdateAbsMatTree();
}

/*! \brief print the whole tree, starting at this component
 * 
 * \param[in] level recursion level of the current component
 */
void Joint3D::PrintTree(int level)
{
    PrintName(level);

    for(unsigned int i=0; i<light_list.size(); i++)
    {
        for(int j=0; j<level+1; j++)
        {
            std::cout << "    ";
        }

        std::cout << "-> MbsLight : " << light_list[i] << std::endl;
    }

    for(unsigned int i=0; i<view_list.size(); i++)
    {
        for(int j=0; j<level+1; j++)
        {
            std::cout << "    ";
        }

        std::cout << "-> MbsViewPoint : " << view_list[i] << std::endl;
    }

    PrintChildList(level);
}

}
#endif
