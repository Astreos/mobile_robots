
#include "mbs_xml_reader.h"
#include <libxml/tree.h>
#include <libxml/parser.h>

#include "useful_functions.h"


void MDS_coordinates_reader(double *coordinates_pointer, xmlNodePtr node, xmlDocPtr doc)
{
    xmlNodePtr cur_node = node;
    xmlChar* elementValue = NULL;

    if(!strcmp((const char*)node->name,"coordinates"))
    {
        cur_node = cur_node->children;
        while (cur_node != NULL)
        {
            if (cur_node->type == XML_ELEMENT_NODE)
            {
                if(!strcmp((const char*)cur_node->name,"x"))
                {
                    elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                    coordinates_pointer[0] = atof((char*)elementValue);
                    xmlFree(elementValue);
                }
                if(!strcmp((const char*)cur_node->name,"y"))
                {
                    elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                    coordinates_pointer[1] = atof((char*)elementValue);
                    xmlFree(elementValue);
                }
                if(!strcmp((const char*)cur_node->name,"z"))
                {
                    elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                    coordinates_pointer[2] = atof((char*)elementValue);
                    xmlFree(elementValue);
                }
            }
            cur_node = cur_node->next;
        }
    }
}

MDS_sensor_strct* MDS_sensor_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_sensor_strct* mds_sensor_strct;

    xmlAttr* cur_node = node->properties;
    xmlChar* elementValue = NULL;

    while (cur_node != NULL)
    {
        if (1)//cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"name"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_sensor_strct = init_MDS_sensor_strct((char*)elementValue);
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Pos"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_sensor_strct->Pos = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Rot"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_sensor_strct->Rot = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Vit"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_sensor_strct->Vit = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Om"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_sensor_strct->Om = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Acc"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_sensor_strct->Acc = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Omp"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_sensor_strct->Omp = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Jac"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_sensor_strct->Jac = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
        }
        cur_node = cur_node->next;
    }
    return mds_sensor_strct;

}

MDS_extforce_strct* MDS_extforce_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_extforce_strct* mds_extforce_strct;

    xmlAttr* cur_node = node->properties;
    xmlChar* elementValue = NULL;

    while (cur_node != NULL)
    {
        if (1)//cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"name"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_extforce_strct = init_MDS_extforce_strct((char*)elementValue);
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Pos"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_extforce_strct->Pos = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Rot"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_extforce_strct->Rot = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Vit"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_extforce_strct->Vit = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Om"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_extforce_strct->Om = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Acc"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_extforce_strct->Acc = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"Omp"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_extforce_strct->Omp = !strcmp((const char*)elementValue,"true");
                xmlFree(elementValue);
            }
        }
        cur_node = cur_node->next;
    }
    return mds_extforce_strct;

}


MDS_joint_strct* MDS_joint_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_joint_strct* mds_joint_strct;

    xmlNodePtr cur_node = node->children;
    xmlNodePtr sub_cur_node = NULL;
    xmlChar* elementValue = NULL;

    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {

            if(!strcmp((const char*)cur_node->name,"jointname"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_joint_strct = init_MDS_joint_strct((char*)elementValue);
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"type"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                if(!strcmp((char*)elementValue,"T1"))
                {
                    mds_joint_strct->type = 1;
                }
                else if(!strcmp((char*)elementValue,"T2"))
                {
                    mds_joint_strct->type = 2;
                }
                else if(!strcmp((char*)elementValue,"T3"))
                {
                    mds_joint_strct->type = 3;
                }
                else if(!strcmp((char*)elementValue,"R1"))
                {
                    mds_joint_strct->type = 4;
                }
                else if(!strcmp((char*)elementValue,"R2"))
                {
                    mds_joint_strct->type = 5;
                }
                else if(!strcmp((char*)elementValue,"R3"))
                {
                    mds_joint_strct->type = 6;
                }
                else
                {
                    mds_joint_strct->type = 0;
                }
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"nature"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                if(!strcmp((char*)elementValue,"independent"))
                {
                    mds_joint_strct->nature = 1;
                }
                else if(!strcmp((char*)elementValue,"dependent"))
                {
                    mds_joint_strct->nature = 2;
                }
                else if(!strcmp((char*)elementValue,"driven"))
                {
                    mds_joint_strct->nature = 3;
                }
                else
                {
                    mds_joint_strct->type = 0;
                }
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"initialvalue"))
            {
                sub_cur_node = cur_node->children;
                while (sub_cur_node != NULL)
                {
                    if (sub_cur_node->type == XML_ELEMENT_NODE || sub_cur_node->type == XML_COMMENT_NODE)
                    {
                        if(!strcmp((const char*)sub_cur_node->name,"q"))
                        {
                            elementValue = xmlNodeListGetString(doc, sub_cur_node->xmlChildrenNode, 1);
                            mds_joint_strct->q0 = atof((char*)elementValue);
                            xmlFree(elementValue);
                        }
                        if(!strcmp((const char*)sub_cur_node->name,"qd"))
                        {
                            elementValue = xmlNodeListGetString(doc, sub_cur_node->xmlChildrenNode, 1);
                            mds_joint_strct->qd0 = atof((char*)elementValue);
                            xmlFree(elementValue);
                        }
                        if(!strcmp((const char*)sub_cur_node->name,"qdd"))
                        {
                            elementValue = xmlNodeListGetString(doc, sub_cur_node->xmlChildrenNode, 1);
                            mds_joint_strct->qdd0 = atof((char*)elementValue);
                            xmlFree(elementValue);
                        }
                    }
                    sub_cur_node = sub_cur_node->next;
                }
            }
            if(!strcmp((const char*)cur_node->name,"actuated"))
            {
                mds_joint_strct->actuated = 1;
            }
            if(!strcmp((const char*)cur_node->name,"graphics"))
            {
                // to complet
            }
        }
        cur_node = cur_node->next;
    }
    return mds_joint_strct ;
}

MDS_joint_strct** MDS_joint_list_reader(int *n_joint, xmlNodePtr node, xmlDocPtr doc)
{
    int i = 0;
    int n = 0;
    xmlNodePtr cur_node;
    MDS_joint_strct** joint_list;

    cur_node = node;
    // counting loop
    while (cur_node != NULL)
    {
        if ((cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE) && !strcmp((const char*)cur_node->name,"joint"))
        {
            n++;
        }
        cur_node = cur_node->next;
    }

    *n_joint=n;

    joint_list = (MDS_joint_strct**) malloc(n*sizeof(MDS_joint_strct*));

    // reading loop
    i = 0;
    cur_node = node;
    while (cur_node != NULL)
    {
        if ((cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE) && !strcmp((const char*)cur_node->name,"joint"))
        {
            joint_list[i] = MDS_joint_reader(cur_node, doc);
            i++;
        }
        cur_node = cur_node->next;
    }
    return joint_list;
}

MDS_point_strct* MDS_point_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_point_strct* mds_point_strct;

    xmlNodePtr cur_node = node->children;
    xmlChar* elementValue = NULL;

    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {

            if(!strcmp((const char*)cur_node->name,"pointname"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_point_strct = init_MDS_point_strct((char*)elementValue);
                xmlFree(elementValue);
            }
            else if(!strcmp((const char*)cur_node->name,"coordinates"))
            {
                MDS_coordinates_reader(mds_point_strct->pt, cur_node, doc);
            }
            else if(!strcmp((const char*)cur_node->name,"sensor"))
            {
                mds_point_strct->sensor = MDS_sensor_reader(cur_node, doc);
            }
            else if(!strcmp((const char*)cur_node->name,"extforce"))
            {
                mds_point_strct->extforce = MDS_extforce_reader(cur_node, doc);
            }
            else if(!strcmp((const char*)cur_node->name,"graphics"))
            {
                // to complet
            }
        }
        cur_node = cur_node->next;
    }
    return mds_point_strct ;
}

MDS_point_strct** MDS_point_list_reader(int *n_point, xmlNodePtr node, xmlDocPtr doc)
{
    int i = 0;
    int n = 0;
    xmlNodePtr cur_node;
    MDS_point_strct** point_list;

    cur_node = node;
    // counting loop
    while (cur_node != NULL)
    {
        if ((cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE) && !strcmp((const char*)cur_node->name,"point"))
        {
            n++;
        }
        cur_node = cur_node->next;
    }

    *n_point=n;

    point_list = (MDS_point_strct**) malloc(n*sizeof(MDS_point_strct*));

    // reading loop
    i = 0;
    cur_node = node;
    while (cur_node != NULL)
    {
        if ((cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE) && !strcmp((const char*)cur_node->name,"point"))
        {
            point_list[i] = MDS_point_reader(cur_node, doc);
            i++;
        }
        cur_node = cur_node->next;
    }
    return point_list;
}

MDS_ref_point_strct* MDS_ref_point_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_ref_point_strct* mds_ref_point_strct;

    xmlNodePtr cur_node = node->children;
    xmlChar* elementValue = NULL;

    mds_ref_point_strct = init_MDS_ref_point_strct();

    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"bodyname"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_ref_point_strct->bodyname = (char*)elementValue;
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"pointname"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_ref_point_strct->pointname = (char*)elementValue;
                xmlFree(elementValue);
            }
        }
        cur_node = cur_node->next;
    }
    return mds_ref_point_strct ;
}


MDS_base_strct* MDS_base_reader(xmlNodePtr node, xmlDocPtr doc)
{
    xmlNodePtr cur_node = node->children;
    xmlChar* elementValue = NULL;

    MDS_base_strct *mds_base_strct;

    int is_point_list_readed = 0;

    mds_base_strct = init_MDS_base_strct();

    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"gravity"))
            {
                MDS_coordinates_reader(mds_base_strct->gravity,cur_node->children,doc);
            }
            if(!strcmp((const char*)cur_node->name,"baseBodyName"))
            {
                // to complet
            }
            if(!strcmp((const char*)cur_node->name,"point") && !is_point_list_readed)
            {
                mds_base_strct->base_point_list = MDS_point_list_reader(&(mds_base_strct->n_base_point), cur_node, doc);
                is_point_list_readed = 1;
            }
            if(!strcmp((const char*)cur_node->name,"graphics"))
            {
                // to complet
            }
        }
        cur_node = cur_node->next;
    }
    return mds_base_strct;
}

MDS_body_strct* MDS_body_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_body_strct* mds_body_strct;

    xmlNodePtr cur_node = node->children;
    xmlNodePtr sub_cur_node;
    xmlChar* elementValue = NULL;

    int is_joint_list_readed = 0;
    int is_point_list_readed = 0;

    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"bodyname"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_body_strct = init_MDS_body_strct((char*)elementValue);
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"parent"))
            {
                mds_body_strct->parent =  MDS_ref_point_reader(cur_node, doc);
            }
            if(!strcmp((const char*)cur_node->name,"joint")&& !is_joint_list_readed)
            {
                mds_body_strct->joint_list = MDS_joint_list_reader(&(mds_body_strct->n_joint), cur_node, doc);
                is_joint_list_readed = 1;
            }
            if(!strcmp((const char*)cur_node->name,"mass"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_body_strct->mass = atof((char*)elementValue);
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"com"))
            {
                MDS_coordinates_reader(mds_body_strct->com,cur_node->children,doc);
            }
            if(!strcmp((const char*)cur_node->name,"inertia"))
            {
                sub_cur_node = cur_node->children;
                while (sub_cur_node != NULL)
                {
                    if (sub_cur_node->type == XML_ELEMENT_NODE || sub_cur_node->type == XML_COMMENT_NODE)
                    {
                        if(!strcmp((const char*)sub_cur_node->name,"Ixx"))
                        {
                            elementValue = xmlNodeListGetString(doc, sub_cur_node->xmlChildrenNode, 1);
                            mds_body_strct->inertia[0]= atof((char*)elementValue);
                            xmlFree(elementValue);
                        }
                        if(!strcmp((const char*)sub_cur_node->name,"Ixy"))
                        {
                            elementValue = xmlNodeListGetString(doc, sub_cur_node->xmlChildrenNode, 1);
                            mds_body_strct->inertia[1]= atof((char*)elementValue);
                            xmlFree(elementValue);
                        }
                        if(!strcmp((const char*)sub_cur_node->name,"Ixz"))
                        {
                            elementValue = xmlNodeListGetString(doc, sub_cur_node->xmlChildrenNode, 1);
                            mds_body_strct->inertia[2]= atof((char*)elementValue);
                            xmlFree(elementValue);
                        }
                        if(!strcmp((const char*)sub_cur_node->name,"Iyy"))
                        {
                            elementValue = xmlNodeListGetString(doc, sub_cur_node->xmlChildrenNode, 1);
                            mds_body_strct->inertia[3]= atof((char*)elementValue);
                            xmlFree(elementValue);
                        }
                        if(!strcmp((const char*)sub_cur_node->name,"Iyz"))
                        {
                            elementValue = xmlNodeListGetString(doc, sub_cur_node->xmlChildrenNode, 1);
                            mds_body_strct->inertia[4]= atof((char*)elementValue);
                            xmlFree(elementValue);
                        }
                        if(!strcmp((const char*)sub_cur_node->name,"Izz"))
                        {
                            elementValue = xmlNodeListGetString(doc, sub_cur_node->xmlChildrenNode, 1);
                            mds_body_strct->inertia[5]= atof((char*)elementValue);
                            xmlFree(elementValue);
                        }
                    }
                    sub_cur_node = sub_cur_node->next;
                }
            }
            if(!strcmp((const char*)cur_node->name,"point")&& !is_point_list_readed)
            {
                mds_body_strct->point_list = MDS_point_list_reader(&(mds_body_strct->n_point), cur_node, doc);
                is_point_list_readed = 1;
            }
            if(!strcmp((const char*)cur_node->name,"graphics"))
            {
                // to complet
            }
        }
        cur_node = cur_node->next;
    }
    return mds_body_strct;
}

MDS_bodytree_strct* MDS_bodytree_reader(xmlNodePtr node, xmlDocPtr doc)
{
    int i = 0;
    xmlNodePtr cur_node = node->children;

    MDS_bodytree_strct *mds_bodytree_strct;

    mds_bodytree_strct = init_MDS_bodytree_strct();

    // counting loop
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"body"))
            {
                (mds_bodytree_strct->n_body)++;
            }
        }
        cur_node = cur_node->next;
    }
    mds_bodytree_strct->body_list = (MDS_body_strct**) malloc(mds_bodytree_strct->n_body * sizeof(MDS_body_strct*));

    // reading loop
    cur_node = node->children;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"body"))
            {
                mds_bodytree_strct->body_list[i] = MDS_body_reader(cur_node, doc);
                i++;
            }
        }
        cur_node = cur_node->next;
    }
    return mds_bodytree_strct;
}

