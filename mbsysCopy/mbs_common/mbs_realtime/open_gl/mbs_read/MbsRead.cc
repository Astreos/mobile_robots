#ifdef OPEN_GL

#include "MbsRead.hh"

#include "MbsWorld3D.hh"

#include "Mbs3DComponent.hh"
#include "Base3D.hh"
#include "AnchorPoint3D.hh"
#include "Joint3D.hh"
#include "Body3D.hh"

#include "MbsShape3D.hh"
#include "MbsBox.hh"
#include "MbsCylinder.hh"
#include "MbsCone.hh"
#include "MbsSphere.hh"
#include "MbsInline.hh"

#include "MbsViewPoint.hh"
#include "MbsViewPointMouse.hh"
#include "MbsViewPointFPS.hh"

#include "MbsPointLight.hh"
#include "MbsSpotLight.hh"
#include "MbsDirLight.hh"

#include <iostream>
#include <libxml/parser.h>
#include <sstream>
#include <string.h>

namespace OpenGLMbs{

/*! \brief constructor
 */
MbsRead::MbsRead(MbsWorld3D *world_3d)
{
    this->world_3d = world_3d;
}

/*! \brief destructor
 */
MbsRead::~MbsRead()
{
    
}

/*! \brief get the first node with a given name
 * 
 * \param[in] node first node to check, then checking its brothers (i.e. nodes at the same level)
 * \param[in] node_name name of the node to find
 * \return XML node pointer (succeed) or NULL (not found)
 */
xmlNodePtr MbsRead::FirstNode(xmlNodePtr node, const char* node_name)
{
    while (node != NULL)
    {
        if(!strcmp((const char*)node->name, node_name))
        {
            return node;
        }

        node = node->next;
    }

    // node not found
    return NULL;
}

/*! \brief get the first node with the correct parent name and own name
 * 
 * \param[in] node first parent node to check, then checking its brothers (i.e. nodes at the same level)
 * \param[in] name_1 name of the parent of the node to find
 * \param[in] name_2 name of the node to find
 * \return XML node pointer (succeed) or NULL (not found)
 */
xmlNodePtr MbsRead::FirstNodeLevel2(xmlNodePtr node, const char* name_1, const char* name_2)
{
    node = FirstNode(node, name_1);

    return (node == NULL) ? NULL : FirstNode(node->children, name_2);
}

/*! \brief get the first node with the correct grand-parent name, parent name and own name
 * 
 * \param[in] node first grand-parent node to check, then checking its brothers (i.e. nodes at the same level)
 * \param[in] name_1 name of the grand-parent of the node to find
 * \param[in] name_2 name of the parent of the node to find
 * \param[in] name_3 name of the node to find
 * \return XML node pointer (succeed) or NULL (not found)
 */
xmlNodePtr MbsRead::FirstNodeLevel3(xmlNodePtr node, const char* name_1, const char* name_2, const char* name_3)
{
    node = FirstNodeLevel2(node, name_1, name_2);

    return (node == NULL) ? NULL : FirstNode(node->children, name_3);
}

/*! \brief fill a list with all the children with a given name
 * 
 * \param[in] parent_node parent node
 * \param[in] node_name name of the nodes to find
 * \param[out] node_list list of found nodes
 */
void MbsRead::ListNodes(xmlNodePtr parent_node, const char* node_name, std::vector<xmlNodePtr> &node_list)
{
    xmlNodePtr cur_node;

    node_list.clear();

    // not found
    if (parent_node == NULL)
    {
        return;
    }

    cur_node = FirstNode(parent_node->children, node_name);

    while (cur_node != NULL)
    {
        node_list.push_back(cur_node);

        cur_node = FirstNode(cur_node->next, node_name);
    }
}

/*! \brief get the float value associated with a node
 * 
 * \param[in] node node with the float to extract
 * \param[in] doc parsed xml document
 * \return requested float value, 0.0 if not found
 */
float MbsRead::FloatNode(xmlNodePtr node, xmlDocPtr doc)
{
    float result;
    xmlChar* elem_val = NULL;

    // safety
    if (node == NULL)
    {
        std::cout << "MbsRead warning : cannot read float from node" << std::endl;
        return 0.0f;
    }

    elem_val = xmlNodeListGetString(doc, node->xmlChildrenNode, 1);
    result = atof((const char*) elem_val);
    xmlFree(elem_val);

    return result;
}

/*! \brief get the float value associated with an attribute
 * 
 * \param[in] attr attribute with the float to extract
 * \param[in] doc parsed xml document
 * \return requested float value, 0.0 if not found
 */
float MbsRead::FloatAttr(xmlAttr* attr, xmlDocPtr doc)
{
    float result;
    xmlChar* elem_val = NULL;

    // safety
    if (attr == NULL)
    {
        std::cout << "MbsRead warning : cannot read float from attribute" << std::endl;
        return 0.0f;
    }

    elem_val = xmlNodeListGetString(doc, attr->xmlChildrenNode, 1);
    result = atof((const char*) elem_val);
    xmlFree(elem_val);

    return result;
}

/*! \brief get the float value associated with a child node
 * 
 * \param[in] parent_node parent node
 * \param[in] doc parsed xml document
 * \param[in] child_name name of the child whose float must be extracted
 * \return requested float value, 0.0 if not found
 */
float MbsRead::FloatChild(xmlNodePtr parent_node, xmlDocPtr doc, const char *child_name)
{
    float result;
    xmlNodePtr child_node;

    xmlChar* elem_val = NULL;

    child_node = FirstNode(parent_node->children, child_name);

    return FloatNode(child_node, doc);
}

/*! \brief get the x,y,z values associated with a parent node
 * 
 * \param[in] parent_node parent node (its childs being x,y,z)
 * \param[in] doc parsed xml document
 * \param[in] x value to extract corresponding to 'x'
 * \param[in] y value to extract corresponding to 'y'
 * \param[in] z value to extract corresponding to 'z'
 */
void MbsRead::XYZChild(xmlNodePtr parent_node, xmlDocPtr doc, float &x, float &y, float &z)
{
    x = FloatChild(parent_node, doc, "x");
    y = FloatChild(parent_node, doc, "y");
    z = FloatChild(parent_node, doc, "z");
}

/*! \brief get the cst, lin, quad values associated with a parent node
 * 
 * \param[in] parent_node parent node (its childs being cst, lin, quad)
 * \param[in] doc parsed xml document
 * \param[in] x value to extract corresponding to 'cst'
 * \param[in] y value to extract corresponding to 'lin'
 * \param[in] z value to extract corresponding to 'quad'
 */
void MbsRead::AttChild(xmlNodePtr parent_node, xmlDocPtr doc, float &x, float &y, float &z)
{
    x = FloatChild(parent_node, doc, "cst");
    y = FloatChild(parent_node, doc, "lin");
    z = FloatChild(parent_node, doc, "quad");
}

/*! \brief get the RGB colors associated with a parent node
 * 
 * \param[in] parent_node parent node (its childs being R,G,B)
 * \param[in] doc parsed xml document
 * \param[in] R_col color value to extract corresponding to red
 * \param[in] G_col color value to extract corresponding to green
 * \param[in] B_col color value to extract corresponding to blue
 */
void MbsRead::RGBAttr(xmlNodePtr node, xmlDocPtr doc, float &R_col, float &G_col, float &B_col)
{
    xmlAttr* cur_attr = node->properties;

    // default values
    R_col = 0.0f;
    G_col = 0.0f;
    B_col = 0.0f;

    while (cur_attr != NULL)
    {
        if(!strcmp((const char*) cur_attr->name, "R"))
        {
            R_col = FloatAttr(cur_attr, doc) / 255.0f;
        }
        else if(!strcmp((const char*) cur_attr->name, "G"))
        {
            G_col = FloatAttr(cur_attr, doc) / 255.0f;
        }
        else if(!strcmp((const char*) cur_attr->name, "B"))
        {
            B_col = FloatAttr(cur_attr, doc) / 255.0f;
        }
        cur_attr = cur_attr->next;
    }
}

/*! \brief locate the parent of a body (origin or anchor point)
 * 
 * \param[in] node XML node corresponding to body
 * \param[in] doc parsed xml document
 * \param[out] base base of the current .mbs file
 * \return requested 3D component, NULL if not found or problem
 */
Mbs3DComponent* MbsRead::ParentLocate(xmlNodePtr node, xmlDocPtr doc, Base3D *base)
{
    Mbs3DComponent *parent_comp;
    xmlNodePtr bodyname_n, pointname_n;
    xmlChar *elem_body, *elem_point;

    bodyname_n  = FirstNode(node->children, "bodyname" );
    pointname_n = FirstNode(node->children, "pointname");

    if (bodyname_n == NULL || pointname_n == NULL)
    {
        std::cout << "MbsRead warning : \"bodyname\" or \"pointname\" not found !" << std::endl;
        return NULL;
    }

    parent_comp = NULL;

    elem_body  = xmlNodeListGetString(doc, bodyname_n->xmlChildrenNode , 1);
    elem_point = xmlNodeListGetString(doc, pointname_n->xmlChildrenNode, 1);

    // find parent body
    parent_comp = base->Get3DCompName(COMP_TYPE_BODY, (const char*) elem_body);

    if (parent_comp == NULL)
    {
        std::cout << "MbsRead warning : parent body not found !" << std::endl;
        return NULL;
    }

    // not the origin, so the body is not the parent
    if(strcmp((const char*) elem_point, "origin"))
    {
        parent_comp = parent_comp->Get3DCompName(COMP_TYPE_ANCHOR, (const char*) elem_point);
    }

    xmlFree(elem_point);
    xmlFree(elem_body);

    return parent_comp;
}

/*! \brief add a single .mbs file
 * 
 * \param[in] mbs_filename name of the .mbs file (with path)
 * \return pointer to the base of this .mbs file
 */
Base3D* MbsRead::AddSingleMbs(const char* mbs_filename)
{
    // variables declaration
    float R_col, G_col, B_col;

    Base3D *base;
    Body3D *cur_body;
    Joint3D *cur_joint;
    Mbs3DComponent *cur_parent;
    xmlChar* elem_val;
    xmlDocPtr doc;
    xmlNodePtr cur_node, root, base_n, bodytree_n, x3D_n;

    std::vector<xmlNodePtr> point_n_list, body_n_list, shape_n_list;
    std::vector<xmlNodePtr> joint_n_list, view_n_list, light_n_list;

    std::vector<Joint3D*> joint_list;

    size_t pos;
    std::string prj_path, cur_name;

    // extract PRJPATH
    prj_path = mbs_filename;    
    prj_path = prj_path.substr(0, prj_path.find_last_of("\\/"));

    pos = prj_path.find_last_of("\\/");
    cur_name = prj_path.substr(pos+1, prj_path.size()-pos-1);

    if (!cur_name.compare("dataR"))
    {
        prj_path = prj_path.substr(0, prj_path.find_last_of("\\/"));
    }

    // base of the .mbs
    base = NULL;

    // ignore text nodes
    xmlKeepBlanksDefault(0);

    // xml document opening
    doc = xmlParseFile(mbs_filename);
    if (doc == NULL)
    {
        std::cout << "MbsRead warning: invalid XML document" << std::endl;
        return NULL;
    }

    // getting the root
    root = xmlDocGetRootElement(doc);
    if (root == NULL)
    {
        std::cout << "MbsRead warning: empty XML document" << std::endl;
        xmlFreeDoc(doc);
        return NULL;
    }

    // check root name
    if (strcmp((const char*)root->name, "mbsDescR7"))
    {
        std::cout << "MbsRead warning : xml is not a mbsDescR7" << std::endl;
        xmlFreeDoc(doc);
        return NULL;
    }

    // main .mbs parts
    base_n     = FirstNode(root->children, "base");
    bodytree_n = FirstNode(root->children, "bodytree");
    x3D_n      = FirstNode(root->children, "x3D");

    // check main parts
    if (base_n == NULL || bodytree_n == NULL || x3D_n == NULL)
    {
        std::cout << "MbsRead warning : main xml parts not found !" << std::endl;
        xmlFreeDoc(doc);
        return NULL;
    }

    // base
    base = new Base3D(world_3d);
    cur_body = base->AddBody("base");

    // anchor points of the base body
    ListNodes(base_n, "point", point_n_list);

    for(unsigned int i=0; i<point_n_list.size(); i++)
    {
        PointExtract(point_n_list[i], doc, cur_body);
    }

    // 3D shapes of the base body
    cur_node = FirstNodeLevel3(base_n->children, "graphics", "x3D", "x3D_shapes");

    if (cur_node != NULL)
    {
        cur_node = cur_node->children;

        while (cur_node != NULL)
        {
            ShapeExtract(cur_node, doc, cur_body, prj_path);

            cur_node = cur_node->next;
        }
    }

    // get all the bodies
    ListNodes(bodytree_n, "body", body_n_list);

    // list on all the bodies
    for(unsigned int i=0; i<body_n_list.size(); i++)
    {
        // get parent
        cur_node = FirstNode(body_n_list[i]->children, "parent");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : parent field not found !" << std::endl;
            continue;
        }

        cur_parent = ParentLocate(cur_node, doc, base);

        if (cur_parent == NULL)
        {
            std::cout << "MbsRead warning : current parent not found !" << std::endl;
            continue;
        }

        // get all the joints
        ListNodes(body_n_list[i], "joint", joint_n_list);

        for(unsigned int j=0; j<joint_n_list.size(); j++)
        {
            cur_joint  = JointExtract(joint_n_list[j], doc, cur_parent, joint_list);
            cur_parent = cur_joint;
        }

        if (cur_joint == NULL)
        {
            std::cout << "MbsRead warning : joint parent not found !" << std::endl;
            continue;
        }

        // add body
        cur_node = FirstNode(body_n_list[i]->children, "bodyname");

        elem_val = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
        cur_body = cur_joint->AddBody((const char*) elem_val);
        xmlFree(elem_val);

        // anchor points of the current body
        ListNodes(body_n_list[i], "point", point_n_list);

        for(unsigned int j=0; j<point_n_list.size(); j++)
        {
            PointExtract(point_n_list[j], doc, cur_body);
        }

        // 3D shapes of the current body
        cur_node = FirstNodeLevel3(body_n_list[i]->children, "graphics", "x3D", "x3D_shapes");

        if (cur_node != NULL)
        {
            cur_node = cur_node->children;

            while (cur_node != NULL)
            {
                ShapeExtract(cur_node, doc, cur_body, prj_path);

                cur_node = cur_node->next;
            }
        }
    }

