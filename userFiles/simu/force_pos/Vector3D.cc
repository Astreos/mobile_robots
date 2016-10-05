
#include "Vector3D.hh"

/// constructor
Vector3D::Vector3D()
{
	
}

/// constructor
Vector3D::Vector3D(double x, double y, double z)
{
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
}

/// constructor
Vector3D::Vector3D(double *X)
{
	for (int i=0; i<3; i++)
	{
		vector[i] = X[i];
	}
}

/// destructor
Vector3D::~Vector3D()
{

}