MDS_ball_strct* MDS_ball_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_ball_strct* mds_ball_strct;

    xmlNodePtr cur_node = node->children;
    xmlNodePtr sub_cur_node = NULL;
    xmlChar* elementValue = NULL;

    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"name"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_ball_strct = init_MDS_ball_strct((char*)elementValue);
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"endpoint1"))
            {
                mds_ball_strct->endpoint1 = MDS_ref_point_reader(cur_node, doc);
            }
            if(!strcmp((const char*)cur_node->name,"endpoint2"))
            {
                mds_ball_strct->endpoint2 = MDS_ref_point_reader(cur_node, doc);
            }
            if(!strcmp((const char*)cur_node->name,"ignore"))
            {
                sub_cur_node = cur_node->children;
                while (sub_cur_node != NULL)
                {
                    if (sub_cur_node->type == XML_ELEMENT_NODE || sub_cur_node->type == XML_COMMENT_NODE)
                    {
                        if(!strcmp((const char*)sub_cur_node->name,"x"))
                        {
                            mds_ball_strct->ignore[0] = 1;
                        }
                        if(!strcmp((const char*)sub_cur_node->name,"y"))
                        {
                            mds_ball_strct->ignore[1] = 1;
                        }
                        if(!strcmp((const char*)sub_cur_node->name,"z"))
                        {
                            mds_ball_strct->ignore[2] = 1;
                        }
                    }
                    sub_cur_node = sub_cur_node->next;
                }
            }
        }
        cur_node = cur_node->next;
    }
    return mds_ball_strct ;
}

MDS_rod_strct* MDS_rod_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_rod_strct* mds_rod_strct;

    xmlNodePtr cur_node = node->children;
    xmlChar* elementValue = NULL;

    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"name"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_rod_strct = init_MDS_rod_strct((char*)elementValue);
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"endpoint1"))
            {
                mds_rod_strct->endpoint1 = MDS_ref_point_reader(cur_node, doc);
            }
            if(!strcmp((const char*)cur_node->name,"endpoint2"))
            {
                mds_rod_strct->endpoint2 = MDS_ref_point_reader(cur_node, doc);
            }
            if(!strcmp((const char*)cur_node->name,"length"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_rod_strct->length = atof((char*)elementValue);
                xmlFree(elementValue);
            }
        }
        cur_node = cur_node->next;
    }
    return mds_rod_strct ;
}

MDS_solid_strct* MDS_solid_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_solid_strct* mds_solid_strct;

    xmlNodePtr cur_node = node->children;
    xmlNodePtr sub_cur_node = NULL;
    xmlChar* elementValue = NULL;

    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"name"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_solid_strct = init_MDS_solid_strct((char*)elementValue);
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"endpoint1"))
            {
                mds_solid_strct->endpoint1 = MDS_ref_point_reader(cur_node, doc);
            }
            if(!strcmp((const char*)cur_node->name,"endpoint2"))
            {
                mds_solid_strct->endpoint2 = MDS_ref_point_reader(cur_node, doc);
            }
            if(!strcmp((const char*)cur_node->name,"ignore"))
            {
                sub_cur_node = cur_node->children;
                while (sub_cur_node != NULL)
                {
                    if (sub_cur_node->type == XML_ELEMENT_NODE || sub_cur_node->type == XML_COMMENT_NODE)
                    {
                        if(!strcmp((const char*)sub_cur_node->name,"x"))
                        {
                            mds_solid_strct->ignore[0] = 1;
                        }
                        if(!strcmp((const char*)sub_cur_node->name,"y"))
                        {
                            mds_solid_strct->ignore[1] = 1;
                        }
                        if(!strcmp((const char*)sub_cur_node->name,"z"))
                        {
                            mds_solid_strct->ignore[2] = 1;
                        }
                    }
                    sub_cur_node = sub_cur_node->next;
                }
            }
        }
        cur_node = cur_node->next;
    }
    return mds_solid_strct ;
}

MDS_cuts_strct* MDS_cuts_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_cuts_strct *mds_cuts_strct;
    int i = 0;
    int j = 0;
    int k = 0;
    xmlNodePtr cur_node = node->children;

    mds_cuts_strct = init_MDS_cuts_strct();

    // counting loop
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"ball"))
            {
                (mds_cuts_strct->n_ball)++;
            }
            if(!strcmp((const char*)cur_node->name,"rod"))
            {
                (mds_cuts_strct->n_rod)++;
            }
            if(!strcmp((const char*)cur_node->name,"solid"))
            {
                (mds_cuts_strct->n_solid)++;
            }
        }
        cur_node = cur_node->next;
    }
    mds_cuts_strct->ball_list = (MDS_ball_strct**) malloc(mds_cuts_strct->n_ball * sizeof(MDS_ball_strct*));
    mds_cuts_strct->rod_list = (MDS_rod_strct**) malloc(mds_cuts_strct->n_rod * sizeof(MDS_rod_strct*));
    mds_cuts_strct->solid_list = (MDS_solid_strct**) malloc(mds_cuts_strct->n_solid * sizeof(MDS_solid_strct*));

    // reading loop
    cur_node = node->children;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"ball"))
            {
                mds_cuts_strct->ball_list[i] = MDS_ball_reader(cur_node, doc);
                i++;
            }
            if(!strcmp((const char*)cur_node->name,"rod"))
            {
                mds_cuts_strct->rod_list[j] = MDS_rod_reader(cur_node, doc);
                j++;
            }
            if(!strcmp((const char*)cur_node->name,"solid"))
            {
                mds_cuts_strct->solid_list[k] = MDS_solid_reader(cur_node, doc);
                k++;
            }
        }
        cur_node = cur_node->next;
    }

    return mds_cuts_strct;
}

MDS_link_strct* MDS_link_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_link_strct* mds_link_strct;

    xmlNodePtr cur_node = node->children;
    xmlChar* elementValue = NULL;

    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"name"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_link_strct = init_MDS_link_strct((char*)elementValue);
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"endpoint1"))
            {
                mds_link_strct->endpoint1 = MDS_ref_point_reader(cur_node, doc);
            }
            if(!strcmp((const char*)cur_node->name,"endpoint2"))
            {
                mds_link_strct->endpoint2 = MDS_ref_point_reader(cur_node, doc);
            }
        }
        cur_node = cur_node->next;
    }
    return mds_link_strct ;
}

MDS_links_strct* MDS_links_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_links_strct *mds_links_strct;
    int ind_link = 0;
    int ind_link3D = 0;
    xmlNodePtr cur_node = node->children;

    mds_links_strct = init_MDS_links_strct();

    // counting loop
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"link"))
            {
                (mds_links_strct->n_link)++;
            }
            if(!strcmp((const char*)cur_node->name,"link3D"))
            {
                (mds_links_strct->n_link3D)++;
            }
        }
        cur_node = cur_node->next;
    }
    mds_links_strct->link_list = (MDS_link_strct**) malloc(mds_links_strct->n_link * sizeof(MDS_link_strct*));
    mds_links_strct->link3D_list = (MDS_link_strct**) malloc(mds_links_strct->n_link3D * sizeof(MDS_link_strct*));

    // reading loop
    cur_node = node->children;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"link"))
            {
                mds_links_strct->link_list[ind_link] = MDS_link_reader(cur_node, doc);
                ind_link++;
            }
            if(!strcmp((const char*)cur_node->name,"link3D"))
            {
                mds_links_strct->link3D_list[ind_link3D] = MDS_link_reader(cur_node, doc);
                ind_link3D++;
            }
        }
        cur_node = cur_node->next;
    }

    return mds_links_strct;
}

MDS_parameter_strct* MDS_parameter_reader(xmlNodePtr node, xmlDocPtr doc)
{
    int i;
    MDS_parameter_strct* mds_parameter_strct = NULL;
    int ind_parameter = 0;
    xmlNodePtr cur_node = node->children;
    xmlChar* elementValue = NULL;

    // name loop
    while (cur_node != NULL && mds_parameter_strct == NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"name"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_parameter_strct = init_MDS_parameter_strct((char*)elementValue);
                xmlFree(elementValue);
            }
        }
        cur_node = cur_node->next;
    }

    // counting loop
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"value"))
            {
                (mds_parameter_strct->n_value)++;
            }
        }
        cur_node = cur_node->next;
    }
    mds_parameter_strct->value_list = (double*) get_dvec_0(mds_parameter_strct->n_value);

    /////////////////

    mds_parameter_strct->d_value =  (MDS_d_data_strct**) malloc(mds_parameter_strct->n_value * sizeof(MDS_d_data_strct*));

    for(i=0; i<mds_parameter_strct->n_value; i++)
    {
        mds_parameter_strct->d_value[i] = init_MDS_d_data_strct();
    }

    /////////////////

    // reading loop
    cur_node = node->children;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"type"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                if(!strcmp((const char*)elementValue,"scalar"))
                {
                    mds_parameter_strct->type = 1;
                }
                if(!strcmp((const char*)elementValue,"vector"))
                {
                    mds_parameter_strct->type = 2;
                }
                if(!strcmp((const char*)elementValue,"lut1D"))
                {
                    mds_parameter_strct->type = 3;
                }
                if(!strcmp((const char*)elementValue,"lut2D"))
                {
                    mds_parameter_strct->type = 4;
                }
                if(!strcmp((const char*)elementValue,"state"))
                {
                    mds_parameter_strct->type = 5;
                }
                if(!strcmp((const char*)elementValue,"structure"))
                {
                    mds_parameter_strct->type = 6;
                }
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"value"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_parameter_strct->value_list[ind_parameter] = atof((char*)elementValue);
                ind_parameter++;
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"structure"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                if (elementValue != NULL){
                    mds_parameter_strct->structure_name = strdup((char*)elementValue);
                }
                else{
                    mds_parameter_strct->structure_name = strdup("");
                }
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"header"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                if (elementValue != NULL){
                    mds_parameter_strct->structure_header = strdup((char*)elementValue);
                }
                else{
                    mds_parameter_strct->structure_header = strdup("");
                }
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"lutfile"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                if (elementValue != NULL){
                    if (strncmp((const char*)elementValue, "PRJPATH", 7)){
                        mds_parameter_strct->structure_lutfile = strdup((char*)elementValue);
                    }
                    else{
                        mds_parameter_strct->flag_PRJPATH = 1;
                        mds_parameter_strct->structure_lutfile = strdup((char*)elementValue+7);
                    }
                }
                else{
                    mds_parameter_strct->structure_lutfile = strdup("");
                }
                xmlFree(elementValue);
            }
        }
        cur_node = cur_node->next;
    }

    return mds_parameter_strct;
}

MDS_user_model_strct* MDS_user_model_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_user_model_strct* mds_user_model_strct = NULL;
    int ind_parameter = 0;
    xmlNodePtr cur_node = node->children;
    xmlChar* elementValue = NULL;

    // name loop
    while (cur_node != NULL && mds_user_model_strct == NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"name"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_user_model_strct = init_MDS_user_model_strct((char*)elementValue);
                xmlFree(elementValue);
            }
        }
        cur_node = cur_node->next;
    }

    // counting loop
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"parameter"))
            {
                (mds_user_model_strct->n_parameter)++;
            }
        }
        cur_node = cur_node->next;
    }
    mds_user_model_strct->parameter_list = (MDS_parameter_strct**) malloc(mds_user_model_strct->n_parameter * sizeof(MDS_parameter_strct*));

    // reading loop
    cur_node = node->children;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"parameter"))
            {
                mds_user_model_strct->parameter_list[ind_parameter] = MDS_parameter_reader(cur_node, doc);
                ind_parameter++;
            }
        }
        cur_node = cur_node->next;
    }

    return mds_user_model_strct;
}

MDS_user_models_strct* MDS_user_models_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_user_models_strct *mds_user_models_strct;
    int ind_user_model = 0;
    xmlNodePtr cur_node = node->children;

    mds_user_models_strct = init_MDS_user_models_strct();

    // counting loop
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"user_model"))
            {
                (mds_user_models_strct->n_user_model)++;
            }
        }
        cur_node = cur_node->next;
    }
    mds_user_models_strct->user_model_list = (MDS_user_model_strct**) malloc(mds_user_models_strct->n_user_model * sizeof(MDS_user_model_strct*));

    // reading loop
    cur_node = node->children;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"user_model"))
            {
                mds_user_models_strct->user_model_list[ind_user_model] = MDS_user_model_reader(cur_node, doc);
                ind_user_model++;
            }
        }
        cur_node = cur_node->next;
    }

    return mds_user_models_strct;
}

MDS_user_IO_strct* MDS_user_IO_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_user_IO_strct* mds_user_IO_strct = NULL;
    int ind_parameter = 0;
    xmlNodePtr cur_node = node->children;
    xmlChar* elementValue = NULL;

    // name loop
    while (cur_node != NULL && mds_user_IO_strct == NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"varname"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_user_IO_strct = init_MDS_user_IO_strct((char*)elementValue);
                xmlFree(elementValue);
            }
        }
        cur_node = cur_node->next;
    }

    // reading loop
    cur_node = node->children;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"type"))
            {

                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                if(!strcmp((char*)elementValue,"in"))
                {
                    mds_user_IO_strct->type = 1;
                }
                else if(!strcmp((char*)elementValue,"out"))
                {
                    mds_user_IO_strct->type = 2;
                }
                xmlFree(elementValue);
            }
            if(!strcmp((const char*)cur_node->name,"size"))
            {
                elementValue = xmlNodeListGetString(doc, cur_node->xmlChildrenNode, 1);
                mds_user_IO_strct->size = atoi((char*)elementValue);
                xmlFree(elementValue);
            }
        }
        cur_node = cur_node->next;
    }

    return mds_user_IO_strct;
}

