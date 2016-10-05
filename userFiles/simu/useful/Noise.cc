
#include "Noise.hh"

/// generate random number in [0,1]
double rnd(void)
{
	return ((double)rand())/((double)RAND_MAX);
}

/// generate random number in [1-0.5*range , 1+0.5*range]
double unity_noise(double range)
{
	return 1.0 + (rnd() - 0.5) * range;
}

/// generate random number in [-0.5*range , 0.5*range]
double range_noise(double range)
{
	return (rnd() - 0.5) * range;
}

/*! \brief constructor
 * 
 * \param[in] t time [s]
 * \param[in] max_noise maximum noise level (amplitude)
 * \param[in] t_period period between two new random values computed [s]
 */
Noise::Noise(double t, double max_noise, double t_period)
{
	this->max_noise = max_noise;
	this->t_period  = t_period;

	last_t = t;

	last_n = get_new_noise();
	next_n = get_new_noise();
	cur_n  = last_n;
}

// destructor
Noise::~Noise()
{

}

/*! \brief update the noise attributes
 *
 * \param[in] t time [s]
 */
void Noise::update(double t)
{
	double diff_t = t - last_t;

	// new random value generated
	if(diff_t >= t_period)
	{
		cur_n  = next_n;
		last_n = next_n;
		next_n = get_new_noise();

		last_t = t;
	}
	// interpolation between two random values
	else
	{
		cur_n = (next_n - last_n) * (diff_t / t_period) + last_n;
	}
}

/*! \brief compute a new random value in the [-max_noise ; max_noise] interval
 * 
 * \return requested random value
 */
double Noise::get_new_noise()
{
	return (rnd() - 0.5) * (2.0 * max_noise);
}
