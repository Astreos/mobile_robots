/*! 
 * \author Nicolas Van der Noot
 * \file Noise.hh
 * \brief Noise class
 */

#ifndef _NOISE_HH_
#define _NOISE_HH_

#include <cstdlib>

double rnd(void);
double unity_noise(double range);
double range_noise(double range);

/*! \brief Noise of a signal signal with linear interpolation
 */
class Noise
{
	public:
		Noise(double t, double max_noise, double t_period);
		~Noise();

		void update(double t);

		/// get cur noise value in [-max_noise ; max_noise]
		double get()
		{
			return cur_n;
		}

		/// get cur noise value in [0 ; max_noise]
		double get_positive()
		{
			return 0.5 * (cur_n + max_noise);
		}

		/// get and get cur noise value in [-max_noise ; max_noise]
		double update_and_get(double t)
		{
			update(t);
			return get();
		}

		/// update and get cur noise value in [0 ; max_noise]
		double update_and_get_positive(double t)
		{
			update(t);
			return get_positive();
		}

		double get_new_noise();

	private:
		double last_n; ///< last random noise computed
		double next_n; ///< next random noise computed
		double cur_n;  ///< current random noise computed

		double last_t; ///< last time mew random noise computed [s]

		double max_noise; ///< maximum noise level (amplitude)
		double t_period;  ///< period between two new random values computed [s]
};

#endif
