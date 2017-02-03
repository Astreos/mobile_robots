#ifdef OPEN_GL

#include "VrmlManualParser.hh"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

namespace OpenGLMbs{

/// check if integer is in range
inline bool int_in_range(int x, int min, int max) { return (x < min) ? false : (x > max ? false : true); }

/*! \brief constructor
 */
VrmlManualParser::VrmlManualParser()
{

}

/*! \brief destructor
 */
VrmlManualParser::~VrmlManualParser()
{
    
}

/*! \brief check if string is an integer
 * 
 * \param[in] cur_str string to check
 * \return true if integer, false otherwise
 */
bool VrmlManualParser::IsInt(std::string cur_str)
{
    int i;
    std::istringstream iss(cur_str);

    iss >> std::noskipws >> i;

    return !iss.fail(); 
}

/*! \brief check if string is a float
 * 
 * \param[in] cur_str string to check
 * \return true if float, false otherwise
 */
bool VrmlManualParser::IsFloat(std::string cur_str)
{
    float f;
    std::istringstream iss(cur_str);

    iss >> std::noskipws >> f;

    return !iss.fail(); 
}

/*! \brief convert a string to an integer
 * 
 * \param[in] cur_str string to convert
 * \return converted integer
 */
int VrmlManualParser::StringToInt(std::string cur_str)
{
    int i;
    std::istringstream iss(cur_str);

    iss >> std::noskipws >> i;

    return i; 
}

/*! \brief convert a string to a float
 * 
 * \param[in] cur_str string to convert
 * \return converted float
 */
float VrmlManualParser::StringToFloat(std::string cur_str)
{
    float f;
    std::istringstream iss(cur_str);

    iss >> std::noskipws >> f;

    return f;
}

/*! \brief process a line
 * 
 * \param[in] cur_str string to read
 * \param[in] flag_int 1 if integer, 0 otherwise
 * \param[out] out_int list of integers extracted
 * \param[out] out_float list of flaots extracted
 * \return true if success, false otherwise
 */
bool VrmlManualParser::LineProcess(std::string cur_str, int flag_int,
    std::vector<int> &out_int, std::vector<double> &out_float)
{
    bool found;
    std::size_t cur_pos;
    std::string cur_part;

    std::stringstream ss_out(cur_str);

    found = false;

    while (getline(ss_out, cur_part, ' '))
    {
        cur_pos = cur_part.find(",");

        if (cur_pos != std::string::npos)
        {
            cur_part.erase(cur_pos, 1);
        }

        if (flag_int)
        {
            if (IsInt(cur_part))
            {
                out_int.push_back(StringToInt(cur_part));
                found = true;
            }
        }
        else
        {
            if (IsFloat(cur_part))
            {
                out_float.push_back(StringToFloat(cur_part));
                found = true;
            }
        }
    }

    return found;
}

/*! \brief process the CAD file up to a marker (for a line)
 * 
 * \param[in] cad_file CAD file to analyze
 * \param[in] marker marker to find
 * \param[in] shape_id ID of the shape
 * \param[in] flag_int 1 if integer, 0 otherwise
 * \param[out] out_int list of integers extracted
 * \param[out] out_float list of flaots extracted
 * \return true if success, false otherwise
 */
bool VrmlManualParser::MarkerLineVRML(std::string cad_file, std::string marker, int shape_id, int flag_int,
    std::vector<int> &out_int, std::vector<double> &out_float)
{
    int shape_count;
    bool found;
    std::size_t cur_pos, marker_len;

    std::string cur_line;
    std::ifstream f(cad_file.c_str());

    marker_len = std::string(marker).length();

    found = false;

    shape_count = -1;

    while(getline(f, cur_line) && shape_count <= shape_id)
    {
        if (cur_line.find("Shape{") != std::string::npos || cur_line.find("Shape {") != std::string::npos)
        {
            shape_count++;
        }

        if (shape_count < shape_id)
        {
            continue;
        }
        else if (shape_count > shape_id)
        {
            break;
        }

        cur_pos = cur_line.find(marker);

        if (cur_pos != std::string::npos)
        {
            cur_line.erase(0, cur_pos + marker_len);

            found = LineProcess(cur_line, flag_int, out_int, out_float);
        }
    }

    f.close();

    return found;
}

/*! \brief process the CAD file up to a marker
 * 
 * \param[in] cad_file CAD file to analyze
 * \param[in] marker marker to find
 * \param[in] shape_id ID of the shape
 * \param[in] flag_int 1 if integer, 0 otherwise
 * \param[out] out_int list of integers extracted
 * \param[out] out_float list of flaots extracted
 * \return true if success, false otherwise
 */
bool VrmlManualParser::MarkerReadVRML(std::string cad_file, std::string marker, int shape_id, int flag_int,
    std::vector<int> &out_int, std::vector<double> &out_float)
{
    // variables declaration
    int state, shape_count;
    std::size_t cur_pos, marker_len;

    std::string cur_line;
    std::string out_str;

    std::ifstream f(cad_file.c_str());

    marker_len = std::string(marker).length();

    shape_count = -1;
    state = 0;

    while(getline(f, cur_line) && state < 3 && shape_count <= shape_id)
    {
        if (cur_line.find("Shape{") != std::string::npos || cur_line.find("Shape {") != std::string::npos)
        {
            shape_count++;
        }

        if (shape_count < shape_id)
        {
            continue;
        }
        else if (shape_count > shape_id)
        {
            break;
        }

        switch (state)
        {
            case 0:
                cur_pos = cur_line.find(marker);

                if (cur_pos != std::string::npos)
                {
                    cur_line.erase(0, cur_pos + marker_len);
                    state++;

                    cur_pos = cur_line.find("[");

                    if (cur_pos != std::string::npos)
                    {
                        cur_line.erase(0, cur_pos + 1);
                        state++;

                        cur_pos = cur_line.find("]");

                        if (cur_pos != std::string::npos)
                        {
                            cur_line.erase(cur_pos, 1);
                            state++;
                        }

                        out_str += cur_line + " ";
                    }
                }
                break;

            case 1:
                cur_pos = cur_line.find("[");

                if (cur_pos != std::string::npos)
                {
                    cur_line.erase(0, cur_pos + 1);
                    state++;

                    cur_pos = cur_line.find("]");

                    if (cur_pos != std::string::npos)
                    {
                        cur_line.erase(cur_pos, 1);
                        state++;
                    }

                    out_str += cur_line + " ";
                }
                break;

            case 2:
                cur_pos = cur_line.find("]");

                if (cur_pos != std::string::npos)
                {
                    cur_line.erase(cur_pos, 1);
                    state++;
                }

                out_str += cur_line + " ";;
                break;
        
            default:
                break;
        }       
    }

    f.close();

    if (state < 3)
    {
        return false;
    }

    return LineProcess(out_str, flag_int, out_int, out_float);
}

/*! \brief get the number of shapes in the VRML
 * 
 * \param[in] cad_file CAD file to read
 * \return number of shapes
 */
int VrmlManualParser::NumberShapesVRML(std::string cad_file)
{
    int nb_shapes;
    std::string cur_line;

    std::ifstream f(cad_file.c_str());

    nb_shapes = 0;

    while(getline(f, cur_line))
    {
        if (cur_line.find("Shape{") != std::string::npos || cur_line.find("Shape {") != std::string::npos)
        {
            nb_shapes++;
        }
    }

    f.close();

    return nb_shapes;
}

/*! \brief load a single shape
 * 
 * \param[in] cad_file CAD file to read
 * \param[in] shape_id ID of the shape
 * \param[out] vertices extracted vertices
 * \param[out] colors extracted colors
 * \return true if success, false otherwise
 */
bool VrmlManualParser::LoadSingleShapeVRML(std::string cad_file, int shape_id,
    std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &colors)
{
    int nb_vert, nb_triangles, nb_colors;
    int i0, i1, i2, ic;
    int color_id_size;

    std::vector<int> int_void;
    std::vector<int> coord_id_vec;
    std::vector<int> color_id_vec;

    std::vector<double> float_void;
    std::vector<double> vertices_vec;
    std::vector<double> color_vec;

    // vertices
    if (! MarkerReadVRML(cad_file, "point", shape_id, 0, int_void, vertices_vec))
    {
        std::cout << "VRML loading warning: [point] marker problem." << std::endl;
        return false;
    }

    if (vertices_vec.size() % 3 != 0)
    {
        std::cout << "VRML loading warning: [point] marker wrong size: " << vertices_vec.size() << "." << std::endl;
        return false;
    }

    nb_vert = vertices_vec.size() / 3;

    // coordinate IDs
    if (! MarkerReadVRML(cad_file, "coordIndex", shape_id, 1, coord_id_vec, float_void))
    {
        std::cout << "VRML loading warning: [coordIndex] marker problem." << std::endl;
        return false;
    }

    if (coord_id_vec.size() % 4 != 0)
    {
        std::cout << "VRML loading warning: [coordIndex] marker wrong size: " << coord_id_vec.size() << "." << std::endl;
        return false;
    }

    nb_triangles = coord_id_vec.size() / 4;

    // colors
    color_id_size = 0;
    nb_colors = 1;

    MarkerReadVRML(cad_file, "color", shape_id, 0, int_void, color_vec);

    if (color_vec.size() > 0 && color_vec.size() % 3 == 0)
    {
        MarkerReadVRML(cad_file, "colorIndex", shape_id, 1, color_id_vec, float_void);

        color_id_size = color_id_vec.size();
        nb_colors = color_vec.size() / 3;

        if (color_id_size != nb_triangles)
        {
            std::cout << "VRML loading warning: \"colorIndex\" has a size of " << color_id_size
                << " while there are " << nb_triangles << " triangles." << std::endl;
            return false;
        }
    }
    else
    {
        color_vec.clear();

        MarkerLineVRML(cad_file, "diffuseColor", shape_id, 0, int_void, color_vec);

        if (color_vec.size() != 3)
        {
            std::cout << "VRML loading warning: cad file " << cad_file << ", shape " << shape_id << ":" << std::endl;
            std::cout << "Reading of only one color (\"color\"/\"diffuseColor\") not successful." << std::endl;
            color_vec.clear();

            for(int i=0; i<3; i++)
            {
                color_vec.push_back(0.5);
            }
        }
    }

    for(int i=0; i<nb_triangles; i++)
    {
        i0 = coord_id_vec[4*i];
        i1 = coord_id_vec[4*i+1];
        i2 = coord_id_vec[4*i+2];

        if (!int_in_range(i0, 0, nb_vert-1) || !int_in_range(i1, 0, nb_vert-1) || !int_in_range(i2, 0, nb_vert-1))
        {
            std::cout << "VRML loading warning: coordinate vertex error." << std::endl;
            return false;
        }

        vertices.push_back(glm::vec3(vertices_vec[3*i0], vertices_vec[3*i0+1], vertices_vec[3*i0+2]));
        vertices.push_back(glm::vec3(vertices_vec[3*i1], vertices_vec[3*i1+1], vertices_vec[3*i1+2]));
        vertices.push_back(glm::vec3(vertices_vec[3*i2], vertices_vec[3*i2+1], vertices_vec[3*i2+2]));

        for(int j=0; j<3; j++)
        {
            if (color_id_size > 0)
            {
                ic = color_id_vec[i];

                if (!int_in_range(ic, 0, nb_colors-1))
                {
                    std::cout << "VRML loading warning: coordinate color error." << std::endl;
                    return false;
                }
                colors.push_back(glm::vec3(color_vec[3*ic], color_vec[3*ic+1], color_vec[3*ic+2]));
            }
            else
            {
                colors.push_back(glm::vec3(color_vec[0], color_vec[1], color_vec[2]));
            }
        }
    }

    return true;
}

/*! \brief load all the shapes
 * 
 * \param[in] cad_file CAD file to read
 * \param[out] vertices extracted vertices
 * \param[out] colors extracted colors
 * \return true if success, false otherwise
 */
bool VrmlManualParser::LoadVRML(std::string cad_file,
    std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &colors)
{
    bool load_success;
    int nb_shapes;

    load_success = false;
    nb_shapes = NumberShapesVRML(cad_file);

    for(int i=0; i<nb_shapes; i++)
    {
        if (LoadSingleShapeVRML(cad_file, i, vertices, colors))
        {
            load_success = true;
        }
    }

    return load_success;
}

}
#endif
