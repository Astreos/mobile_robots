#ifdef OPEN_GL

#include "AnimReader.hh"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <cmath>

namespace OpenGLMbs{

/// limit x in the [min;max] range
inline int limit_range(int x, int min, int max) { return (x <= min) ? min : ((x >= max) ? max : x); }

/*! \brief constructor
 *
 * \param[in] anim_filename name of the .anim file (with its path)
 */
AnimReader::AnimReader(const char* anim_filename)
{
    // variables declaration
    int cur_i;
    std::string cur_line, token;

    // load .anim file
    std::ifstream f(anim_filename);

    // get the number of joints
    nb_q = NbQCompute(anim_filename);

    // loop on all the .anim file lines
    while(std::getline(f, cur_line))
    {
        std::istringstream iss(cur_line);

        cur_i = -1;

        q_tab.push_back(new double[nb_q]);

        while(std::getline(iss, token, ' '))
        {
            if (IsFloat(token))
            {
                // time
                if (cur_i == -1)
                {
                    // add new time
                    t_vec.push_back(StringToFloat(token));
                }
                else
                {
                    // add new joint value
                    q_tab.back()[cur_i] = StringToFloat(token);
                }

                cur_i++;
            }
        }

        // safety
        if (cur_i != nb_q)
        {
            std::cout << "AnimReader error: number of joints not consistend (" << cur_i << " >< " << nb_q << ") !" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    f.close();

    // time vector size
    t_size = t_vec.size();

    if (t_size <= 0)
    {
        std::cout << "AnimReader error: t_vec size (" << t_size << ") should be strictly positive !" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (t_size != q_tab.size())
    {
        std::cout << "AnimReader error: t_vec size (" << t_size << ") different from q_tab size (" << q_tab.size() << ") !" << std::endl;
        exit(EXIT_FAILURE);
    }

    // timing special values
    init_t = t_vec[0];
    end_t  = t_vec[t_size-1];
    diff_t = end_t - init_t;

    if (diff_t < 0.0)
    {
        std::cout << "AnimReader error: final time should be bigger than initial time !" << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*! \brief destructor
 */
AnimReader::~AnimReader()
{
    for(unsigned int i=0; i<q_tab.size(); i++)
    {
        delete[] q_tab[i];
    }
}

/*! \brief check if the current string is a float
 * 
 * \param[in] cur_str string to check
 * \return true if floar, false otherwise
 */
bool AnimReader::IsFloat(std::string cur_str)
{
    float f;
    std::istringstream iss(cur_str);

    iss >> std::noskipws >> f;

    return !iss.fail(); 
}

/*! \brief convert string to float
 * 
 * \param[in] cur_str string to convert
 * \return converted float
 */
float AnimReader::StringToFloat(std::string cur_str)
{
    float f;
    std::istringstream iss(cur_str);

    iss >> std::noskipws >> f;

    return f;
}

/*! \brief compute the number of joints
 * 
 * \param[in] anim_filename .anim file (with path)
 * \return number of joints
 */
int AnimReader::NbQCompute(const char* anim_filename)
{
    int cur_nb_q;
    std::string first_line, token;

    // read file
    std::ifstream f(anim_filename);

    // get first line
    std::getline(f, first_line);

    std::istringstream iss(first_line);

    // start to -1 to remove time
    cur_nb_q = -1;

    // loop on all the numbers of the first line
    while(std::getline(iss, token, ' '))
    {
        if (IsFloat(token))
        {
            cur_nb_q++;
        }
    }

    f.close();

    return cur_nb_q;
}

/*! \brief fill a vector with the joint values for a requested time
 * 
 * \param[in] target_t requested time [s]
 * \param[in] nb_q number of joints requested (for check)
 * \param[out] q_vec vector to fill with the joints
 *
 * The joint values are interpolated from the closest time data.
 * If requested time is out of the minimal and maximal timing range,
 * the returned solution corresponds to the initial/final time in the range.
 */
void AnimReader::FillQ(double target_t, int nb_q, double *q_vec)
{
    int i0, i1;
    double alpha, alpha_comp, diff_t_vec;

    // safety
    if (this->nb_q != nb_q)
    {
        std::cout << "AnimReader error: nb_q (" << nb_q << ") should be " << this->nb_q << " !" << std::endl;
        exit(EXIT_FAILURE);
    }

    // get the indexes of the closest neighbours
    IndexTime(target_t, i0, i1);

    diff_t_vec = t_vec[i1] - t_vec[i0];

    // if not two different and valid neighbours
    if (i0 == i1 || diff_t <= 0.0)
    {
        for(int i=0; i<nb_q; i++)
        {
            q_vec[i] = q_tab[i0][i];
        }
    }
    // interpolation
    else
    {
        alpha = (target_t - t_vec[i0]) / diff_t_vec;
        alpha = limit_range(alpha, 0.0, 1.0);
        alpha_comp = 1.0 - alpha;

        for(int i=0; i<nb_q; i++)
        {
            q_vec[i] = alpha_comp * q_tab[i0][i] + alpha * q_tab[i1][i];
        }
    }
}

/*! \brief get the indexes of the closest neighbours for one requested time
 * 
 * \param[in] target_t requested time [s]
 * \param[out] i0 index of the neighbour, just before requested time
 * \param[out] i1 index of the neighbour, just after requested time
 */
void AnimReader::IndexTime(double target_t, int &i0, int &i1)
{
    int guess_i, cur_i;

    // safety
    if (diff_t <= 0.0)
    {
        i0 = 0;
        i1 = 0;
        return;
    }

    // initial guess
    guess_i = round(((target_t - init_t) / diff_t) * t_size);

    // bound guess
    guess_i = limit_range(guess_i, 0, t_size-1);

    // below requested time value
    if (target_t < t_vec[guess_i])
    {
        if (guess_i > 0)
        {
            i0 = guess_i-1;
            i1 = guess_i;
        }
        else
        {
            i0 = 0;
            i1 = 0;
            return;
        }

        cur_i = guess_i - 1;

        while (cur_i > 0 && target_t < t_vec[cur_i])
        {
            cur_i--;
        }

        if (cur_i > 0 && target_t >= t_vec[cur_i])
        {
            i0 = cur_i;
            i1 = cur_i+1;
        }
        else
        {
            i0 = 0;
            i1 = 0;
            return;
        }
    }
    // above requested time value
    else
    {
        if (guess_i < t_size-1)
        {
            i0 = guess_i;
            i1 = guess_i+1;
        }
        else
        {
            i0 = t_size-1;
            i1 = t_size-1;
            return;
        }

        cur_i = guess_i + 1;

        while (cur_i < t_size-1 && target_t > t_vec[cur_i])
        {
            cur_i++;
        }

        if (cur_i < t_size-1 && target_t <= t_vec[cur_i])
        {
            i0 = cur_i;
            i1 = cur_i+1;
        }
        else
        {
            i0 = t_size-1;
            i1 = t_size-1;
            return;
        }
    }

    // limit final values
    i0 = limit_range(i0, 0, t_size-1);
    i1 = limit_range(i1, 0, t_size-1);
}

}
#endif