    // add current .mbs joint list
    world_3d->AddJointList(joint_list);

    // background color
    cur_node = FirstNodeLevel2(x3D_n->children, "background", "color");

    if (cur_node != NULL)
    {
        RGBAttr(cur_node, doc, R_col, G_col, B_col);
        
        world_3d->SetBackgroundColor(glm::vec3(R_col, G_col, B_col));
    }
    else
    {
        std::cout << "MbsRead warning : no color background found !" << std::endl;
    }

    // viewpoints
    cur_node = FirstNode(x3D_n->children, "viewpoints");

    ListNodes(cur_node, "viewpoint", view_n_list);

    for(unsigned int i=0; i<view_n_list.size(); i++)
    {
        ViewPointExtract(view_n_list[i], doc, base);
    }

    // no viewpoint added: add a default one
    if (world_3d->GetNbViews() <= 0)
    {
        world_3d->AddViewPointMouse(glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0, 0.0, 1.0));
    }

    // lights
    cur_node = FirstNode(x3D_n->children, "lights");

    // point light
    ListNodes(cur_node, "pointLight", light_n_list);

    for(unsigned int i=0; i<light_n_list.size(); i++)
    {
        PointLightExtract(light_n_list[i], doc, base);
    }

    // spotLight
    ListNodes(cur_node, "spotLight", light_n_list);

    for(unsigned int i=0; i<light_n_list.size(); i++)
    {
        SpotLightExtract(light_n_list[i], doc, base);
    }

    // directionalLight
    ListNodes(cur_node, "directionalLight", light_n_list);

    for(unsigned int i=0; i<light_n_list.size(); i++)
    {
        DirLightExtract(light_n_list[i], doc, base);
    }

    // free the memory allocate to the xml parser
    xmlFreeDoc(doc);

    return base;
}

