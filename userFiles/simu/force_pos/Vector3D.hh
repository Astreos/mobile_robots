/*! 
 * \author Nicolas Van der Noot
 * \file Vector3D.hh
 * \brief Vector3D class
 */

#ifndef __VECTOR_3_D__HH__
#define __VECTOR_3_D__HH__

#include "Vector.hh"

/*! \brief Description of a 3D vector
 */
class Vector3D: public Vector
{
	public:
		Vector3D(double x, double y, double z);
		Vector3D(double *X);
		Vector3D();

		virtual ~Vector3D();

		/*! \brief get a vector element
		 * 
		 * \param[in] index of the requested component
		 * \return distance between the two points [m]
		 */
		double get_vec(int index) const { return vector[index]; }

		/*! \brief return first vector element
		 * 
		 * \return x component
		 */
		double get_x() const { return vector[0]; }

		/*! \brief return second vector element
		 * 
		 * \return y component
		 */
		double get_y() const { return vector[1]; }

		/*! \brief return third vector element
		 * 
		 * \return z component
		 */
		double get_z() const { return vector[2]; }

		/*! \brief set new values for the vector
		 * 
		 * \param[in] pos3D new vector to copy
		 */
		void set_vector(Vector3D const& pos3D)
		{
			for (int i=0; i<3; i++)
			{
				vector[i] = pos3D.get_vec(i);
			}
		}

		/*! \brief set new values for the vector
		 * 
		 * \param[in] x x component of the vector
		 * \param[in] y y component of the vector
		 * \param[in] z z component of the vector
		 */
		void set_vector(double x, double y, double z)
		{
			vector[0] = x;
			vector[1] = y;
			vector[2] = z;
		}

		/*! \brief set the x value
		 * 
		 * \param[in] value new value for x
		 */
		void set_x(double value)
		{
			vector[0] = value;
		}

		/*! \brief set the y value
		 * 
		 * \param[in] value new value for y
		 */
		void set_y(double value)
		{
			vector[1] = value;
		}

		/*! \brief set the z value
		 * 
		 * \param[in] value new value for z
		 */
		void set_z(double value)
		{
			vector[2] = value;
		}

		/*! \brief increment the x value
		 * 
		 * \param[in] value value for x to increment
		 */
		void inc_x(double value)
		{
			vector[0] += value;
		}

		/*! \brief increment the y value
		 * 
		 * \param[in] value value for y to increment
		 */
		void inc_y(double value)
		{
			vector[1] += value;
		}

		/*! \brief increment the z value
		 * 
		 * \param[in] value value for z to increment
		 */
		void inc_z(double value)
		{
			vector[2] += value;
		}

		/*! \brief set a new value for one index of the vector
		 * 
		 * \pre index <= 2
		 * \param[in] index index of the vector
		 * \param[in] value new value
		 */
		void set_index_vector(int index, double value)
		{
			vector[index] = value;
		}

		/*! \brief set new values for the vector
		 * 
		 * \pre X must be a tabular of three elements
		 * \param[in] X tabular to copy
		 */
		void set_vector(double *X)
		{
			for (int i=0; i<3; i++)
			{
				vector[i] = X[i];
			}
		}

	private:
		double vector[3]; ///< three components of the vector
};

/*! \brief sum two vectors with the + opration
 * 
 * \param[in] a first vector to sum
 * \param[in] b second vector to sum 
 * \return sum of the two vectors
 */
inline Vector3D operator+(Vector3D const& a, Vector3D const& b)
{
	Vector3D result;

	for (int i=0; i<3; i++)
	{
		result.set_index_vector(i, a.get_vec(i) + b.get_vec(i));
	}

	return result;
}

/*! \brief difference of two vectors with the - opration
 * 
 * \param[in] a first vector
 * \param[in] b second vector 
 * \return difference of the two vectors
 */
inline Vector3D operator-(Vector3D const& a, Vector3D const& b)
{
	Vector3D result;

	for (int i=0; i<3; i++)
	{
		result.set_index_vector(i, a.get_vec(i) - b.get_vec(i));
	}

	return result;
}

/*! \brief return a vector with only zero values
 * 
 * \return vector
 */
inline Vector3D get_nul_vector_3D()
{
	Vector3D result(0.0, 0.0, 0.0);

	return result;
}

/*! \brief return a vector
 * 
 * \param[in] x x component
 * \param[in] y y component
 * \return vector with x and y components
 */
inline Vector3D get_vector_3D(double x, double y, double z)
{
	Vector3D result(x, y, z);

	return result;
}

#endif
