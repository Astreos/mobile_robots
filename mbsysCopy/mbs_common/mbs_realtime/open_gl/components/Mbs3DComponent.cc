#ifdef OPEN_GL

#include "Mbs3DComponent.hh"
#include "MbsWorld3D.hh"
#include "Joint3D.hh"
#include "Base3D.hh"

#include <stdlib.h>
#include <iostream>
#include <string.h>

namespace OpenGLMbs{

/*! \brief constructor
 *
 * \param[in] world_3d 3D world
 * \param[in] name name of the component
 * \param[in] abs_mat absolute matrix position
 */
Mbs3DComponent::Mbs3DComponent(MbsWorld3D *world_3d, const char* name, glm::mat4 const& abs_mat)
{
    comp_type = COMP_TYPE_COMPONENT;

    this->world_3d = world_3d;

    this->name = strdup(name);

    this->abs_mat = abs_mat;
}

/*! \brief destructor
 */
Mbs3DComponent::~Mbs3DComponent()
{
    if (name != NULL)
    {
        free(name);
    }
}

/*! \brief release the childs, after sending them a command to release their own childs (recursively)
 */
void Mbs3DComponent::ReleaseChildList()
{
    for(unsigned int i=0; i<mbs_3d_child_list.size(); i++)
    {
        mbs_3d_child_list[i]->ReleaseChildList();

        delete mbs_3d_child_list[i];
    }
}

/*! \brief add a joint
 * 
 * \param[in] name name of the joint
 * \param[in] joint_type type of the joint
 * \param[in] joint_value value of the joint
 * \return pointer to the new joint
 */
Joint3D* Mbs3DComponent::AddJoint(const char* name, int joint_type, float joint_value)
{
    Joint3D *new_joint = new Joint3D(world_3d, name, joint_type, joint_value);

    joint_child_list.push_back(new_joint);
    mbs_3d_child_list.push_back(new_joint);

    return new_joint;
}

/*! \brief add a T1 joint
 * 
 * \param[in] name name of the joint
 * \param[in] joint_value value of the joint
 * \return pointer to the new joint
 */
Joint3D* Mbs3DComponent::AddT1(const char* name, float joint_value)
{
    return AddJoint(name, JOINT_T1, joint_value);
}

/*! \brief add a T2 joint
 * 
 * \param[in] name name of the joint
 * \param[in] joint_value value of the joint
 * \return pointer to the new joint
 */
Joint3D* Mbs3DComponent::AddT2(const char* name, float joint_value)
{
    return AddJoint(name, JOINT_T2, joint_value);
}

/*! \brief add a T3 joint
 * 
 * \param[in] name name of the joint
 * \param[in] joint_value value of the joint
 * \return pointer to the new joint
 */
Joint3D* Mbs3DComponent::AddT3(const char* name, float joint_value)
{
    return AddJoint(name, JOINT_T3, joint_value);
}

/*! \brief add a R1 joint
 * 
 * \param[in] name name of the joint
 * \param[in] joint_value value of the joint
 * \return pointer to the new joint
 */
Joint3D* Mbs3DComponent::AddR1(const char* name, float joint_value)
{
    return AddJoint(name, JOINT_R1, joint_value);
}

/*! \brief add a R2 joint
 * 
 * \param[in] name name of the joint
 * \param[in] joint_value value of the joint
 * \return pointer to the new joint
 */
Joint3D* Mbs3DComponent::AddR2(const char* name, float joint_value)
{
    return AddJoint(name, JOINT_R2, joint_value);
}

/*! \brief add a R3 joint
 * 
 * \param[in] name name of the joint
 * \param[in] joint_value value of the joint
 * \return pointer to the new joint
 */
Joint3D* Mbs3DComponent::AddR3(const char* name, float joint_value)
{
    return AddJoint(name, JOINT_R3, joint_value);
}

/*! \brief add a joint corresponding to the base
 * 
 * \return pointer to the base (joint)
 */
Base3D* Mbs3DComponent::AddBaseJoint()
{
    Base3D *new_base = new Base3D(world_3d);

    joint_child_list.push_back(new_base);
    mbs_3d_child_list.push_back(new_base);

    return new_base;
}

/*! \brief check the name of this component
 * 
 * \param[in] name name to check
 * \return true if correct name, false otherwise
 */
bool Mbs3DComponent::IsName(const char* name)
{
    return strcmp(this->name, name) ? false : true;
}

/*! \brief get a component based on its name
 * 
 * \param[in] comp_type type of the requested component
 * \param[in] name name of the requested component
 * \return pointer to the requested component
 */
Mbs3DComponent* Mbs3DComponent::Get3DCompName(int comp_type, const char* name)
{
    Mbs3DComponent *cur_mbs_3d;

    // in case the element itself is the one requested
    if (this->comp_type == comp_type && IsName(name))
    {
        return this;
    }

    // loop on all the possible children
    for(unsigned int i=0; i<mbs_3d_child_list.size(); i++)
    {
        cur_mbs_3d = mbs_3d_child_list[i]->Get3DCompName(comp_type, name);

        if (cur_mbs_3d != NULL)
        {
            return cur_mbs_3d;
        }
    }

    // not found
    return NULL;
}

/*! \brief update the absolute matrix for the whole tree starting at this component (recursively)
 */
void Mbs3DComponent::UpdateAbsMatTree()
{
    for(unsigned int i=0; i<joint_child_list.size(); i++)
    {
        joint_child_list[i]->AbsMatCompute(abs_mat);
    }

    for(unsigned int i=0; i<mbs_3d_child_list.size(); i++)
    {
        mbs_3d_child_list[i]->UpdateAbsMatTree();
    }
}

/*! \brief print the name of this compoenent
 * 
 * \param[in] level recursion level of the current component
 */
void Mbs3DComponent::PrintName(int level)
{
    for(int i=0; i<level; i++)
    {
        std::cout << "    ";
    }

    std::cout << "- ";
    
    switch (comp_type)
    {
        case COMP_TYPE_COMPONENT : std::cout << "component"; break;
        case COMP_TYPE_BODY      : std::cout << "body";      break;
        case COMP_TYPE_ABSTRACT  : std::cout << "abstract";  break;
        case COMP_TYPE_ANCHOR    : std::cout << "anchor";    break;
        case COMP_TYPE_JOINT     : std::cout << "joint";     break;
        case COMP_TYPE_BASE      : std::cout << "base";      break;
    
        default:
            std::cout << "Mbs3DComponent error: unknwon comp_type (" << comp_type << ") !" << std::endl;
            exit(EXIT_FAILURE);
    }

    std::cout << " : \"" << name << "\"" << std::endl;
}

/*! \brief print the tree of the child
 * 
 * \param[in] level recursion level of the current component
 */
void Mbs3DComponent::PrintChildList(int level)
{
    for(unsigned int i=0; i<mbs_3d_child_list.size(); i++)
    {
        mbs_3d_child_list[i]->PrintTree(level + 1);
    }
}

/*! \brief print the whole tree, starting at this component
 * 
 * \param[in] level recursion level of the current component
 */
void Mbs3DComponent::PrintTree(int level)
{
    PrintName(level);

    PrintChildList(level);
}

}
#endif