MDS_user_IOs_strct* MDS_user_IOs_reader(xmlNodePtr node, xmlDocPtr doc)
{
    MDS_user_IOs_strct *mds_user_IOs_strct;
    int ind_user_IO = 0;
    xmlNodePtr cur_node = node->children;

    mds_user_IOs_strct = init_MDS_user_IOs_strct();

    // counting loop
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"user_variable"))
            {
                (mds_user_IOs_strct->n_user_IO)++;
            }
        }
        cur_node = cur_node->next;
    }
    mds_user_IOs_strct->user_IO_list = (MDS_user_IO_strct**) malloc(mds_user_IOs_strct->n_user_IO * sizeof(MDS_user_IO_strct*));

    // reading loop
    cur_node = node->children;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE || cur_node->type == XML_COMMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"user_variable"))
            {
                mds_user_IOs_strct->user_IO_list[ind_user_IO] = MDS_user_IO_reader(cur_node, doc);
                if(mds_user_IOs_strct->user_IO_list[ind_user_IO]->type == 1){
                    mds_user_IOs_strct->n_in++;
                }else if(mds_user_IOs_strct->user_IO_list[ind_user_IO]->type == 2){
                    mds_user_IOs_strct->n_out++;
                }
                ind_user_IO++;
            }
        }
        cur_node = cur_node->next;
    }

    return mds_user_IOs_strct;
}



MDS_gen_strct* MDS_mbsDescR7_reader(xmlNodePtr node, xmlDocPtr doc)
{
    xmlNodePtr cur_node;

    MDS_gen_strct *mds_gen_strct;
    int user_model_created = 0;
    int user_IO_created = 0;

    if (strcmp((const char*)node->name,"mbsDescR7"))
    {
        printf("MDS_reader : xml is not a mbsDescR7 \n");
        return NULL;
    }

    mds_gen_strct = init_MDS_gen_strct();
    cur_node = node->children;


    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"mbsname"))
            {
                // to complet
            }
            if(!strcmp((const char*)cur_node->name,"mbsyspad_version"))
            {
                // to complet
            }
            if(!strcmp((const char*)cur_node->name,"base"))
            {
                mds_gen_strct->base = MDS_base_reader(cur_node, doc);
            }
            if(!strcmp((const char*)cur_node->name,"bodytree"))
            {
                mds_gen_strct->bodytree = MDS_bodytree_reader(cur_node, doc);
            }
            if(!strcmp((const char*)cur_node->name,"cuts"))
            {
                mds_gen_strct->cuts = MDS_cuts_reader(cur_node, doc);
            }
            if(!strcmp((const char*)cur_node->name,"links"))
            {
                mds_gen_strct->links = MDS_links_reader(cur_node, doc);
            }
            if(!strcmp((const char*)cur_node->name,"user_models"))
            {
                mds_gen_strct->user_models = MDS_user_models_reader(cur_node, doc);
                user_model_created = 1;
            }
            if(!strcmp((const char*)cur_node->name,"user_variables"))
            {
                mds_gen_strct->user_IOs = MDS_user_IOs_reader(cur_node, doc);
                user_IO_created = 1;
            }
            if(!strcmp((const char*)cur_node->name,"x3D"))
            {
                // to complet
            }
        }
        cur_node = cur_node->next;
    }

    if(user_model_created == 0) // create a empty user model list, n_usermodel = 0;
    {
        mds_gen_strct->user_models = init_MDS_user_models_strct();
    }

    if(user_IO_created == 0) // create a empty user IO list, n_user_IO = 0;
    {
        mds_gen_strct->user_IOs = init_MDS_user_IOs_strct();
    }

    return mds_gen_strct;
}

void MDS_structurer(MDS_gen_strct* mds_gen_strct)
{
    int i,j;
    int ind_joint = 0;
    int ind_qu = 0;
    int ind_qv = 0;
    int ind_qc = 0;
    int ind_qlocked = 0;
    int ind_qdriven = 0;
    int ind_qa = 0;

    int ind_point = 0;
    int ind_sensor = 0;
    int ind_extforce = 0;
    int ind_state = 0;

    for(i=0; i<mds_gen_strct->base->n_base_point;i++)
    {
        if(mds_gen_strct->base->base_point_list[i]->sensor != NULL)
        {
            (mds_gen_strct->n_sensor)++;
        }
        if(mds_gen_strct->base->base_point_list[i]->extforce != NULL)
        {
            (mds_gen_strct->n_extforce)++;
        }
    }

    mds_gen_strct->n_point = mds_gen_strct->base->n_base_point;

    for(i=0; i<mds_gen_strct->bodytree->n_body;i++)
    {
        mds_gen_strct->n_point = mds_gen_strct->n_point + mds_gen_strct->bodytree->body_list[i]->n_point;
        for(j=0; j<mds_gen_strct->bodytree->body_list[i]->n_point; j++)
        {
            if(mds_gen_strct->bodytree->body_list[i]->point_list[j]->sensor != NULL)
            {
                (mds_gen_strct->n_sensor)++;
            }
            if(mds_gen_strct->bodytree->body_list[i]->point_list[j]->extforce != NULL)
            {
                (mds_gen_strct->n_extforce)++;
            }
        }
        for(j=0; j<mds_gen_strct->bodytree->body_list[i]->n_joint; j++)
        {
            (mds_gen_strct->bodytree->n_joint)++;

            switch( mds_gen_strct->bodytree->body_list[i]->joint_list[j]->nature)
            {
                case 1:
                    (mds_gen_strct->bodytree->n_qu)++;
                break;
                case 2:
                    (mds_gen_strct->bodytree->n_qv)++;
                break;
                case 3:
                    (mds_gen_strct->bodytree->n_qc)++;
                    (mds_gen_strct->bodytree->n_qdriven)++;
                break;
            }
            if(mds_gen_strct->bodytree->body_list[i]->joint_list[j]->actuated == 1)
            {
                (mds_gen_strct->bodytree->n_qa)++;
            }
        }
    }
    for(i=0; i<mds_gen_strct->user_models->n_user_model;i++)
    {
        for(j=0; j<mds_gen_strct->user_models->user_model_list[i]->n_parameter;j++)
        {
            if(mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->type == 5)
            {
                (mds_gen_strct->n_state)++;
                mds_gen_strct->n_state_value = mds_gen_strct->n_state_value + mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->n_value;
            }
        }
    }

    mds_gen_strct->bodytree->qu = get_ivec_0(mds_gen_strct->bodytree->n_qu);
    mds_gen_strct->bodytree->qv = get_ivec_0(mds_gen_strct->bodytree->n_qv);
    mds_gen_strct->bodytree->qc = get_ivec_0(mds_gen_strct->bodytree->n_qc);
    mds_gen_strct->bodytree->qlocked = get_ivec_0(mds_gen_strct->bodytree->n_qlocked);
    mds_gen_strct->bodytree->qdriven = get_ivec_0(mds_gen_strct->bodytree->n_qdriven);
    mds_gen_strct->bodytree->qa = get_ivec_0(mds_gen_strct->bodytree->n_qa);

    mds_gen_strct->bodytree->joint_list = (MDS_joint_strct**) malloc(mds_gen_strct->bodytree->n_joint * sizeof(MDS_joint_strct*));

    mds_gen_strct->point_list = (MDS_point_strct**) malloc(mds_gen_strct->n_point * sizeof(MDS_point_strct*));

    mds_gen_strct->sensor_point_list = get_ivec_0(mds_gen_strct->n_sensor);
    mds_gen_strct->extforce_point_list = get_ivec_0(mds_gen_strct->n_extforce);

    mds_gen_strct->state_list = (MDS_parameter_strct**) malloc(mds_gen_strct->n_point * sizeof(MDS_parameter_strct*));

    for(i=0; i<mds_gen_strct->base->n_base_point; i++)
    {
        mds_gen_strct->point_list[ind_point] = mds_gen_strct->base->base_point_list[i];
        if(mds_gen_strct->base->base_point_list[i]->sensor != NULL)
        {
            mds_gen_strct->sensor_point_list[ind_sensor] = ind_point;
            ind_sensor++;
        }
        if(mds_gen_strct->base->base_point_list[i]->extforce != NULL)
        {
            mds_gen_strct->extforce_point_list[ind_extforce] = ind_point;
            ind_extforce++;
        }

        ind_point++;
    }
    for(i=0; i<mds_gen_strct->bodytree->n_body;i++)
    {
        for(j=0; j<mds_gen_strct->bodytree->body_list[i]->n_point; j++)
        {
            mds_gen_strct->point_list[ind_point] = mds_gen_strct->bodytree->body_list[i]->point_list[j];
            if(mds_gen_strct->bodytree->body_list[i]->point_list[j]->sensor != NULL)
            {
                mds_gen_strct->sensor_point_list[ind_sensor] = ind_point;
                ind_sensor++;
            }
            if(mds_gen_strct->bodytree->body_list[i]->point_list[j]->extforce != NULL)
            {
                mds_gen_strct->extforce_point_list[ind_extforce] = ind_point;
                ind_extforce++;
            }
            ind_point++;
        }
        for(j=0; j<mds_gen_strct->bodytree->body_list[i]->n_joint; j++)
        {
            mds_gen_strct->bodytree->joint_list[ind_joint] = mds_gen_strct->bodytree->body_list[i]->joint_list[j];
            switch(mds_gen_strct->bodytree->body_list[i]->joint_list[j]->nature)
            {
                case 1:
                    mds_gen_strct->bodytree->qu[ind_qu] = ind_joint;
                    ind_qu++;
                break;
                case 2:
                    mds_gen_strct->bodytree->qv[ind_qv] = ind_joint;
                    ind_qv++;
                break;
                case 3:
                    mds_gen_strct->bodytree->qc[ind_qc] = ind_joint;
                    ind_qc++;
                    mds_gen_strct->bodytree->qdriven[ind_qdriven] = ind_joint;
                    ind_qdriven++;
                break;
            }
            if(mds_gen_strct->bodytree->body_list[i]->joint_list[j]->actuated == 1)
            {
                mds_gen_strct->bodytree->qa[ind_qa] = ind_joint;
                ind_qa++;
            }
            ind_joint++;
        }
    }
    for(i=0; i<mds_gen_strct->user_models->n_user_model;i++)
    {
        for(j=0; j<mds_gen_strct->user_models->user_model_list[i]->n_parameter;j++)
        {
            if(mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->type == 5)
            {
                mds_gen_strct->state_list[ind_state] =  mds_gen_strct->user_models->user_model_list[i]->parameter_list[j];
                ind_state++;
            }
        }
    }
    ///////////////////////////////////////////////////////////////
    /*for(i=0; i<mds_gen_strct->n_point; i++)
    {
        for(j=0; j<3; j++)
        {
            if(mds_gen_strct->point_list[i]->pt[j] == 0.0)
            {
                mds_gen_strct->point_list[i]->d_pt[j]->d_type = 1;
            }
            else
            {
                mds_gen_strct->point_list[i]->d_pt[j]->d_type = 2;
                mds_gen_strct->point_list[i]->d_pt[j]->d_0 = mds_gen_strct->point_list[i]->pt[j];
            }
        }
    }*/

    ///////////////////////////////////////////////////////////////
}


