/*! 
 * \author Nicolas Van der Noot
 * \file AnimReader.hh
 * \brief AnimReader class
 */
#ifdef OPEN_GL

#ifndef _ANIM_READER_HH_
#define _ANIM_READER_HH_

#include <string>
#include <vector>

namespace OpenGLMbs{

/*! \brief read a .anim file and return the joint vector for a given time
 * (with interpolation if needed)
 */
class AnimReader
{
    public:
        AnimReader(const char* anim_filename);
        ~AnimReader();

        void FillQ(double target_t, int nb_q, double *q_vec);

    private:
        bool IsFloat(std::string cur_str);
        float StringToFloat(std::string cur_str);
        int NbQCompute(const char* anim_filename);
        void IndexTime(double target_t, int &i0, int &i1);

        int nb_q;   ///< number of joints
        int t_size; ///< size of the time vector

        double init_t; ///< initial time [s]
        double end_t;  ///< final time [s]
        double diff_t; ///< time from start to end [s]

        std::vector<double> t_vec; ///< time vector

        std::vector<double*> q_tab; ///< joints tabular
};

}
#endif
#endif