/*! \brief extract an anchor point
 * 
 * \param[in] point_node XML node corresponding to the anchor point
 * \param[in] doc parsed xml document
 * \param[out] cur_body current body, being the parent of the anchor point
 * \return requested anchor point, NULL if not found or problem
 */
AnchorPoint3D* MbsRead::PointExtract(xmlNodePtr point_node, xmlDocPtr doc, Body3D *cur_body)
{
    float x, y, z;
    xmlNodePtr cur_node;
    xmlChar *elem_val;
    AnchorPoint3D *new_point;

    elem_val  = NULL;
    new_point = NULL;

    cur_node = FirstNode(point_node->children, "coordinates");

    // safety
    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : cannot read \"coordinates\"" << std::endl;
        return NULL;
    }

    XYZChild(cur_node, doc, x, y, z);

    cur_node = FirstNode(point_node->children, "pointname");

    // safety
    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : cannot read \"pointname\"" << std::endl;
        return NULL;
    }

    elem_val = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
    new_point = cur_body->AddAnchor((const char*) elem_val, glm::vec3(x, y, z));
    xmlFree(elem_val);

    return new_point;
}

/*! \brief extract a joint
 * 
 * \param[in] joint_node XML node corresponding to the joint
 * \param[in] doc parsed xml document
 * \param[out] parent parent of the joint
 * \param[out] joint_list list of joints to increment
 * \return requested joint, NULL if not found or problem
 */