MDS_gen_strct* MDS_mbs_reader(const char* mbs_xml_name)
{
    xmlDocPtr doc;
    xmlNodePtr root;
    MDS_gen_strct* mds_gen_strct;

    // xml document opening
    xmlKeepBlanksDefault(0); // Ignore les noeuds texte composant la mise en forme
    doc = xmlParseFile(mbs_xml_name);
    if (doc == NULL) {
        fprintf(stderr, "MDS_mbs_reader : Invalid XML document\n");
        return NULL;
    }
    // getting the root
    root = xmlDocGetRootElement(doc);
    if (root == NULL) {
        fprintf(stderr, "MDS_mbs_reader : Empty XML document\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    mds_gen_strct = MDS_mbsDescR7_reader(root, doc);

    MDS_structurer(mds_gen_strct);

    mds_gen_strct->mbsfile = strdup(mbs_xml_name);

    // free the memory allocate to the xml parser
    xmlFreeDoc(doc);

    return mds_gen_strct;
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Init and free functions for MDS structures  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


MDS_d_data_strct* init_MDS_d_data_strct(void)
{
    MDS_d_data_strct *mds_d_data_strct;

    mds_d_data_strct = (MDS_d_data_strct*) malloc(sizeof(MDS_d_data_strct));

    mds_d_data_strct->d_type = -1;
    mds_d_data_strct->d_0 = 0.0;
    mds_d_data_strct->d_min = 0.0;
    mds_d_data_strct->d_max = 0.0;
    mds_d_data_strct->d_opti = 0.0;

    mds_d_data_strct->is_related = 0;
    mds_d_data_strct->master_d_ptr = NULL;
    mds_d_data_strct->d_ptr = NULL;

    mds_d_data_strct->MBSdata_d_ptr = NULL;

    return mds_d_data_strct;
}
void free_MDS_d_data_strct(MDS_d_data_strct *mds_d_data_strct)
{
    free(mds_d_data_strct);
}


MDS_ref_point_strct* init_MDS_ref_point_strct(void)
{
    MDS_ref_point_strct *mds_ref_point_strct;

    mds_ref_point_strct = (MDS_ref_point_strct*) malloc(sizeof(MDS_ref_point_strct));

    mds_ref_point_strct->bodyname = NULL;
    mds_ref_point_strct->pointname = NULL;

    return mds_ref_point_strct;
}
void free_MDS_ref_point_strct(MDS_ref_point_strct *mds_ref_point_strct)
{
    free(mds_ref_point_strct);
}

MDS_sensor_strct* init_MDS_sensor_strct(char* name)
{
    MDS_sensor_strct *mds_sensor_strct;

    mds_sensor_strct = (MDS_sensor_strct*) malloc(sizeof(MDS_sensor_strct));

    mds_sensor_strct->name = strdup(name);

    mds_sensor_strct->Pos = 0;
    mds_sensor_strct->Rot = 0;
    mds_sensor_strct->Vit = 0;
    mds_sensor_strct->Om = 0;
    mds_sensor_strct->Acc = 0;
    mds_sensor_strct->Omp = 0;
    mds_sensor_strct->Jac = 0;

    return mds_sensor_strct;
}
void free_MDS_sensor_strct(MDS_sensor_strct *mds_sensor_strct)
{
    free(mds_sensor_strct->name);
    free(mds_sensor_strct);
}

MDS_extforce_strct* init_MDS_extforce_strct(char* name)
{
    MDS_extforce_strct *mds_extforce_strct;

    mds_extforce_strct = (MDS_extforce_strct*) malloc(sizeof(MDS_extforce_strct));

    mds_extforce_strct->name = strdup(name);

    mds_extforce_strct->Pos = 0;
    mds_extforce_strct->Rot = 0;
    mds_extforce_strct->Vit = 0;
    mds_extforce_strct->Om = 0;
    mds_extforce_strct->Acc = 0;
    mds_extforce_strct->Omp = 0;

    return mds_extforce_strct;
}
void free_MDS_extforce_strct(MDS_extforce_strct *mds_extforce_strct)
{
    free(mds_extforce_strct->name);
    free(mds_extforce_strct);
}


MDS_point_strct* init_MDS_point_strct(char* name)
{
    int i;
    MDS_point_strct *mds_point_strct;

    mds_point_strct = (MDS_point_strct*) malloc(sizeof(MDS_point_strct));

    mds_point_strct->name = strdup(name);

    mds_point_strct->pt = get_dvec_0(3);

    mds_point_strct->sensor = NULL;
    mds_point_strct->extforce = NULL;

    mds_point_strct->d_pt = (MDS_d_data_strct**) malloc(3*sizeof(MDS_d_data_strct*));

    for(i=0; i<3; i++)
    {
        mds_point_strct->d_pt[i] = init_MDS_d_data_strct();
    }

    mds_point_strct->is_symmmetric = 0;
    mds_point_strct->symmetric_point_name = NULL;

    return mds_point_strct;
}
void free_MDS_point_strct(MDS_point_strct *mds_point_strct)
{
    int i;
    free(mds_point_strct->name);
    free_dvec_0(mds_point_strct->pt);

    if(mds_point_strct->sensor!=NULL)
    {
        free_MDS_sensor_strct(mds_point_strct->sensor);
    }
    if(mds_point_strct->extforce!=NULL)
    {
        free_MDS_extforce_strct(mds_point_strct->extforce);
    }

    for(i=0; i<3; i++)
    {
        free_MDS_d_data_strct(mds_point_strct->d_pt[i]);
    }
    free(mds_point_strct->d_pt);


    free(mds_point_strct);
}

MDS_base_strct* init_MDS_base_strct(void)
{
    MDS_base_strct *mds_base_strct;

    mds_base_strct = (MDS_base_strct*) malloc(sizeof(MDS_base_strct));

    mds_base_strct->gravity = get_dvec_0(3);

    mds_base_strct->n_base_point = 0;
    mds_base_strct->base_point_list = NULL;

    return mds_base_strct;
}
void free_MDS_base_strct(MDS_base_strct *mds_base_strct)
{
    int i = 0;

    free_dvec_0(mds_base_strct->gravity);

    for(i=0; i<mds_base_strct->n_base_point;i++)
    {
        free_MDS_point_strct(mds_base_strct->base_point_list[i]);
    }
    free(mds_base_strct->base_point_list);

    free(mds_base_strct);
}

MDS_joint_strct* init_MDS_joint_strct(char* name)
{
    MDS_joint_strct *mds_joint_strct;

    mds_joint_strct = (MDS_joint_strct*) malloc(sizeof(MDS_joint_strct));

    mds_joint_strct->name = strdup(name);

    mds_joint_strct->type = 0;
    mds_joint_strct->nature = 0;

    mds_joint_strct->q0 = 0.0;
    mds_joint_strct->qd0 = 0.0;
    mds_joint_strct->qdd0 = 0.0;

    mds_joint_strct->actuated = 0;

    mds_joint_strct->d_qf = init_MDS_d_data_strct();

    mds_joint_strct->is_symmmetric = 0;
    mds_joint_strct->symmetric_joint_name = NULL;

    return mds_joint_strct;
}
void free_MDS_joint_strct(MDS_joint_strct *mds_joint_strct)
{
    free(mds_joint_strct->name);

    free_MDS_d_data_strct(mds_joint_strct->d_qf);

    free(mds_joint_strct);
}

MDS_body_strct* init_MDS_body_strct(char* name)
{
    MDS_body_strct *mds_body_strct;

    mds_body_strct = (MDS_body_strct*) malloc(sizeof(MDS_body_strct));

    mds_body_strct->name = strdup(name);

    mds_body_strct->parent = NULL;

    mds_body_strct->n_joint = 0;
    mds_body_strct->joint_list = NULL;

    mds_body_strct->mass = 0.0;
    mds_body_strct->com = get_dvec_0(3);
    mds_body_strct->inertia = get_dvec_0(6);

    mds_body_strct->n_point = 0;
    mds_body_strct->point_list = NULL;

    return mds_body_strct;
}
void free_MDS_body_strct(MDS_body_strct *mds_body_strct)
{
    int i=0;

    free(mds_body_strct->name);

    free_MDS_ref_point_strct(mds_body_strct->parent);

    for(i=0; i<mds_body_strct->n_joint;i++)
    {
        free_MDS_joint_strct(mds_body_strct->joint_list[i]);
    }
    free(mds_body_strct->joint_list);

    free_dvec_0(mds_body_strct->com);
    free_dvec_0(mds_body_strct->inertia);

    for(i=0; i<mds_body_strct->n_point;i++)
    {
        free_MDS_point_strct(mds_body_strct->point_list[i]);
    }
    free(mds_body_strct->point_list);

    free(mds_body_strct);
}

MDS_bodytree_strct* init_MDS_bodytree_strct(void)
{
    MDS_bodytree_strct *mds_bodytree_strct;

    mds_bodytree_strct = (MDS_bodytree_strct*) malloc(sizeof(MDS_bodytree_strct));

    mds_bodytree_strct->n_body = 0;
    mds_bodytree_strct->body_list = NULL;

    mds_bodytree_strct->n_joint = 0;
    mds_bodytree_strct->joint_list = NULL;

    mds_bodytree_strct->n_qu = 0;
    mds_bodytree_strct->qu = NULL;

    mds_bodytree_strct->n_qv = 0;
    mds_bodytree_strct->qv = NULL;

    mds_bodytree_strct->n_qc = 0;
    mds_bodytree_strct->qc = NULL;

    mds_bodytree_strct->n_qlocked = 0;
    mds_bodytree_strct->qlocked = NULL;

    mds_bodytree_strct->n_qdriven = 0;
    mds_bodytree_strct->qdriven = NULL;

    mds_bodytree_strct->n_qa = 0;
    mds_bodytree_strct->qa = NULL;

    return mds_bodytree_strct;
}
void free_MDS_bodytree_strct(MDS_bodytree_strct *mds_bodytree_strct)
{
    int i = 0;

    for(i=0; i<mds_bodytree_strct->n_body;i++)
    {
        free_MDS_body_strct(mds_bodytree_strct->body_list[i]);
    }
    free(mds_bodytree_strct->body_list);

    free(mds_bodytree_strct->joint_list);

    free_ivec_0(mds_bodytree_strct->qu);
    free_ivec_0(mds_bodytree_strct->qv);
    free_ivec_0(mds_bodytree_strct->qc);
    free_ivec_0(mds_bodytree_strct->qlocked);
    free_ivec_0(mds_bodytree_strct->qdriven);
    free_ivec_0(mds_bodytree_strct->qa);

    free(mds_bodytree_strct);
}

MDS_ball_strct* init_MDS_ball_strct(char* name)
{
    MDS_ball_strct *mds_ball_strct;

    mds_ball_strct = (MDS_ball_strct*) malloc(sizeof(MDS_ball_strct));

    mds_ball_strct->name = strdup(name);

    mds_ball_strct->endpoint1 = NULL;
    mds_ball_strct->endpoint2 = NULL;

    mds_ball_strct->ignore = get_ivec_0(3);

    return mds_ball_strct;
}
void free_MDS_ball_strct(MDS_ball_strct *mds_ball_strct)
{
    free(mds_ball_strct->name);

    if(mds_ball_strct->endpoint1!=NULL)
    {
        free_MDS_ref_point_strct(mds_ball_strct->endpoint1);
    }
    if(mds_ball_strct->endpoint2!=NULL)
    {
        free_MDS_ref_point_strct(mds_ball_strct->endpoint2);
    }

    free_ivec_0(mds_ball_strct->ignore);
    free(mds_ball_strct);
}

MDS_rod_strct* init_MDS_rod_strct(char* name)
{
    MDS_rod_strct *mds_rod_strct;

    mds_rod_strct = (MDS_rod_strct*) malloc(sizeof(MDS_rod_strct));

    mds_rod_strct->name = strdup(name);

    mds_rod_strct->length = 0.0;

    mds_rod_strct->endpoint1 = NULL;
    mds_rod_strct->endpoint2 = NULL;

    mds_rod_strct->d_length = init_MDS_d_data_strct();

    mds_rod_strct->is_symmmetric = 0;
    mds_rod_strct->symmetric_rod_name = NULL;

    return mds_rod_strct;
}
void free_MDS_rod_strct(MDS_rod_strct *mds_rod_strct)
{
    free(mds_rod_strct->name);

    if(mds_rod_strct->endpoint1!=NULL)
    {
        free_MDS_ref_point_strct(mds_rod_strct->endpoint1);
    }
    if(mds_rod_strct->endpoint2!=NULL)
    {
        free_MDS_ref_point_strct(mds_rod_strct->endpoint2);
    }

    free_MDS_d_data_strct(mds_rod_strct->d_length);

    free(mds_rod_strct);
}

MDS_solid_strct* init_MDS_solid_strct(char* name)
{
    MDS_solid_strct *mds_solid_strct;

    mds_solid_strct = (MDS_solid_strct*) malloc(sizeof(MDS_solid_strct));

    mds_solid_strct->name = strdup(name);

    mds_solid_strct->endpoint1 = NULL;
    mds_solid_strct->endpoint2 = NULL;

    mds_solid_strct->ignore = get_ivec_0(3);

    return mds_solid_strct;
}
void free_MDS_solid_strct(MDS_solid_strct *mds_solid_strct)
{
    free(mds_solid_strct->name);

    if(mds_solid_strct->endpoint1!=NULL)
    {
        free_MDS_ref_point_strct(mds_solid_strct->endpoint1);
    }
    if(mds_solid_strct->endpoint2!=NULL)
    {
        free_MDS_ref_point_strct(mds_solid_strct->endpoint2);
    }

    free_ivec_0(mds_solid_strct->ignore);

    free(mds_solid_strct);
}

MDS_cuts_strct* init_MDS_cuts_strct()
{
    MDS_cuts_strct *mds_cuts_strct;

    mds_cuts_strct = (MDS_cuts_strct*) malloc(sizeof(MDS_cuts_strct));

    mds_cuts_strct->n_ball = 0;
    mds_cuts_strct->n_rod = 0;
    mds_cuts_strct->n_solid = 0;

    mds_cuts_strct->ball_list = NULL;
    mds_cuts_strct->rod_list = NULL;
    mds_cuts_strct->solid_list = NULL;

    return mds_cuts_strct;
}
void free_MDS_cuts_strct(MDS_cuts_strct *mds_cuts_strct)
{
    int i;

    // if there were no cut, nothing ot free
    if(mds_cuts_strct==NULL) return;

    if(mds_cuts_strct->ball_list!=NULL)
    {
        for(i=0; i<mds_cuts_strct->n_ball; i++)
        {
            free_MDS_ball_strct(mds_cuts_strct->ball_list[i]);
        }
        free(mds_cuts_strct->ball_list);
    }
    if(mds_cuts_strct->rod_list!=NULL)
    {
        for(i=0; i<mds_cuts_strct->n_rod; i++)
        {
            free_MDS_rod_strct(mds_cuts_strct->rod_list[i]);
        }
        free(mds_cuts_strct->rod_list);
    }
    if(mds_cuts_strct->solid_list!=NULL)
    {
        for(i=0; i<mds_cuts_strct->n_solid; i++)
        {
            free_MDS_solid_strct(mds_cuts_strct->solid_list[i]);
        }
        free(mds_cuts_strct->solid_list);
    }
    free(mds_cuts_strct);
}

MDS_link_strct* init_MDS_link_strct(char* name)
{
    MDS_link_strct *mds_link_strct;

    mds_link_strct = (MDS_link_strct*) malloc(sizeof(MDS_link_strct));

    mds_link_strct->name = strdup(name);

    mds_link_strct->endpoint1 = NULL;
    mds_link_strct->endpoint2 = NULL;

    return mds_link_strct;
}
void free_MDS_link_strct(MDS_link_strct *mds_link_strct)
{
    free(mds_link_strct->name);

    if(mds_link_strct==NULL) return;

    if(mds_link_strct->endpoint1!=NULL)
    {
        free_MDS_ref_point_strct(mds_link_strct->endpoint1);
    }
    if(mds_link_strct->endpoint2!=NULL)
    {
        free_MDS_ref_point_strct(mds_link_strct->endpoint2);
    }
    free(mds_link_strct);
}

MDS_links_strct* init_MDS_links_strct()
{
    MDS_links_strct *mds_links_strct;

    mds_links_strct = (MDS_links_strct*) malloc(sizeof(MDS_links_strct));

    mds_links_strct->n_link = 0;
    mds_links_strct->link_list = NULL;

    mds_links_strct->n_link3D = 0;
    mds_links_strct->link3D_list = NULL;

    return mds_links_strct;
}
void free_MDS_links_strct(MDS_links_strct *mds_links_strct)
{
    int i;

    // if no link, nothing to do
    if(mds_links_strct==NULL) return;


    if(mds_links_strct->link_list!=NULL)
    {
        for(i=0; i<mds_links_strct->n_link; i++)
        {
            free_MDS_link_strct(mds_links_strct->link_list[i]);
        }
        free(mds_links_strct->link_list);
    }
    if(mds_links_strct->link3D_list!=NULL)
    {
        for(i=0; i<mds_links_strct->n_link3D; i++)
        {
            free_MDS_link_strct(mds_links_strct->link3D_list[i]);
        }
        free(mds_links_strct->link3D_list);
    }
    free(mds_links_strct);
}

MDS_parameter_strct* init_MDS_parameter_strct(char* name)
{
    MDS_parameter_strct *mds_parameter_strct;

    mds_parameter_strct = (MDS_parameter_strct*) malloc(sizeof(MDS_parameter_strct));

    mds_parameter_strct->name = strdup(name);

    mds_parameter_strct->type = 0;
    mds_parameter_strct->n_value = 0;

    mds_parameter_strct->value_list = NULL;

    mds_parameter_strct->d_value = NULL;

    mds_parameter_strct->is_symmmetric = 0;
    mds_parameter_strct->symmetric_parameter_name = NULL;

    mds_parameter_strct->structure_name = NULL;
    mds_parameter_strct->structure_header = NULL;

    mds_parameter_strct->flag_PRJPATH = 0;
    mds_parameter_strct->structure_lutfile = NULL;

    return mds_parameter_strct;
}

void free_MDS_parameter_strct(MDS_parameter_strct *mds_parameter_strct)
{
    int i;
    free(mds_parameter_strct->name);

    if(mds_parameter_strct->value_list != NULL)
    {
        free_dvec_0(mds_parameter_strct->value_list);
    }

    for(i=0; i<mds_parameter_strct->n_value; i++)
    {
        free_MDS_d_data_strct(mds_parameter_strct->d_value[i]);
    }

    free(mds_parameter_strct->d_value);

    if (mds_parameter_strct->structure_header != NULL){
        free(mds_parameter_strct->structure_header);
    }

    if (mds_parameter_strct->structure_name != NULL){
        free(mds_parameter_strct->structure_name);
    }

    if (mds_parameter_strct->structure_lutfile != NULL){
        free(mds_parameter_strct->structure_lutfile);
    }

    free(mds_parameter_strct);
}

MDS_user_model_strct* init_MDS_user_model_strct(char* name)
{
    MDS_user_model_strct *mds_user_model_strct;

    mds_user_model_strct = (MDS_user_model_strct*) malloc(sizeof(MDS_user_model_strct));

    mds_user_model_strct->name = strdup(name);
    mds_user_model_strct->n_parameter = 0;
    mds_user_model_strct->parameter_list = NULL;

    return mds_user_model_strct;
}
void free_MDS_user_model_strct(MDS_user_model_strct *mds_user_model_strct)
{
    int i;
    free(mds_user_model_strct->name);

    if(mds_user_model_strct->parameter_list != NULL)
    {
        for(i=0; i<mds_user_model_strct->n_parameter; i++)
        {
            free_MDS_parameter_strct(mds_user_model_strct->parameter_list[i]);
        }
    }

    free(mds_user_model_strct->parameter_list);

    free(mds_user_model_strct);
}

MDS_user_models_strct* init_MDS_user_models_strct(void)
{
    MDS_user_models_strct *mds_user_models_strct;

    mds_user_models_strct = (MDS_user_models_strct*) malloc(sizeof(MDS_user_models_strct));

    mds_user_models_strct->n_user_model = 0;
    mds_user_models_strct->user_model_list = NULL;

    return mds_user_models_strct;
}
void free_MDS_user_models_strct(MDS_user_models_strct *mds_user_models_strct)
{
    int i;
    /*free(mds_user_models_strct->name);*/

    // if no user model, nothing to do
    if(mds_user_models_strct==NULL) return;

    if(mds_user_models_strct->user_model_list != NULL)
    {
        for(i=0; i<mds_user_models_strct->n_user_model; i++)
        {
            free_MDS_user_model_strct(mds_user_models_strct->user_model_list[i]);
        }
        free(mds_user_models_strct->user_model_list);
    }

    free(mds_user_models_strct);
}

MDS_user_IO_strct* init_MDS_user_IO_strct(char* name)
{
    MDS_user_IO_strct *mds_user_IO_strct;

    mds_user_IO_strct = (MDS_user_IO_strct*) malloc(sizeof(MDS_user_IO_strct));

    mds_user_IO_strct->name = strdup(name);
    mds_user_IO_strct->type = 0;
    mds_user_IO_strct->size = 0;

    return mds_user_IO_strct;
}
void free_MDS_user_IO_strct(MDS_user_IO_strct *mds_user_IO_strct)
{
    free(mds_user_IO_strct->name);
    free(mds_user_IO_strct);
}

MDS_user_IOs_strct* init_MDS_user_IOs_strct(void)
{
    MDS_user_IOs_strct *mds_user_IOs_strct;

    mds_user_IOs_strct = (MDS_user_IOs_strct*) malloc(sizeof(MDS_user_IOs_strct));

    mds_user_IOs_strct->n_user_IO = 0;
    mds_user_IOs_strct->n_in = 0;
    mds_user_IOs_strct->n_out = 0;
    mds_user_IOs_strct->user_IO_list = NULL;

    return mds_user_IOs_strct;
}
void free_MDS_user_IOs_strct(MDS_user_IOs_strct *mds_user_IOs_strct)
{
    int i;

    // if no user IO, nothing to do
    if(mds_user_IOs_strct==NULL) return;

    if(mds_user_IOs_strct->user_IO_list != NULL)
    {
        for(i=0; i<mds_user_IOs_strct->n_user_IO; i++)
        {
            free_MDS_user_IO_strct(mds_user_IOs_strct->user_IO_list[i]);
        }
        free(mds_user_IOs_strct->user_IO_list);
    }

    free(mds_user_IOs_strct);
}

MDS_opti_strct* init_MDS_opti_strct(void)
{
    MDS_opti_strct *mds_opti_strct;

    mds_opti_strct = (MDS_opti_strct*) malloc(sizeof(MDS_opti_strct));

    mds_opti_strct->n_d_simplified = 0;
    mds_opti_strct->n_d_forced = 0;
    mds_opti_strct->n_d_optimized = 0;
    mds_opti_strct->n_d_resulting = 0;
    mds_opti_strct->n_d_related = 0;

    mds_opti_strct->d_simplified_list = NULL;
    mds_opti_strct->d_forced_list = NULL;
    mds_opti_strct->d_optimized_list = NULL;
    mds_opti_strct->d_resulting_list = NULL;
    mds_opti_strct->d_related_list = NULL;


    return mds_opti_strct;
}
void free_MDS_opti_strct(MDS_opti_strct *mds_opti_strct)
{
    if(mds_opti_strct != NULL)
    {
        free(mds_opti_strct->d_simplified_list);
        free(mds_opti_strct->d_forced_list);
        free(mds_opti_strct->d_optimized_list);
        free(mds_opti_strct->d_resulting_list);
        free(mds_opti_strct->d_related_list);
    }
    free(mds_opti_strct);
}

MDS_gen_strct* init_MDS_gen_strct(void)
{
    MDS_gen_strct *mds_gen_strct;

    mds_gen_strct = (MDS_gen_strct*) malloc(sizeof(MDS_gen_strct));

    mds_gen_strct->base = NULL;
    mds_gen_strct->bodytree = NULL;
    mds_gen_strct->cuts = NULL;
    mds_gen_strct->links = NULL;
    mds_gen_strct->user_models = NULL;
    mds_gen_strct->user_IOs = NULL;

    mds_gen_strct->n_point = 0;
    mds_gen_strct->point_list = NULL;

    mds_gen_strct->n_sensor = 0;
    mds_gen_strct->sensor_point_list = NULL;

    mds_gen_strct->n_extforce = 0;
    mds_gen_strct->extforce_point_list = NULL;

    mds_gen_strct->n_state = 0;
    mds_gen_strct->n_state_value = 0;
    mds_gen_strct->state_list = NULL;

    mds_gen_strct->opti = NULL;

    return mds_gen_strct;
}
void free_MDS_gen_strct(MDS_gen_strct *mds_gen_strct)
{
    free_MDS_base_strct(mds_gen_strct->base);
    free_MDS_bodytree_strct(mds_gen_strct->bodytree);
    free_MDS_cuts_strct(mds_gen_strct->cuts);
    free_MDS_links_strct(mds_gen_strct->links);
    free_MDS_user_models_strct(mds_gen_strct->user_models);
    free_MDS_user_IOs_strct(mds_gen_strct->user_IOs);

    free(mds_gen_strct->point_list);

    free_ivec_0(mds_gen_strct->sensor_point_list);
    free_ivec_0(mds_gen_strct->extforce_point_list);

    free(mds_gen_strct->state_list);

    free_MDS_opti_strct(mds_gen_strct->opti);

    free(mds_gen_strct->mbsfile);

    free(mds_gen_strct);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


xmlNodePtr MDS_find_xmlNodePtr(xmlNodePtr node, char *name)
{
    xmlNodePtr cur_node = node;
    xmlChar* elementValue = NULL;

    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->properties->name,"name"))
            {
                elementValue = xmlNodeListGetString(cur_node->doc, cur_node->properties->xmlChildrenNode, 1);
            }
            if(!strcmp((const char*)elementValue,name))
            {
                return cur_node;
            }
        }
        cur_node = cur_node->next;
    }
    return NULL;
}

