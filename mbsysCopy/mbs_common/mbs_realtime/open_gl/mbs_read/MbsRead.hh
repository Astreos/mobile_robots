/*! 
 * \author Nicolas Van der Noot
 * \file MbsRead.hh
 * \brief MbsRead class
 */
#ifdef OPEN_GL

#ifndef _MBS_READ_HH_
#define _MBS_READ_HH_

#include <vector>
#include <libxml/tree.h>
#include <string>

namespace OpenGLMbs{

// forward declaration
class Mbs3DComponent;
class Base3D;
class AnchorPoint3D;
class Joint3D;
class Body3D;

class MbsShape3D;

class MbsViewPoint;

class MbsPointLight;
class MbsSpotLight;
class MbsDirLight;

class MbsWorld3D;

/*! \brief read a .mbs file and extract the visu information
 */
class MbsRead
{
    public:
        MbsRead(MbsWorld3D *world_3d);
        ~MbsRead();

        Base3D* AddSingleMbs(const char* mbs_filename);

    private:
        MbsWorld3D *world_3d; ///< world 3D

        // function prototypes
        xmlNodePtr FirstNode(xmlNodePtr node, const char* node_name);
        xmlNodePtr FirstNodeLevel2(xmlNodePtr node, const char* name_1, const char* name_2);
        xmlNodePtr FirstNodeLevel3(xmlNodePtr node, const char* name_1, const char* name_2, const char* name_3);

        void ListNodes(xmlNodePtr parent_node, const char* node_name, std::vector<xmlNodePtr> &node_list);

        float FloatNode(xmlNodePtr node, xmlDocPtr doc);
        float FloatAttr(xmlAttr* attr, xmlDocPtr doc);
        float FloatChild(xmlNodePtr parent_node, xmlDocPtr doc, const char *child_name);
        void XYZChild(xmlNodePtr parent_node, xmlDocPtr doc, float &x, float &y, float &z);
        void AttChild(xmlNodePtr parent_node, xmlDocPtr doc, float &x, float &y, float &z);
        void RGBAttr(xmlNodePtr node, xmlDocPtr doc, float &R_col, float &G_col, float &B_col);

        AnchorPoint3D* PointExtract(xmlNodePtr point_node, xmlDocPtr doc, Body3D *cur_body);
        Joint3D* JointExtract(xmlNodePtr joint_node, xmlDocPtr doc, Mbs3DComponent *parent, std::vector<Joint3D*> &joint_list);
        MbsShape3D* ShapeExtract(xmlNodePtr shape_node, xmlDocPtr doc, Body3D *cur_body, std::string prj_path);
        MbsViewPoint* ViewPointExtract(xmlNodePtr view_node, xmlDocPtr doc, Base3D *base);
        MbsPointLight* PointLightExtract(xmlNodePtr light_node, xmlDocPtr doc, Base3D *base);
        MbsSpotLight* SpotLightExtract(xmlNodePtr light_node, xmlDocPtr doc, Base3D *base);
        MbsDirLight* DirLightExtract(xmlNodePtr light_node, xmlDocPtr doc, Base3D *base);

        Mbs3DComponent* ParentLocate(xmlNodePtr node, xmlDocPtr doc, Base3D *base);
};

}
#endif
#endif