Joint3D* MbsRead::JointExtract(xmlNodePtr joint_node, xmlDocPtr doc,
    Mbs3DComponent *parent, std::vector<Joint3D*> &joint_list)
{
    xmlNodePtr name_n, type_n, init_n;
    xmlChar *elem_name, *elem_type;
    Joint3D *new_joint;
    float joint_val;

    new_joint = NULL;
    joint_val = 0.0f;

    // get nodes
    name_n = FirstNode(joint_node->children, "jointname");
    type_n = FirstNode(joint_node->children, "type");
    init_n = FirstNodeLevel2(joint_node->children, "initialvalue", "q");

    if (name_n == NULL || type_n == NULL)
    {
        std::cout << "MbsRead warning : cannot read joint name or type !" << std::endl;
        return NULL;
    }

    elem_name = xmlNodeListGetString(doc, name_n->xmlChildrenNode, 1);
    elem_type = xmlNodeListGetString(doc, type_n->xmlChildrenNode, 1);

    if (init_n != NULL)
    {
        joint_val = FloatNode(init_n, doc);
    }
    
    if(!strcmp((const char*) elem_type, "T1"))
    {
        new_joint = parent->AddT1((const char*) elem_name, joint_val);
    }
    else if(!strcmp((const char*) elem_type, "T2"))
    {
        new_joint = parent->AddT2((const char*) elem_name, joint_val);
    }
    else if(!strcmp((const char*) elem_type, "T3"))
    {
        new_joint = parent->AddT3((const char*) elem_name, joint_val);
    }
    else if(!strcmp((const char*) elem_type, "R1"))
    {
        new_joint = parent->AddR1((const char*) elem_name, joint_val);
    }
    else if(!strcmp((const char*) elem_type, "R2"))
    {
        new_joint = parent->AddR2((const char*) elem_name, joint_val);
    }
    else if(!strcmp((const char*) elem_type, "R3"))
    {
        new_joint = parent->AddR3((const char*) elem_name, joint_val);
    }
    else
    {
        std::cout << "MbsRead warning : joint type (" << (const char*) elem_type << ") unknown !" << std::endl;
        return NULL;
    }

    xmlFree(elem_type);
    xmlFree(elem_name);

    // add to joint list
    joint_list.push_back(new_joint);

    return new_joint;
}