xmlNodePtr MDS_create_d_data_xmlNodePtr(MDS_d_data_strct *mds_d_data_strct, char *name)
{
    xmlNodePtr d_data_node;
    char cur_char[30];

    if ((d_data_node = xmlNewNode(NULL, BAD_CAST "d_data")) == NULL)
    {
        return NULL;
    }
    if (xmlSetProp(d_data_node, BAD_CAST "name", BAD_CAST name ) == NULL)
    {
        xmlFreeNode(d_data_node);
        return NULL;
    }
    sprintf(cur_char, "%d", mds_d_data_strct->d_type);
    if (xmlNewTextChild(d_data_node, NULL, BAD_CAST "d_type", BAD_CAST cur_char) == NULL)
    {
        xmlFreeNode(d_data_node);
        return NULL;
    }
    sprintf(cur_char, "%f", mds_d_data_strct->d_0);
    if (xmlNewTextChild(d_data_node, NULL, BAD_CAST "d_0", BAD_CAST cur_char) == NULL)
    {
        xmlFreeNode(d_data_node);
        return NULL;
    }
    sprintf(cur_char, "%f", mds_d_data_strct->d_min);
    if (xmlNewTextChild(d_data_node, NULL, BAD_CAST "d_min", BAD_CAST cur_char) == NULL)
    {
        xmlFreeNode(d_data_node);
        return NULL;
    }
    sprintf(cur_char, "%f", mds_d_data_strct->d_max);
    if (xmlNewTextChild(d_data_node, NULL, BAD_CAST "d_max", BAD_CAST cur_char) == NULL)
    {
        xmlFreeNode(d_data_node);
        return NULL;
    }
    sprintf(cur_char, "%f", mds_d_data_strct->d_opti);
    if (xmlNewTextChild(d_data_node, NULL, BAD_CAST "d_opti", BAD_CAST cur_char) == NULL)
    {
        xmlFreeNode(d_data_node);
        return NULL;
    }
    return d_data_node;
}

xmlNodePtr MDS_create_point_xmlNodePtr(MDS_point_strct *mds_point_strct)
{
    int i;
    xmlNodePtr point_node;
    xmlNodePtr d_data_node_x, d_data_node_y, d_data_node_z;
    char cur_char[30];

    if ((point_node = xmlNewNode(NULL, BAD_CAST "point")) == NULL) {
        return NULL;
    }
    if (xmlSetProp(point_node, BAD_CAST "name", BAD_CAST mds_point_strct->name ) == NULL) {
        xmlFreeNode(point_node);
        return NULL;
    }
#ifdef GEN_MDS
    for(i=0; i<3; i++)
    {
        if(mds_point_strct->pt[i] == 0.0)
        {
            mds_point_strct->d_pt[i]->d_type = 1;
        }
        else
        {
            mds_point_strct->d_pt[i]->d_type = 2;
            mds_point_strct->d_pt[i]->d_0 = mds_point_strct->pt[i];
            mds_point_strct->d_pt[i]->d_opti = mds_point_strct->pt[i];
        }
    }
#endif
    d_data_node_x = MDS_create_d_data_xmlNodePtr(mds_point_strct->d_pt[0],"x");
    if (xmlAddChild(point_node, d_data_node_x) == NULL)
    {
        xmlFreeNode(point_node);
        xmlFreeNode(d_data_node_x);
        return NULL;
    }
    d_data_node_y = MDS_create_d_data_xmlNodePtr(mds_point_strct->d_pt[1],"y");
    if (xmlAddChild(point_node, d_data_node_y) == NULL)
    {
        xmlFreeNode(point_node);
        xmlFreeNode(d_data_node_y);
        return NULL;
    }
    d_data_node_z = MDS_create_d_data_xmlNodePtr(mds_point_strct->d_pt[2],"z");
    if (xmlAddChild(point_node, d_data_node_z) == NULL)
    {
        xmlFreeNode(point_node);
        xmlFreeNode(d_data_node_z);
        return NULL;
    }
    sprintf(cur_char, "%d", mds_point_strct->is_symmmetric);
    if (xmlNewTextChild(point_node, NULL, BAD_CAST "is_symmetric", BAD_CAST cur_char) == NULL)
    {
        xmlFreeNode(point_node);
        return NULL;
    }
    if (xmlNewTextChild(point_node, NULL, BAD_CAST "symmetric_point_name", BAD_CAST mds_point_strct->symmetric_point_name) == NULL)
    {
        xmlFreeNode(point_node);
        return NULL;
    }
    return point_node;
}

