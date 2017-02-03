/*! 
 * \author Nicolas Van der Noot
 * \file VrmlManualParser.hh
 * \brief VrmlManualParser class
 */
#ifdef OPEN_GL

#ifndef _VRML_MANUAL_PARSER_HH_
#define _VRML_MANUAL_PARSER_HH_

#include <string>
#include <vector>

#ifndef degreesToRadians
    #define degreesToRadians(x) x*(3.141592f/180.0f)
#endif

#ifndef GLM_FORCE_RADIANS
    #define GLM_FORCE_RADIANS
#endif

#include <glm.hpp>

namespace OpenGLMbs{

/*! \brief manual VRML loader
 */
class VrmlManualParser
{
    public:
        VrmlManualParser();
        ~VrmlManualParser();

        bool LoadVRML(std::string cad_file, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &colors);

    private:
        int StringToInt(std::string cur_str);
        float StringToFloat(std::string cur_str);

        bool IsInt(std::string cur_str);
        bool IsFloat(std::string cur_str);

        bool LineProcess(std::string cur_str, int flag_int, 
            std::vector<int> &out_int, std::vector<double> &out_float);

        bool LoadSingleShapeVRML(std::string cad_file, int shape_id, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &colors);
        int NumberShapesVRML(std::string cad_file);

        bool MarkerLineVRML(std::string cad_file, std::string marker, int shape_id, int flag_int,
            std::vector<int> &out_int, std::vector<double> &out_float);
        bool MarkerReadVRML(std::string cad_file, std::string marker, int shape_id, int flag_int,
            std::vector<int> &out_int, std::vector<double> &out_float);
};

}
#endif
#endif