/*! \brief extract a viewpoint
 * 
 * \param[in] view_node XML node corresponding to viewpoint
 * \param[in] doc parsed xml document
 * \param[out] base base of the current .mbs file
 * \return requested viewpoint, NULL if not found or problem
 */
MbsViewPoint* MbsRead::ViewPointExtract(xmlNodePtr view_node, xmlDocPtr doc, Base3D *base)
{
    float pos_x, pos_y, pos_z;
    float tar_x, tar_y, tar_z;
    float up_x, up_y, up_z;
    float scale, FoV_rad, near, far;

    MbsViewPoint *new_view;
    xmlNodePtr joint_n, proj_n, cur_node;
    xmlChar *elem_joint, *elem_proj;
    Mbs3DComponent *joint;

    new_view = NULL;

    // get nodes
    joint_n = FirstNode(view_node->children, "joint");
    proj_n  = FirstNode(view_node->children, "projection");

    if (joint_n == NULL || proj_n == NULL)
    {
        std::cout << "MbsRead warning : cannot read viewpoint parent or projection !" << std::endl;
        return NULL;
    }

    // get parent joint
    elem_joint = xmlNodeListGetString(doc, joint_n->xmlChildrenNode, 1);

    if(!strcmp((const char*) elem_joint, "base"))
    {
        joint = base;
    }
    else
    {
        joint = base->Get3DCompName(COMP_TYPE_JOINT, (const char*) elem_joint);
    }
    xmlFree(elem_joint);

    if (joint == NULL)
    {
        std::cout << "MbsRead warning : cannot get viewpoint joint" << std::endl;
        return NULL;
    }

    // projection type
    elem_proj  = xmlNodeListGetString(doc, proj_n->xmlChildrenNode, 1);

    if( !strcmp((const char*) elem_proj, "perspective") ||
        !strcmp((const char*) elem_proj, "parallel") )
    {
        // position
        cur_node = FirstNode(view_node->children, "position");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : view position not found !" << std::endl;
            return NULL;
        }
        XYZChild(cur_node, doc, pos_x, pos_y, pos_z);

        // target
        cur_node = FirstNode(view_node->children, "target");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : view target not found !" << std::endl;
            return NULL;
        }
        XYZChild(cur_node, doc, tar_x, tar_y, tar_z);

        // up
        cur_node = FirstNode(view_node->children, "up");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : view up not found !" << std::endl;
            return NULL;
        }
        XYZChild(cur_node, doc, up_x, up_y, up_z);

        // frontClipDst
        cur_node = FirstNode(view_node->children, "frontClipDst");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : view frontClipDst not found !" << std::endl;
            return NULL;
        }
        near = FloatNode(cur_node, doc);

        // backClipDst
        cur_node = FirstNode(view_node->children, "backClipDst");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : view backClipDst not found !" << std::endl;
            return NULL;
        }
        far = FloatNode(cur_node, doc);
    }

    if(!strcmp((const char*) elem_proj, "perspective"))
    {
        xmlFree(elem_proj);

        // fieldOfView
        cur_node = FirstNode(view_node->children, "fieldOfView");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : view fieldOfView not found !" << std::endl;
            return NULL;
        }
        FoV_rad = FloatNode(cur_node, doc);

        new_view = world_3d->AddViewPointMouse(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(tar_x, tar_y, tar_z),
            glm::vec3(up_x, up_y, up_z), true, FoV_rad, near, far, joint->GetAbsMat());
    }
    else if(!strcmp((const char*) elem_proj, "parallel"))
    {
        xmlFree(elem_proj);

        // scale
        cur_node = FirstNode(view_node->children, "scale");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : view scale not found !" << std::endl;
            return NULL;
        }
        scale = FloatNode(cur_node, doc);

        new_view = world_3d->AddViewPointMouse(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(tar_x, tar_y, tar_z),
            glm::vec3(up_x, up_y, up_z), false, scale, near, far, joint->GetAbsMat());
    }
    else
    {
        std::cout << "MbsRead warning : viewpoint type (" << (const char*) elem_proj << ") unknown !" << std::endl;
        xmlFree(elem_proj);
        return NULL;
    }

    static_cast<Joint3D*>(joint)->AddViewPoint(new_view);

    return new_view;
}