xmlNodePtr MDS_create_joint_xmlNodePtr(MDS_joint_strct *mds_joint_strct)
{
    int i;
    xmlNodePtr joint_node;
    xmlNodePtr d_data_node;
    char cur_char[30];

    if ((joint_node = xmlNewNode(NULL, BAD_CAST "joint")) == NULL) {
        return NULL;
    }
    if (xmlSetProp(joint_node, BAD_CAST "name", BAD_CAST mds_joint_strct->name ) == NULL) {
        xmlFreeNode(joint_node);
        return NULL;
    }
#ifdef GEN_MDS
    if(mds_joint_strct->q0 == 0.0)
    {
        mds_joint_strct->d_qf->d_type = 1;
    }
    else
    {
        mds_joint_strct->d_qf->d_type = 2;
        mds_joint_strct->d_qf->d_0 = mds_joint_strct->q0;
        mds_joint_strct->d_qf->d_opti = mds_joint_strct->q0;
    }
#endif
    d_data_node = MDS_create_d_data_xmlNodePtr(mds_joint_strct->d_qf,"d_qf");
    if (xmlAddChild(joint_node, d_data_node) == NULL)
    {
        xmlFreeNode(joint_node);
        xmlFreeNode(d_data_node);
        return NULL;
    }

    sprintf(cur_char, "%d", mds_joint_strct->is_symmmetric);
    if (xmlNewTextChild(joint_node, NULL, BAD_CAST "is_symmetric", BAD_CAST cur_char) == NULL)
    {
        xmlFreeNode(joint_node);
        return NULL;
    }
    if (xmlNewTextChild(joint_node, NULL, BAD_CAST "symmetric_joint_name", BAD_CAST mds_joint_strct->symmetric_joint_name) == NULL)
    {
        xmlFreeNode(joint_node);
        return NULL;
    }
    return joint_node;
}
xmlNodePtr MDS_create_rod_xmlNodePtr(MDS_rod_strct *mds_rod_strct)
{
    int i;
    xmlNodePtr rod_node;
    xmlNodePtr d_data_node;
    char cur_char[30];

    if ((rod_node = xmlNewNode(NULL, BAD_CAST "rod")) == NULL) {
        return NULL;
    }
    if (xmlSetProp(rod_node, BAD_CAST "name", BAD_CAST mds_rod_strct->name ) == NULL) {
        xmlFreeNode(rod_node);
        return NULL;
    }
#ifdef GEN_MDS
    mds_rod_strct->d_length->d_type = 2;
    mds_rod_strct->d_length->d_0 = mds_rod_strct->length;
    mds_rod_strct->d_length->d_opti = mds_rod_strct->length;
#endif
    d_data_node = MDS_create_d_data_xmlNodePtr(mds_rod_strct->d_length,"d_length");
    if (xmlAddChild(rod_node, d_data_node) == NULL)
    {
        xmlFreeNode(rod_node);
        xmlFreeNode(d_data_node);
        return NULL;
    }

    sprintf(cur_char, "%d", mds_rod_strct->is_symmmetric);
    if (xmlNewTextChild(rod_node, NULL, BAD_CAST "is_symmetric", BAD_CAST cur_char) == NULL)
    {
        xmlFreeNode(rod_node);
        return NULL;
    }
    if (xmlNewTextChild(rod_node, NULL, BAD_CAST "symmetric_rod_name", BAD_CAST mds_rod_strct->symmetric_rod_name) == NULL)
    {
        xmlFreeNode(rod_node);
        return NULL;
    }
    return rod_node;
}

xmlNodePtr MDS_create_parameter_xmlNodePtr(MDS_parameter_strct *mds_parameter_strct)
{
    int i;
    xmlNodePtr parameter_node;
    xmlNodePtr d_data_node;
    char cur_char[30];

    if ((parameter_node = xmlNewNode(NULL, BAD_CAST "parameter")) == NULL) {
        return NULL;
    }
    if (xmlSetProp(parameter_node, BAD_CAST "name", BAD_CAST mds_parameter_strct->name ) == NULL) {
        xmlFreeNode(parameter_node);
        return NULL;
    }

    for(i=0; i<mds_parameter_strct->n_value; i++)
    {
#ifdef GEN_MDS
        mds_parameter_strct->d_value[i]->d_type = 2;
        mds_parameter_strct->d_value[i]->d_0 = mds_parameter_strct->value_list[i];
        mds_parameter_strct->d_value[i]->d_opti = mds_parameter_strct->value_list[i];
#endif
        sprintf(cur_char, "%d",i);
        d_data_node = MDS_create_d_data_xmlNodePtr(mds_parameter_strct->d_value[i],cur_char);
        if (xmlAddChild(parameter_node, d_data_node) == NULL)
        {
            xmlFreeNode(parameter_node);
            xmlFreeNode(d_data_node);
            return NULL;
        }
    }

    sprintf(cur_char, "%d", mds_parameter_strct->is_symmmetric);
    if (xmlNewTextChild(parameter_node, NULL, BAD_CAST "is_symmetric", BAD_CAST cur_char) == NULL)
    {
        xmlFreeNode(parameter_node);
        return NULL;
    }
    if (xmlNewTextChild(parameter_node, NULL, BAD_CAST "symmetric_parameter_name", BAD_CAST mds_parameter_strct->symmetric_parameter_name) == NULL)
    {
        xmlFreeNode(parameter_node);
        return NULL;
    }
    return parameter_node;
}

xmlNodePtr MDS_create_user_model_xmlNodePtr(MDS_user_model_strct *mds_user_model_strct, xmlNodePtr old_user_model_node)
{
    int i;
    xmlNodePtr cur_node, old_node;
    xmlNodePtr  new_user_model_node;

    if ((new_user_model_node = xmlNewNode(NULL, BAD_CAST "user_model")) == NULL)
    {
        return NULL;
    }
    if (xmlSetProp(new_user_model_node, BAD_CAST "name", BAD_CAST mds_user_model_strct->name ) == NULL)
    {
        xmlFreeNode(new_user_model_node);
        return NULL;
    }

    for(i=0; i<mds_user_model_strct->n_parameter; i++)
    {
        if (old_user_model_node->children == NULL)
        {
            cur_node = MDS_create_parameter_xmlNodePtr(mds_user_model_strct->parameter_list[i]);
            xmlAddChild(new_user_model_node, cur_node);
        }
        else
        {
            old_node = MDS_find_xmlNodePtr(old_user_model_node->children, mds_user_model_strct->parameter_list[i]->name);

            cur_node = xmlCopyNode(old_node,1);

            xmlUnlinkNode(old_node);
            xmlFreeNode(old_node);

            if(cur_node == NULL)
            {
                cur_node = MDS_create_parameter_xmlNodePtr(mds_user_model_strct->parameter_list[i]);
            }
            xmlAddChild(new_user_model_node, cur_node);
        }
    }
    return new_user_model_node;
}

#ifdef GEN_MDS
int MDS_write_mds_opti_xml(MDS_gen_strct *mds_gen_strct, const char *mds_opti_xml)
#else
int MDS_update_mds_opti_xml(MDS_gen_strct *mds_gen_strct, const char *mds_opti_xml)
#endif
{
    int i;
    xmlDocPtr doc;
    xmlNodePtr root, cur_node, old_node;
    xmlNodePtr old_point_list_node, new_point_list_node;
    xmlNodePtr old_joint_list_node, new_joint_list_node;
    xmlNodePtr old_rod_list_node, new_rod_list_node;
    xmlNodePtr old_user_model_list_node, new_user_model_list_node;

    xmlKeepBlanksDefault(0);
    doc = xmlParseFile(mds_opti_xml);
    if (doc == NULL) {
        fprintf(stderr, "Document XML invalide\n");
        return EXIT_FAILURE;
    }

    root = xmlDocGetRootElement(doc);
    if (root == NULL) {
        fprintf(stderr, "Document XML vierge\n");
        xmlFreeDoc(doc);
        return EXIT_FAILURE;
    }

    if (strcmp((const char*)root->name,"mds_opti"))
    {
        fprintf(stderr, "pas bon type de xml !!!!!!!!!!!!!!!!!\n");
        xmlFreeDoc(doc);
        return EXIT_FAILURE;
    }

    cur_node = root->children;
    ////////////
    old_point_list_node = NULL;
    old_joint_list_node = NULL;
    old_rod_list_node = NULL;
    old_user_model_list_node = NULL;
    ////////////
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"point_list"))
            {
                old_point_list_node = cur_node;
            }
            if(!strcmp((const char*)cur_node->name,"joint_list"))
            {
                old_joint_list_node = cur_node;
            }
            if(!strcmp((const char*)cur_node->name,"rod_list"))
            {
                old_rod_list_node = cur_node;
            }
            if(!strcmp((const char*)cur_node->name,"user_model_list"))
            {
                old_user_model_list_node = cur_node;
            }

        }
        cur_node = cur_node->next;
    }
    ////////////
    if ((new_point_list_node = xmlNewNode(NULL, BAD_CAST "point_list")) == NULL)
    {
        return -1; //  change it
    }
    if ((new_joint_list_node = xmlNewNode(NULL, BAD_CAST "joint_list")) == NULL)
    {
        return -1; //  change it
    }
    if ((new_rod_list_node = xmlNewNode(NULL, BAD_CAST "rod_list")) == NULL)
    {
        return -1; //  change it
    }
    if ((new_user_model_list_node = xmlNewNode(NULL, BAD_CAST "user_model_list")) == NULL)
    {
        return -1; //  change it
    }
    ////////////
    for(i=0; i<mds_gen_strct->n_point; i++)
    {
#ifdef GEN_MDS
        if (old_point_list_node->children == NULL)
        {
#endif
            cur_node = MDS_create_point_xmlNodePtr(mds_gen_strct->point_list[i]);
            xmlAddChild(new_point_list_node, cur_node);
#ifdef GEN_MDS
        }
        else
        {
            old_node = MDS_find_xmlNodePtr(old_point_list_node->children, mds_gen_strct->point_list[i]->name);

            cur_node = xmlCopyNode(old_node,1);

            xmlUnlinkNode(old_node);
            xmlFreeNode(old_node);

            if(cur_node == NULL)
            {
                cur_node = MDS_create_point_xmlNodePtr(mds_gen_strct->point_list[i]);
            }
            xmlAddChild(new_point_list_node, cur_node);
        }
#endif
    }
    xmlAddChild(root,new_point_list_node);
    xmlUnlinkNode(old_point_list_node);
    xmlFreeNode(old_point_list_node);
    for(i=0; i<mds_gen_strct->bodytree->n_qc; i++)
    {
#ifdef GEN_MDS
        if (old_joint_list_node->children == NULL)
        {
#endif
            cur_node = MDS_create_joint_xmlNodePtr(mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]);
            xmlAddChild(new_joint_list_node, cur_node);
#ifdef GEN_MDS
        }
        else
        {
            old_node = MDS_find_xmlNodePtr(old_joint_list_node->children, mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->name);

            cur_node = xmlCopyNode(old_node,1);

            xmlUnlinkNode(old_node);
            xmlFreeNode(old_node);

            if(cur_node == NULL)
            {
                cur_node = MDS_create_joint_xmlNodePtr(mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]);
            }
            xmlAddChild(new_joint_list_node, cur_node);
        }
#endif
    }
    xmlAddChild(root,new_joint_list_node);
    xmlUnlinkNode(old_joint_list_node);
    xmlFreeNode(old_joint_list_node);
    for(i=0; i<mds_gen_strct->cuts->n_rod; i++)
    {
#ifdef GEN_MDS
        if (old_rod_list_node->children == NULL)
        {
#endif
            cur_node = MDS_create_rod_xmlNodePtr(mds_gen_strct->cuts->rod_list[i]);
            xmlAddChild(new_rod_list_node, cur_node);
#ifdef GEN_MDS
        }
        else
        {
            old_node = MDS_find_xmlNodePtr(old_rod_list_node->children, mds_gen_strct->cuts->rod_list[i]->name);

            cur_node = xmlCopyNode(old_node,1);

            xmlUnlinkNode(old_node);
            xmlFreeNode(old_node);

            if(cur_node == NULL)
            {
                cur_node = MDS_create_rod_xmlNodePtr(mds_gen_strct->cuts->rod_list[i]);
            }
            xmlAddChild(new_rod_list_node, cur_node);
        }
#endif
    }
    xmlAddChild(root,new_rod_list_node);
    xmlUnlinkNode(old_rod_list_node);
    xmlFreeNode(old_rod_list_node);
    for(i=0; i<mds_gen_strct->user_models->n_user_model; i++)
    {
#ifdef GEN_MDS
        if (old_user_model_list_node->children == NULL)
        {
#endif
            cur_node = MDS_create_user_model_xmlNodePtr(mds_gen_strct->user_models->user_model_list[i], xmlNewNode(NULL, BAD_CAST "empty"));
            xmlAddChild(new_user_model_list_node, cur_node);
#ifdef GEN_MDS
        }
        else
        {
            old_node = MDS_find_xmlNodePtr(old_user_model_list_node->children, mds_gen_strct->user_models->user_model_list[i]->name);

            if(old_node == NULL)
            {
                cur_node = MDS_create_user_model_xmlNodePtr(mds_gen_strct->user_models->user_model_list[i], xmlNewNode(NULL, BAD_CAST "empty"));
            }
            else
            {
                cur_node = MDS_create_user_model_xmlNodePtr(mds_gen_strct->user_models->user_model_list[i], old_node);
            }
            xmlUnlinkNode(old_node);
            xmlFreeNode(old_node);
            xmlAddChild(new_user_model_list_node, cur_node);
        }
#endif
    }
    xmlAddChild(root,new_user_model_list_node);
    xmlUnlinkNode(old_user_model_list_node);
    xmlFreeNode(old_user_model_list_node);

    ////////////

    xmlSaveFormatFile(mds_opti_xml, doc,1);
    xmlFreeDoc(doc);

    return EXIT_SUCCESS;
}

int MDS_d_data_opti_reader(xmlNodePtr in_node, MDS_d_data_strct *mds_d_data_strct)
{
    xmlNodePtr cur_node = in_node->children;
    xmlChar *elementValue;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"d_type"))
            {
                elementValue = xmlNodeListGetString(cur_node->doc, cur_node->xmlChildrenNode, 1);
                mds_d_data_strct->d_type = atoi((char*)elementValue);
            }
            else if(!strcmp((const char*)cur_node->name,"d_0"))
            {
                elementValue = xmlNodeListGetString(cur_node->doc, cur_node->xmlChildrenNode, 1);
                mds_d_data_strct->d_0 = atof((char*)elementValue);
            }
            else if(!strcmp((const char*)cur_node->name,"d_min"))
            {
                elementValue = xmlNodeListGetString(cur_node->doc, cur_node->xmlChildrenNode, 1);
                mds_d_data_strct->d_min = atof((char*)elementValue);
            }
            else if(!strcmp((const char*)cur_node->name,"d_max"))
            {
                elementValue = xmlNodeListGetString(cur_node->doc, cur_node->xmlChildrenNode, 1);
                mds_d_data_strct->d_max = atof((char*)elementValue);
            }
            else if(!strcmp((const char*)cur_node->name,"d_opti"))
            {
                elementValue = xmlNodeListGetString(cur_node->doc, cur_node->xmlChildrenNode, 1);
                mds_d_data_strct->d_opti = atof((char*)elementValue);
            }
        }
        cur_node = cur_node->next;
    }
        return 0; // Why is ther a int to return in the function signature ????

}

