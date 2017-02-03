#include "MbsInline.hh"
#include <iostream>
#include <sstream>

// OBJ
#include "tiny_obj_loader.h"

// VRML
#include "VrmlManualParser.hh"

namespace OpenGLMbs{

/// check if integer is in range
inline bool int_in_range(int x, int min, int max) { return (x < min) ? false : (x > max ? false : true); }

/*! \brief constructor
 *
 * \param[in] cad_file CAD file to read (with path)
 * \param[in] vertex_file vertex shader file to load (with path)
 * \param[in] fragment_file fragment shader file to load (with path)
 * \param[in] world_3d 3D world
 * \param[in] transparency transparency value
 * \param[in] trans_pos position transformation to apply
 * \param[in] trans_scale scaling transformation to apply
 * \param[in] trans_rot rotation transformation to apply
 * \param[in] name name of the shape
 */
MbsInline::MbsInline(std::string cad_file,
    std::string vertex_file, std::string fragment_file, MbsWorld3D *world_3d,
    float transparency, glm::vec3 const& trans_pos, glm::vec3 const& trans_scale, glm::vec3 const& trans_rot,
    const char* name):
    MbsShape3D(vertex_file, fragment_file, world_3d, transparency, trans_pos, trans_scale, trans_rot, name)
{
    bool load_success;
    size_t pos;
    std::string dir_cad, ext_cad;

    load_success = false;

    // find directory
    pos = cad_file.find_last_of("\\/");

    dir_cad = (std::string::npos == pos) ? "" : cad_file.substr(0, pos) + "/";

    // get extension
    std::stringstream ss_cad(cad_file);

    while (getline(ss_cad, ext_cad, '.'));

    // OBJ file
    if (!ext_cad.compare("obj"))
    {
        load_success = LoadOBJ(cad_file, dir_cad);
    }
    // VRML file
    else if (!ext_cad.compare("wrl"))
    {
        VrmlManualParser manual;
        load_success = manual.LoadVRML(cad_file, temp_vertices, temp_colors);
    }
    else // extension not known
    {
        SetNbTriangles(0);
        Init();

        std::cout << "CAD file loading warning: " << cad_file << " extension is not known !" << std::endl;
        return;
    }

    // set the number of triangles in case loading is successful
    if (load_success)
    {
        SetNbTriangles(temp_vertices.size() / 3);
    }
    else
    {
        std::cout << "CAD file loading warning: could not load the file " << cad_file << " !" << std::endl;
        SetNbTriangles(0);
    }

    Init();
}

/*! \brief destructor
 */
MbsInline::~MbsInline()
{

}

/*! \brief load a .obj file (interface)
 * 
 * \param[in] cad_file CAD file to read (with path)
 * \param[in] dir_cad directory (for .obj and .mtl)
 * \return true if success, false otherwise
 */
bool MbsInline::LoadOBJ(std::string cad_file, std::string dir_cad)
{
    // variables declaration
    int nb_vertices, mat_id;
    bool check;
    size_t id_off, fnum;
    int id_vec[3];

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    tinyobj::index_t idx;

    // load obj
    check = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, cad_file.c_str(), dir_cad.c_str());

    // print error message if any
    if (!err.empty())
    {
        std::cout << "MbsInline warning: " << err << std::endl;
    }

    // check if parsing succeeded
    if (!check)
    {
        std::cout << "MbsInline warning: failed to load/parse .obj !" << std::endl;
        return false;
    }

    // number of vertices
    nb_vertices = attrib.vertices.size() / 3;

    // loop on all the shapes
    for(unsigned int i=0; i<shapes.size(); i++)
    {
        id_off = 0;

        // loop on all the faces (should be triangles)
        for(unsigned int j=0; j<shapes[i].mesh.num_face_vertices.size(); j++)
        {
            // number of vertices per shape
            fnum = shapes[i].mesh.num_face_vertices[j];

            // material ID
            mat_id = shapes[i].mesh.material_ids[j];

            // safety
            if (fnum != 3)
            {
                std::cout << "MbsInline warning: expected 3 vertices per face !" << std::endl;
                temp_vertices.clear();
                temp_colors.clear();
                return false;
            }

            // loop on all the vertices
            for(int k=0; k<3; k++)
            {
                idx = shapes[i].mesh.indices[id_off + k];

                // indexes
                id_vec[k] = idx.vertex_index;

                // safety
                if (!int_in_range(id_vec[k], 0, nb_vertices-1))
                {
                    std::cout << "MbsInline warning: vertice number " << id_vec[k] << " does not exist !" << std::endl;
                    temp_vertices.clear();
                    temp_colors.clear();
                    return false;
                }

                // vertices
                temp_vertices.push_back(glm::vec3(
                    attrib.vertices[3*id_vec[k]+0],
                    attrib.vertices[3*id_vec[k]+1],
                    attrib.vertices[3*id_vec[k]+2]));

                // colors
                if (mat_id >= 0)
                {
                    temp_colors.push_back(glm::vec3(
                        materials[mat_id].diffuse[0],
                        materials[mat_id].diffuse[1],
                        materials[mat_id].diffuse[2]));
                }
                else
                {
                    temp_colors.push_back(glm::vec3(0.0f));
                }
            }

            id_off += fnum;
        }
    }

    // loading succeeded
    return true;
}

}