/*! \brief extract a point light
 * 
 * \param[in] light_node XML node corresponding to light
 * \param[in] doc parsed xml document
 * \param[out] base base of the current .mbs file
 * \return requested light, NULL if not found or problem
 */
MbsPointLight* MbsRead::PointLightExtract(xmlNodePtr light_node, xmlDocPtr doc, Base3D *base)
{
    bool active;
    float ambient;
    float pos_x, pos_y, pos_z;
    float att_x, att_y, att_z;

    MbsPointLight *new_light;

    xmlNodePtr joint_n, cur_node;
    xmlChar *elem_joint;
    Mbs3DComponent *joint;

    glm::vec3 color;

    color   = glm::vec3(1.0f);
    ambient = 0.0f;

    new_light = NULL;
    active = true;

    // get joint node
    joint_n = FirstNode(light_node->children, "joint");

    if (joint_n == NULL)
    {
        std::cout << "MbsRead warning : cannot read light parent !" << std::endl;
        return NULL;
    }

    // get parent joint
    elem_joint = xmlNodeListGetString(doc, joint_n->xmlChildrenNode, 1);

    if(!strcmp((const char*) elem_joint, "base"))
    {
        joint = base;
    }
    else
    {
        joint = base->Get3DCompName(COMP_TYPE_JOINT, (const char*) elem_joint);
    }
    xmlFree(elem_joint);

    if (joint == NULL)
    {
        std::cout << "MbsRead warning : cannot get light joint" << std::endl;
        return NULL;
    }

    // position
    cur_node = FirstNode(light_node->children, "position");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : light position not found !" << std::endl;
        return NULL;
    }

    XYZChild(cur_node, doc, pos_x, pos_y, pos_z);

    // attenuation
    cur_node = FirstNode(light_node->children, "attenuation");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : light attenuation not found !" << std::endl;
        return NULL;
    }

    AttChild(cur_node, doc, att_x, att_y, att_z);

    // active
    cur_node = FirstNode(light_node->children, "active");

    if (cur_node == NULL)
    {
        active = false;
    }

    // new light
    new_light = world_3d->AddPointLight(glm::vec3(pos_x, pos_y, pos_z),
        glm::vec3(att_x, att_y, att_z),
        joint->GetAbsMat(), color, ambient,
        active);

    static_cast<Joint3D*>(joint)->AddLight(new_light);

    return new_light;
}

/*! \brief extract a spot light
 * 
 * \param[in] light_node XML node corresponding to light
 * \param[in] doc parsed xml document
 * \param[out] base base of the current .mbs file
 * \return requested light, NULL if not found or problem
 */
MbsSpotLight* MbsRead::SpotLightExtract(xmlNodePtr light_node, xmlDocPtr doc, Base3D *base)
{
    bool active;
    float ambient;
    float pos_x, pos_y, pos_z;
    float dir_x, dir_y, dir_z;
    float att_x, att_y, att_z;
    float angle, concentration;

    MbsSpotLight *new_light;

    xmlNodePtr joint_n, cur_node;
    xmlChar *elem_joint;
    Mbs3DComponent *joint;

    glm::vec3 color;

    color   = glm::vec3(1.0f);
    ambient = 0.0f;

    new_light = NULL;
    active = true;

    // get joint node
    joint_n = FirstNode(light_node->children, "joint");

    if (joint_n == NULL)
    {
        std::cout << "MbsRead warning : cannot read light parent !" << std::endl;
        return NULL;
    }

    // get parent joint
    elem_joint = xmlNodeListGetString(doc, joint_n->xmlChildrenNode, 1);

    if(!strcmp((const char*) elem_joint, "base"))
    {
        joint = base;
    }
    else
    {
        joint = base->Get3DCompName(COMP_TYPE_JOINT, (const char*) elem_joint);
    }
    xmlFree(elem_joint);

    if (joint == NULL)
    {
        std::cout << "MbsRead warning : cannot get light joint" << std::endl;
        return NULL;
    }

    // position
    cur_node = FirstNode(light_node->children, "position");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : light position not found !" << std::endl;
        return NULL;
    }

    XYZChild(cur_node, doc, pos_x, pos_y, pos_z);

    // direction
    cur_node = FirstNode(light_node->children, "direction");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : light direction not found !" << std::endl;
        return NULL;
    }

    XYZChild(cur_node, doc, dir_x, dir_y, dir_z);

    // attenuation
    cur_node = FirstNode(light_node->children, "attenuation");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : light attenuation not found !" << std::endl;
        return NULL;
    }

    AttChild(cur_node, doc, att_x, att_y, att_z);

    // angle
    cur_node = FirstNode(light_node->children, "angle");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : light angle not found !" << std::endl;
        return NULL;
    }

    angle = FloatNode(cur_node, doc);

    // concentration
    cur_node = FirstNode(light_node->children, "concentration");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : light concentration not found !" << std::endl;
        return NULL;
    }

    concentration = FloatNode(cur_node, doc);

    // active
    cur_node = FirstNode(light_node->children, "active");

    if (cur_node == NULL)
    {
        active = false;
    }

    // new light
    new_light = world_3d->AddSpotLight(glm::vec3(pos_x, pos_y, pos_z), glm::vec3(att_x, att_y, att_z), glm::vec3(dir_x, dir_y, dir_z),
        joint->GetAbsMat(), color, ambient, angle, concentration, active);

    static_cast<Joint3D*>(joint)->AddLight(new_light);

    return new_light;
}