int MDS_point_list_opti_reader(xmlNodePtr in_node, MDS_gen_strct *mds_gen_strct)
{
    int ind = 0;
    xmlNodePtr cur_node = in_node->children;
    xmlNodePtr sub_cur_node;
    xmlChar *elementValue;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"point"))
            {
                if(ind >= mds_gen_strct->n_point)
                {
                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                    return EXIT_FAILURE;
                }
                if(!strcmp((const char*)cur_node->properties->name,"name"))
                {
                    elementValue = xmlNodeListGetString(cur_node->doc, cur_node->properties->xmlChildrenNode, 1);
                }
                if(!strcmp((const char*)elementValue,mds_gen_strct->point_list[ind]->name))
                {
                    sub_cur_node = cur_node->children;
                    while (sub_cur_node != NULL)
                    {
                        if (sub_cur_node->type == XML_ELEMENT_NODE)
                        {
                            if(!strcmp((const char*)sub_cur_node->name,"d_data"))
                            {
                                if(!strcmp((const char*)sub_cur_node->properties->name,"name"))
                                {
                                    elementValue = xmlNodeListGetString(sub_cur_node->doc, sub_cur_node->properties->xmlChildrenNode, 1);
                                }
                                if(!strcmp((const char*)elementValue,"x"))
                                {
                                    MDS_d_data_opti_reader(sub_cur_node,mds_gen_strct->point_list[ind]->d_pt[0]);
                                }
                                else if(!strcmp((const char*)elementValue,"y"))
                                {
                                    MDS_d_data_opti_reader(sub_cur_node,mds_gen_strct->point_list[ind]->d_pt[1]);
                                }
                                else if(!strcmp((const char*)elementValue,"z"))
                                {
                                    MDS_d_data_opti_reader(sub_cur_node,mds_gen_strct->point_list[ind]->d_pt[2]);
                                }
                            }
                            else if(!strcmp((const char*)sub_cur_node->name,"is_symmetric"))
                            {
                                elementValue = xmlNodeListGetString(sub_cur_node->doc, sub_cur_node->xmlChildrenNode, 1);
                                mds_gen_strct->point_list[ind]->is_symmmetric = atoi((char*)elementValue);
                            }
                            else if(!strcmp((const char*)sub_cur_node->name,"symmetric_point_name"))
                            {
                                elementValue = xmlNodeListGetString(sub_cur_node->doc, sub_cur_node->xmlChildrenNode, 1);
                                mds_gen_strct->point_list[ind]->symmetric_point_name = (char*)elementValue;
                            }
                        }
                        sub_cur_node = sub_cur_node->next;
                    }
                }
                else
                {
                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                    return EXIT_FAILURE;
                }
                ind++;
            }
        }
        cur_node = cur_node->next;
    }
    return EXIT_SUCCESS;
}

int MDS_joint_list_opti_reader(xmlNodePtr in_node, MDS_gen_strct *mds_gen_strct)
{
    int ind = 0;
    xmlNodePtr cur_node = in_node->children;
    xmlNodePtr sub_cur_node;
    xmlChar *elementValue;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"joint"))
            {
                if(ind >= mds_gen_strct->bodytree->n_qc)
                {
                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                    return EXIT_FAILURE;
                }
                if(!strcmp((const char*)cur_node->properties->name,"name"))
                {
                    elementValue = xmlNodeListGetString(cur_node->doc, cur_node->properties->xmlChildrenNode, 1);
                }
                if(!strcmp((const char*)elementValue,mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[ind]]->name))
                {
                    sub_cur_node = cur_node->children;
                    while (sub_cur_node != NULL)
                    {
                        if (sub_cur_node->type == XML_ELEMENT_NODE)
                        {
                            if(!strcmp((const char*)sub_cur_node->name,"d_data"))
                            {
                                if(!strcmp((const char*)sub_cur_node->properties->name,"name"))
                                {
                                    elementValue = xmlNodeListGetString(sub_cur_node->doc, sub_cur_node->properties->xmlChildrenNode, 1);
                                }
                                if(!strcmp((const char*)elementValue,"d_qf"))
                                {
                                    MDS_d_data_opti_reader(sub_cur_node,mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[ind]]->d_qf);
                                }
                            }
                            else if(!strcmp((const char*)sub_cur_node->name,"is_symmetric"))
                            {
                                elementValue = xmlNodeListGetString(sub_cur_node->doc, sub_cur_node->xmlChildrenNode, 1);
                                mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[ind]]->is_symmmetric = atoi((char*)elementValue);
                            }
                            else if(!strcmp((const char*)sub_cur_node->name,"symmetric_joint_name"))
                            {
                                elementValue = xmlNodeListGetString(sub_cur_node->doc, sub_cur_node->xmlChildrenNode, 1);
                                mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[ind]]->symmetric_joint_name = (char*)elementValue;
                            }
                        }
                        sub_cur_node = sub_cur_node->next;
                    }
                }
                else
                {
                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                    return EXIT_FAILURE;
                }
                ind++;
            }
        }
        cur_node = cur_node->next;
    }
    return EXIT_SUCCESS;
}

int MDS_rod_list_opti_reader(xmlNodePtr in_node, MDS_gen_strct *mds_gen_strct)
{
    int ind = 0;
    xmlNodePtr cur_node = in_node->children;
    xmlNodePtr sub_cur_node;
    xmlChar *elementValue;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"rod"))
            {
                if(ind >= mds_gen_strct->cuts->n_rod)
                {
                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                    return EXIT_FAILURE;
                }
                if(!strcmp((const char*)cur_node->properties->name,"name"))
                {
                    elementValue = xmlNodeListGetString(cur_node->doc, cur_node->properties->xmlChildrenNode, 1);
                }
                if(!strcmp((const char*)elementValue,mds_gen_strct->cuts->rod_list[ind]->name))
                {
                    sub_cur_node = cur_node->children;
                    while (sub_cur_node != NULL)
                    {
                        if (sub_cur_node->type == XML_ELEMENT_NODE)
                        {
                            if(!strcmp((const char*)sub_cur_node->name,"d_data"))
                            {
                                if(!strcmp((const char*)sub_cur_node->properties->name,"name"))
                                {
                                    elementValue = xmlNodeListGetString(sub_cur_node->doc, sub_cur_node->properties->xmlChildrenNode, 1);
                                }
                                if(!strcmp((const char*)elementValue,"d_length"))
                                {
                                    MDS_d_data_opti_reader(sub_cur_node,mds_gen_strct->cuts->rod_list[ind]->d_length);
                                }
                            }
                            else if(!strcmp((const char*)sub_cur_node->name,"is_symmetric"))
                            {
                                elementValue = xmlNodeListGetString(sub_cur_node->doc, sub_cur_node->xmlChildrenNode, 1);
                                mds_gen_strct->cuts->rod_list[ind]->is_symmmetric = atoi((char*)elementValue);
                            }
                            else if(!strcmp((const char*)sub_cur_node->name,"symmetric_rod_name"))
                            {
                                elementValue = xmlNodeListGetString(sub_cur_node->doc, sub_cur_node->xmlChildrenNode, 1);
                                mds_gen_strct->cuts->rod_list[ind]->symmetric_rod_name = (char*)elementValue;
                            }
                        }
                        sub_cur_node = sub_cur_node->next;
                    }
                }
                else
                {
                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                    return EXIT_FAILURE;
                }
                ind++;
            }
        }
        cur_node = cur_node->next;
    }
    return EXIT_SUCCESS;
}

int MDS_user_model_list_opti_reader(xmlNodePtr in_node, MDS_gen_strct *mds_gen_strct)
{
    int ind = 0;
    int ind_parameter = 0;
    int ind_value = 0;
    xmlNodePtr cur_node = in_node->children;
    xmlNodePtr sub_cur_node, sub_sub_cur_node;
    xmlChar *elementValue;
    char cur_char[30];

    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"user_model"))
            {
                if(ind >= mds_gen_strct->user_models->n_user_model)
                {
                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                    return EXIT_FAILURE;
                }
                if(!strcmp((const char*)cur_node->properties->name,"name"))
                {
                    elementValue = xmlNodeListGetString(cur_node->doc, cur_node->properties->xmlChildrenNode, 1);
                }
                if(!strcmp((const char*)elementValue,mds_gen_strct->user_models->user_model_list[ind]->name))
                {
                    sub_cur_node = cur_node->children;
                    ind_parameter =0;
                    ////////////////////////////////////////////////////////////////////////////////////
                    while (sub_cur_node != NULL)
                    {
                        if (sub_cur_node->type == XML_ELEMENT_NODE)
                        {
                            if(!strcmp((const char*)sub_cur_node->name,"parameter"))
                            {
                                if(ind_parameter >= mds_gen_strct->user_models->user_model_list[ind]->n_parameter)
                                {
                                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                                    return EXIT_FAILURE;
                                }
                                if(!strcmp((const char*)sub_cur_node->properties->name,"name"))
                                {
                                    elementValue = xmlNodeListGetString(sub_cur_node->doc, sub_cur_node->properties->xmlChildrenNode, 1);
                                }
                                if(!strcmp((const char*)elementValue,mds_gen_strct->user_models->user_model_list[ind]->parameter_list[ind_parameter]->name))
                                {
                                    sub_sub_cur_node = sub_cur_node->children;
                                    ind_value = 0;
                                    ///////////////////////////////
                                    while (sub_sub_cur_node != NULL)
                                    {
                                        if (sub_sub_cur_node->type == XML_ELEMENT_NODE)
                                        {
                                            if(!strcmp((const char*)sub_sub_cur_node->name,"d_data"))
                                            {
                                                if(ind_value >= mds_gen_strct->user_models->user_model_list[ind]->parameter_list[ind_parameter]->n_value)
                                                {
                                                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                                                    return EXIT_FAILURE;
                                                }
                                                if(!strcmp((const char*)sub_sub_cur_node->properties->name,"name"))
                                                {
                                                    elementValue = xmlNodeListGetString(sub_sub_cur_node->doc, sub_sub_cur_node->properties->xmlChildrenNode, 1);
                                                }
                                                sprintf(cur_char, "%d", ind_value);
                                                if(!strcmp((const char*)elementValue,cur_char))
                                                {
                                                    MDS_d_data_opti_reader(sub_sub_cur_node,mds_gen_strct->user_models->user_model_list[ind]->parameter_list[ind_parameter]->d_value[ind_value]);
                                                }
                                                else
                                                {
                                                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                                                    return EXIT_FAILURE;
                                                }
                                                ind_value++;
                                            }
                                            else if(!strcmp((const char*)sub_sub_cur_node->name,"is_symmetric"))
                                            {
                                                elementValue = xmlNodeListGetString(sub_sub_cur_node->doc, sub_sub_cur_node->xmlChildrenNode, 1);
                                                mds_gen_strct->user_models->user_model_list[ind]->parameter_list[ind_parameter]->is_symmmetric = atoi((char*)elementValue);
                                            }
                                            else if(!strcmp((const char*)sub_sub_cur_node->name,"symmetric_parameter_name"))
                                            {
                                                elementValue = xmlNodeListGetString(sub_sub_cur_node->doc, sub_sub_cur_node->xmlChildrenNode, 1);
                                                mds_gen_strct->user_models->user_model_list[ind]->parameter_list[ind_parameter]->symmetric_parameter_name = (char*)elementValue;
                                            }
                                        }
                                        sub_sub_cur_node = sub_sub_cur_node->next;
                                    }
                                    ////////////////////////////////////////////////
                                }
                                else
                                {
                                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                                    return EXIT_FAILURE;
                                }
                                ind_parameter++;
                            }
                        }
                        sub_cur_node = sub_cur_node->next;
                    }
                }
                else
                {
                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                    return EXIT_FAILURE;
                }
                ind++;
            }
        }
        cur_node = cur_node->next;
    }
    return EXIT_SUCCESS;
}

int MDS_read_mds_opti_xml(MDS_gen_strct *mds_gen_strct, const char *mds_opti_xml)
{
    int i;
    xmlDocPtr doc;
    xmlNodePtr root, cur_node, old_node;
    xmlNodePtr old_point_list_node, new_point_list_node;
    xmlNodePtr old_joint_list_node, new_joint_list_node;
    xmlNodePtr old_rod_list_node, new_rod_list_node;
    xmlNodePtr old_user_model_list_node, new_user_model_list_node;

    MDS_d_data_strct *mds_d_data_strct;

    xmlKeepBlanksDefault(0);
    doc = xmlParseFile(mds_opti_xml);
    if (doc == NULL) {
        fprintf(stderr, "Document XML invalide\n");
        return EXIT_FAILURE;
    }

    root = xmlDocGetRootElement(doc);
    if (root == NULL) {
        fprintf(stderr, "Document XML vierge\n");
        xmlFreeDoc(doc);
        return EXIT_FAILURE;
    }

    if (strcmp((const char*)root->name,"mds_opti"))
    {
        fprintf(stderr, "pas bon type de xml !!!!!!!!!!!!!!!!!\n");
        xmlFreeDoc(doc);
        return EXIT_FAILURE;
    }

    cur_node = root->children;
    while (cur_node != NULL)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if(!strcmp((const char*)cur_node->name,"point_list"))
            {
                MDS_point_list_opti_reader(cur_node, mds_gen_strct );
            }
            else if(!strcmp((const char*)cur_node->name,"joint_list"))
            {
                MDS_joint_list_opti_reader(cur_node, mds_gen_strct );
            }
            else if(!strcmp((const char*)cur_node->name,"rod_list"))
            {
                MDS_rod_list_opti_reader(cur_node, mds_gen_strct );
            }
            else if(!strcmp((const char*)cur_node->name,"user_model_list"))
            {
                MDS_user_model_list_opti_reader(cur_node, mds_gen_strct );
            }
        }
        cur_node = cur_node->next;
    }
    xmlFreeDoc(doc);
    return EXIT_SUCCESS;
}


