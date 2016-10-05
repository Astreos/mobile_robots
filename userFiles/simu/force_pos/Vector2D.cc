
#include "Vector2D.hh"

/// constructor
Vector2D::Vector2D(double x, double y)
{
	vector[0] = x;
	vector[1] = y;
}

/// constructor
Vector2D::Vector2D(double *X)
{
	for (int i=0; i<2; i++)
	{
		vector[i] = X[i];
	}
}

/// constructor
Vector2D::Vector2D()
{
	for (int i=0; i<2; i++)
	{
		vector[i] = 0.0;
	}
}

/// destructor
Vector2D::~Vector2D()
{

}