/*! \brief extract a directional light
 * 
 * \param[in] light_node XML node corresponding to light
 * \param[in] doc parsed xml document
 * \param[out] base base of the current .mbs file
 * \return requested light, NULL if not found or problem
 */
MbsDirLight* MbsRead::DirLightExtract(xmlNodePtr light_node, xmlDocPtr doc, Base3D *base)
{
    bool active;
    float ambient;
    float dir_x, dir_y, dir_z;

    MbsDirLight *new_light;

    xmlNodePtr joint_n, cur_node;
    xmlChar *elem_joint;
    Mbs3DComponent *joint;

    glm::vec3 color;

    color   = glm::vec3(1.0f);
    ambient = 0.0f;

    new_light = NULL;
    active = true;

    // get joint node
    joint_n = FirstNode(light_node->children, "joint");

    if (joint_n == NULL)
    {
        std::cout << "MbsRead warning : cannot read light parent !" << std::endl;
        return NULL;
    }

    // get parent joint
    elem_joint = xmlNodeListGetString(doc, joint_n->xmlChildrenNode, 1);

    if(!strcmp((const char*) elem_joint, "base"))
    {
        joint = base;
    }
    else
    {
        joint = base->Get3DCompName(COMP_TYPE_JOINT, (const char*) elem_joint);
    }
    xmlFree(elem_joint);

    if (joint == NULL)
    {
        std::cout << "MbsRead warning : cannot get light joint" << std::endl;
        return NULL;
    }

    // direction
    cur_node = FirstNode(light_node->children, "direction");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : light direction not found !" << std::endl;
        return NULL;
    }

    XYZChild(cur_node, doc, dir_x, dir_y, dir_z);

    // active
    cur_node = FirstNode(light_node->children, "active");

    if (cur_node == NULL)
    {
        active = false;
    }

    // new light
    new_light = world_3d->AddDirLight(glm::vec3(dir_x, dir_y, dir_z), joint->GetAbsMat(), color, ambient, active);

    static_cast<Joint3D*>(joint)->AddLight(new_light);

    return new_light;
}

/*! \brief extract a 3D shape
 * 
 * \param[in] shape_node XML node corresponding to 3D shape
 * \param[in] doc parsed xml document
 * \param[out] cur_body current body, being the parent of the 3D shape
 * \param[in] prj_path project path
 * \return requested 3D shape, NULL if not found or problem
 */