int MDS_opti_structurer(MDS_gen_strct* mds_gen_strct)
{
    int i,j,k, ind_find;
    int ind_d_simplified = 0, ind_d_forced = 0, ind_d_optimized = 0, ind_d_resulting = 0, ind_d_related = 0;

    mds_gen_strct->opti = init_MDS_opti_strct();

    for(i=0; i<mds_gen_strct->n_point; i++)
    {

        if(mds_gen_strct->point_list[i]->is_symmmetric == 2)
        {
            ind_find = 0;
            while(strcmp(mds_gen_strct->point_list[ind_find]->name,mds_gen_strct->point_list[i]->symmetric_point_name))
            {
                ind_find++;
                if(ind_find >= mds_gen_strct->n_point)
                {
                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                    printf("point = %s\n ",mds_gen_strct->point_list[i]->symmetric_point_name);
                    return EXIT_FAILURE;
                }
            }
            for(j=0; j<3; j++)
            {
                if (mds_gen_strct->point_list[ind_find]->d_pt[j]->d_type == 1)
                {
                    mds_gen_strct->point_list[i]->d_pt[j]->d_type = 1;
                }
                else
                {
                    mds_gen_strct->point_list[i]->d_pt[j]->d_type = 5;
                }
                mds_gen_strct->point_list[i]->d_pt[j]->master_d_ptr = mds_gen_strct->point_list[ind_find]->d_pt[j];
            }
            mds_gen_strct->point_list[i]->d_pt[0]->is_related = 2;
            mds_gen_strct->point_list[i]->d_pt[1]->is_related = 3;
            mds_gen_strct->point_list[i]->d_pt[2]->is_related = 2;
        }
        for(j=0; j<3; j++)
        {
            mds_gen_strct->point_list[i]->d_pt[j]->d_ptr = &mds_gen_strct->point_list[i]->pt[j];
            switch(mds_gen_strct->point_list[i]->d_pt[j]->d_type)
            {
                case 1:
                    (mds_gen_strct->opti->n_d_simplified)++;
                break;
                case 2:
                    (mds_gen_strct->opti->n_d_forced)++;
                break;
                case 3:
                    (mds_gen_strct->opti->n_d_optimized)++;
                break;
                case 4:
                    (mds_gen_strct->opti->n_d_resulting)++;
                break;
                case 5:
                    (mds_gen_strct->opti->n_d_related)++;
                break;
            }
        }
    }
    for(i=0; i<mds_gen_strct->bodytree->n_qc; i++)
    {
        if(mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->is_symmmetric == 2)
        {
            ind_find = 0;
            while(strcmp(mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[ind_find]]->name,mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->symmetric_joint_name))
            {
                ind_find++;
                if(ind_find >= mds_gen_strct->bodytree->n_qc)
                {
                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                    printf("qc");
                    return EXIT_FAILURE;
                }
            }
            if (mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[ind_find]]->d_qf->d_type == 1)
            {
                mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->d_qf->d_type = 1;
            }
            else
            {
                mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->d_qf->d_type = 5;
            }
            mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->d_qf->master_d_ptr = mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[ind_find]]->d_qf;
            switch(mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->type)
            {
                case 1:
                case 3:
                case 5:
                    mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->d_qf->is_related = 2;
                break;
                case 2:
                case 4:
                case 6:
                    mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->d_qf->is_related = 3;
                break;
            }
        }
        mds_gen_strct->bodytree->joint_list[i]->d_qf->d_ptr = &mds_gen_strct->bodytree->joint_list[i]->q0;
        switch(mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->d_qf->d_type)
        {
            case 1:
                (mds_gen_strct->opti->n_d_simplified)++;
            break;
            case 2:
                (mds_gen_strct->opti->n_d_forced)++;
            break;
            case 3:
                (mds_gen_strct->opti->n_d_optimized)++;
            break;
            case 4:
                (mds_gen_strct->opti->n_d_resulting)++;
            break;
            case 5:
                (mds_gen_strct->opti->n_d_related)++;
            break;
        }
    }
    for(i=0; i<mds_gen_strct->cuts->n_rod; i++)
    {
        if(mds_gen_strct->cuts->rod_list[i]->is_symmmetric == 2)
        {
            ind_find = 0;
            while(strcmp(mds_gen_strct->cuts->rod_list[ind_find]->name,mds_gen_strct->cuts->rod_list[i]->symmetric_rod_name))
            {
                ind_find++;
                if(ind_find >= mds_gen_strct->cuts->n_rod)
                {
                    printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                    printf("rod");
                    return EXIT_FAILURE;
                }
            }
            if (mds_gen_strct->cuts->rod_list[ind_find]->d_length->d_type == 1)
            {
                mds_gen_strct->cuts->rod_list[i]->d_length->d_type = 1;
            }
            else
            {
                mds_gen_strct->cuts->rod_list[i]->d_length->d_type = 5;
            }
            mds_gen_strct->cuts->rod_list[i]->d_length->master_d_ptr = mds_gen_strct->cuts->rod_list[ind_find]->d_length;
            mds_gen_strct->cuts->rod_list[i]->d_length->is_related = 2;
        }
        mds_gen_strct->cuts->rod_list[i]->d_length->d_ptr = &mds_gen_strct->cuts->rod_list[i]->length;
        switch(mds_gen_strct->cuts->rod_list[i]->d_length->d_type)
        {
            case 1:
                (mds_gen_strct->opti->n_d_simplified)++;
            break;
            case 2:
                (mds_gen_strct->opti->n_d_forced)++;
            break;
            case 3:
                (mds_gen_strct->opti->n_d_optimized)++;
            break;
            case 4:
                (mds_gen_strct->opti->n_d_resulting)++;
            break;
            case 5:
                (mds_gen_strct->opti->n_d_related)++;
            break;
        }
    }

    for(i=0; i<mds_gen_strct->user_models->n_user_model; i++)
    {
        for(j=0; j<mds_gen_strct->user_models->user_model_list[i]->n_parameter; j++)
        {
            if(mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->is_symmmetric == 2)
            {
                ind_find = 0;
                while(strcmp(mds_gen_strct->user_models->user_model_list[i]->parameter_list[ind_find]->name,mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->symmetric_parameter_name))
                {
                    ind_find++;
                    if(ind_find >= mds_gen_strct->user_models->user_model_list[i]->n_parameter)
                    {
                        printf("mds_opti_xml don't correspond to the mbs file!!!\n");
                        printf(" user");
                        return EXIT_FAILURE;
                    }
                }
                for(k=0; k<mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->n_value; k++)
                {
                    if (mds_gen_strct->user_models->user_model_list[i]->parameter_list[ind_find]->d_value[k]->d_type == 1)
                    {
                        mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->d_value[k]->d_type = 1;
                    }
                    else
                    {
                        mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->d_value[k]->d_type = 5;
                    }
                    mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->d_value[k]->master_d_ptr = mds_gen_strct->user_models->user_model_list[i]->parameter_list[ind_find]->d_value[k];
                    mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->d_value[k]->is_related = 2;
                }
            }
            for(k=0; k<mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->n_value; k++)
            {
                mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->d_value[k]->d_ptr = &mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->value_list[k];
                switch(mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->d_value[k]->d_type)
                {
                    case 1:
                        (mds_gen_strct->opti->n_d_simplified)++;
                    break;
                    case 2:
                        (mds_gen_strct->opti->n_d_forced)++;
                    break;
                    case 3:
                        (mds_gen_strct->opti->n_d_optimized)++;
                    break;
                    case 4:
                        (mds_gen_strct->opti->n_d_resulting)++;
                    break;
                    case 5:
                        (mds_gen_strct->opti->n_d_related)++;
                    break;
                }
            }
        }
    }
    mds_gen_strct->opti->d_simplified_list = (MDS_d_data_strct**) malloc(mds_gen_strct->opti->n_d_simplified*sizeof(MDS_d_data_strct*));
    mds_gen_strct->opti->d_forced_list = (MDS_d_data_strct**) malloc(mds_gen_strct->opti->n_d_forced*sizeof(MDS_d_data_strct*));
    mds_gen_strct->opti->d_optimized_list = (MDS_d_data_strct**) malloc(mds_gen_strct->opti->n_d_optimized*sizeof(MDS_d_data_strct*));
    mds_gen_strct->opti->d_resulting_list = (MDS_d_data_strct**) malloc(mds_gen_strct->opti->n_d_resulting*sizeof(MDS_d_data_strct*));
    mds_gen_strct->opti->d_related_list = (MDS_d_data_strct**) malloc(mds_gen_strct->opti->n_d_related*sizeof(MDS_d_data_strct*));

    for(i=0; i<mds_gen_strct->n_point; i++)
    {
        for(j=0; j<3; j++)
        {
            switch(mds_gen_strct->point_list[i]->d_pt[j]->d_type)
            {
                case 1:
                    mds_gen_strct->opti->d_simplified_list[ind_d_simplified] = mds_gen_strct->point_list[i]->d_pt[j];
                    ind_d_simplified++;
                break;
                case 2:
                    mds_gen_strct->opti->d_forced_list[ind_d_forced] = mds_gen_strct->point_list[i]->d_pt[j];
                    ind_d_forced++;
                break;
                case 3:
                    mds_gen_strct->opti->d_optimized_list[ind_d_optimized] = mds_gen_strct->point_list[i]->d_pt[j];
                    ind_d_optimized++;
                break;
                case 4:
                    mds_gen_strct->opti->d_resulting_list[ind_d_resulting] = mds_gen_strct->point_list[i]->d_pt[j];
                    ind_d_resulting++;
                break;
                case 5:
                    mds_gen_strct->opti->d_related_list[ind_d_related] = mds_gen_strct->point_list[i]->d_pt[j];
                    ind_d_related++;
                break;
            }
        }
    }
    for(i=0; i<mds_gen_strct->bodytree->n_qc; i++)
    {
        switch(mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->d_qf->d_type)
        {
            case 1:
                mds_gen_strct->opti->d_simplified_list[ind_d_simplified] = mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->d_qf;
                ind_d_simplified++;
            break;
            case 2:
                mds_gen_strct->opti->d_forced_list[ind_d_forced] = mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->d_qf;
                ind_d_forced++;
            break;
            case 3:
                mds_gen_strct->opti->d_optimized_list[ind_d_optimized] = mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->d_qf;
                ind_d_optimized++;
            break;
            case 4:
                mds_gen_strct->opti->d_resulting_list[ind_d_resulting] = mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->d_qf;
                ind_d_resulting++;
            break;
            case 5:
                mds_gen_strct->opti->d_related_list[ind_d_related] = mds_gen_strct->bodytree->joint_list[mds_gen_strct->bodytree->qc[i]]->d_qf;
                ind_d_related++;
            break;
        }
    }
    for(i=0; i<mds_gen_strct->cuts->n_rod; i++)
    {
        switch(mds_gen_strct->cuts->rod_list[i]->d_length->d_type)
        {
            case 1:
                mds_gen_strct->opti->d_simplified_list[ind_d_simplified] = mds_gen_strct->cuts->rod_list[i]->d_length;
                ind_d_simplified++;
            break;
            case 2:
                mds_gen_strct->opti->d_forced_list[ind_d_forced] = mds_gen_strct->cuts->rod_list[i]->d_length;
                ind_d_forced++;
            break;
            case 3:
                mds_gen_strct->opti->d_optimized_list[ind_d_optimized] = mds_gen_strct->cuts->rod_list[i]->d_length;
                ind_d_optimized++;
            break;
            case 4:
                mds_gen_strct->opti->d_resulting_list[ind_d_resulting] = mds_gen_strct->cuts->rod_list[i]->d_length;
                ind_d_resulting++;
            break;
            case 5:
                mds_gen_strct->opti->d_related_list[ind_d_related] = mds_gen_strct->cuts->rod_list[i]->d_length;
                ind_d_related++;
            break;
        }
    }

    for(i=0; i<mds_gen_strct->user_models->n_user_model; i++)
    {
        for(j=0; j<mds_gen_strct->user_models->user_model_list[i]->n_parameter; j++)
        {
            for(k=0; k<mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->n_value; k++)
            {
                switch(mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->d_value[k]->d_type)
                {
                    case 1:
                        mds_gen_strct->opti->d_simplified_list[ind_d_simplified] = mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->d_value[k];
                        ind_d_simplified++;
                    break;
                    case 2:
                        mds_gen_strct->opti->d_forced_list[ind_d_forced] = mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->d_value[k];
                        ind_d_forced++;
                    break;
                    case 3:
                        mds_gen_strct->opti->d_optimized_list[ind_d_optimized] = mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->d_value[k];
                        ind_d_optimized++;
                    break;
                    case 4:
                        mds_gen_strct->opti->d_resulting_list[ind_d_resulting] = mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->d_value[k];
                        ind_d_resulting++;
                    break;
                    case 5:
                        mds_gen_strct->opti->d_related_list[ind_d_related] = mds_gen_strct->user_models->user_model_list[i]->parameter_list[j]->d_value[k];
                        ind_d_related++;
                    break;
                }
            }
        }
    }
    return EXIT_SUCCESS;
}

int MDS_opti_relate_data(MDS_gen_strct* mds_gen_strct)
{
    int i;
    for(i=0; i<mds_gen_strct->opti->n_d_related; i++)
    {
        switch(mds_gen_strct->opti->d_related_list[i]->is_related)
        {
            case 2:
                mds_gen_strct->opti->d_related_list[i]->MBSdata_d_ptr[0] = mds_gen_strct->opti->d_related_list[i]->master_d_ptr->MBSdata_d_ptr[0];
            break;
            case 3:
                mds_gen_strct->opti->d_related_list[i]->MBSdata_d_ptr[0] =  - mds_gen_strct->opti->d_related_list[i]->master_d_ptr->MBSdata_d_ptr[0];
            break;
        }
    }
    return 0; // Why is ther a int to return in the function signature ????
}

int MDS_opti_slave_set_data(MDS_gen_strct *mds_gen_strct, double *optiNorms)
{
    int i;
    for(i=0; i<mds_gen_strct->opti->n_d_optimized; i++)
    {
        mds_gen_strct->opti->d_optimized_list[i]->MBSdata_d_ptr[0] = (mds_gen_strct->opti->d_optimized_list[i]->d_max - mds_gen_strct->opti->d_optimized_list[i]->d_min) * optiNorms[i] + mds_gen_strct->opti->d_optimized_list[i]->d_min;
    }
    return EXIT_SUCCESS;
}
int MDS_opti_test_set_data(MDS_gen_strct *mds_gen_strct)
{
    int i;
    for(i=0; i<mds_gen_strct->opti->n_d_optimized; i++)
    {
        mds_gen_strct->opti->d_optimized_list[i]->MBSdata_d_ptr[0] = mds_gen_strct->opti->d_optimized_list[i]->d_opti;
    }
    return EXIT_SUCCESS;
}
