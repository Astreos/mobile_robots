/*! 
 * \author Nicolas Van der Noot
 * \file Base3D.hh
 * \brief Base3D class
 */
#ifdef OPEN_GL

#ifndef _BASE_3D_HH_
#define _BASE_3D_HH_

#include "Joint3D.hh"

namespace OpenGLMbs{

/*! \brief Base 3D component
 */
class Base3D: public Joint3D
{
    public:
        Base3D(MbsWorld3D *world_3d);
        virtual ~Base3D();
};

}
#endif
#endif