MbsShape3D* MbsRead::ShapeExtract(xmlNodePtr shape_node, xmlDocPtr doc,
    Body3D *cur_body, std::string prj_path)
{
    float pos_x, pos_y, pos_z;
    float sc_x, sc_y, sc_z;
    float rot_x, rot_y, rot_z;
    float R_col, G_col, B_col;
    float transp;

    float size_x, size_y, size_z;
    float radius, height;

    size_t start_pos;

    std::stringstream ss_name, ss_path;
    std::string name_str, elem_str, prj_path_name;

    xmlNodePtr cur_node;

    MbsShape3D *new_shape_3d;

    xmlChar* elem_val = NULL;

    new_shape_3d = NULL;

    // name
    cur_node = FirstNode(shape_node->children, "name");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : cannot read \"name\"" << std::endl;
        return NULL;
    }

    elem_val = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);

    ss_name << (const char*) elem_val;
    ss_name >> name_str;

    // position
    cur_node = FirstNode(shape_node->children, "position");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : cannot read \"position\"" << std::endl;
        return NULL;
    }

    XYZChild(cur_node, doc, pos_x, pos_y, pos_z);

    // scale
    cur_node = FirstNode(shape_node->children, "scale");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : cannot read \"scale\"" << std::endl;
        return NULL;
    }

    XYZChild(cur_node, doc, sc_x, sc_y, sc_z);

    // rotation
    cur_node = FirstNode(shape_node->children, "rotation");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : cannot read \"rotation\"" << std::endl;
        return NULL;
    }

    XYZChild(cur_node, doc, rot_x, rot_y, rot_z);

    // transparency
    cur_node = FirstNode(shape_node->children, "transparency");

    if (cur_node == NULL)
    {
        std::cout << "MbsRead warning : cannot read \"transparency\"" << std::endl;
        return NULL;
    }

    transp = 1.0 - FloatNode(cur_node, doc);

    // color
    if( !strcmp((const char*)shape_node->name, "box") ||
        !strcmp((const char*)shape_node->name, "cylinder") ||
        !strcmp((const char*)shape_node->name, "cone") ||
        !strcmp((const char*)shape_node->name, "sphere") )
    {
        cur_node = FirstNode(shape_node->children, "color");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : cannot read \"color\"" << std::endl;
            return NULL;
        }

        RGBAttr(cur_node, doc, R_col, G_col, B_col);
    }

    // select correct shape
    if(!strcmp((const char*)shape_node->name, "box"))
    {
        // size
        cur_node = FirstNode(shape_node->children, "size");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : cannot read \"size\"" << std::endl;
            return NULL;
        }

        XYZChild(cur_node, doc, size_x, size_y, size_z);

        // add shape
        new_shape_3d = world_3d->AddBox(size_x, size_y, size_z, glm::vec3(R_col, G_col, B_col), transp,
            glm::vec3(pos_x, pos_y, pos_z), glm::vec3(sc_x, sc_y, sc_z), glm::vec3(rot_x, rot_y, rot_z),
            name_str.c_str());

        cur_body->AddMbsShape3D(new_shape_3d);
    }
    else if(!strcmp((const char*)shape_node->name, "cylinder"))
    {
        // radius
        cur_node = FirstNode(shape_node->children, "radius");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : cannot read \"radius\"" << std::endl;
            return NULL;
        }

        radius = FloatNode(cur_node, doc);

        // height
        cur_node = FirstNode(shape_node->children, "height");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : cannot read \"height\"" << std::endl;
            return NULL;
        }

        height = FloatNode(cur_node, doc);

        // add shape
        new_shape_3d = world_3d->AddCylinder(radius, height, glm::vec3(R_col, G_col, B_col), transp,
            glm::vec3(pos_x, pos_y, pos_z), glm::vec3(sc_x, sc_y, sc_z), glm::vec3(rot_x, rot_y, rot_z),
            name_str.c_str());

        cur_body->AddMbsShape3D(new_shape_3d);
    }
    else if(!strcmp((const char*)shape_node->name, "cone"))
    {
        // radius
        cur_node = FirstNode(shape_node->children, "radius");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : cannot read \"radius\"" << std::endl;
            return NULL;
        }

        radius = FloatNode(cur_node, doc);

        // height
        cur_node = FirstNode(shape_node->children, "height");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : cannot read \"height\"" << std::endl;
            return NULL;
        }

        height = FloatNode(cur_node, doc);

        // add shape
        new_shape_3d = world_3d->AddCone(radius, height, glm::vec3(R_col, G_col, B_col), transp,
            glm::vec3(pos_x, pos_y, pos_z), glm::vec3(sc_x, sc_y, sc_z), glm::vec3(rot_x, rot_y, rot_z),
            name_str.c_str());

        cur_body->AddMbsShape3D(new_shape_3d);
    }
    else if(!strcmp((const char*)shape_node->name, "sphere"))
    {
        // radius
        cur_node = FirstNode(shape_node->children, "radius");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : cannot read \"radius\"" << std::endl;
            return NULL;
        }

        radius = FloatNode(cur_node, doc);

        // add shape
        new_shape_3d = world_3d->AddSphere(radius, glm::vec3(R_col, G_col, B_col), transp,
            glm::vec3(pos_x, pos_y, pos_z), glm::vec3(sc_x, sc_y, sc_z), glm::vec3(rot_x, rot_y, rot_z),
            name_str.c_str());

        cur_body->AddMbsShape3D(new_shape_3d);
    }
    else if(!strcmp((const char*)shape_node->name, "inline"))
    {
        // url
        cur_node = FirstNode(shape_node->children, "url");

        if (cur_node == NULL)
        {
            std::cout << "MbsRead warning : cannot read \"url\"" << std::endl;
            return NULL;
        }

        elem_val = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);

        ss_path << (const char*) elem_val;
        ss_path >> elem_str;

        prj_path_name = "PRJPATH";

        start_pos = elem_str.find(prj_path_name);

        if(start_pos != std::string::npos)
        {
            elem_str.replace(start_pos, prj_path_name.length(), prj_path + "/");
        }

        new_shape_3d = world_3d->AddInline((const char*) elem_str.c_str(), transp,
            glm::vec3(pos_x, pos_y, pos_z), glm::vec3(sc_x, sc_y, sc_z), glm::vec3(rot_x, rot_y, rot_z),
            name_str.c_str());

        cur_body->AddMbsShape3D(new_shape_3d);

        xmlFree(elem_val);
    }
    else
    {
        std::cout << "MbsRead warning : unknown shape (" << shape_node->name << ") !" << std::endl;
        return NULL;
    }

    return new_shape_3d;
}

}
#endif
