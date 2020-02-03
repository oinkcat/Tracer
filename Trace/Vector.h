#pragma once
#include <math.h>

// Basic 3-dimensional vector
class Vector
{
public:
	Vector(double x, double y, double z): _x(x), _y(y), _z(z) { }

	~Vector() { }

	// Add a vector
	Vector operator+(Vector &rhs)
	{
		return Vector(_x + rhs._x, _y + rhs._y, _z + rhs._z);
	}

	// Subtract a vector
	Vector operator-(Vector &rhs)
	{
		return Vector(_x - rhs._x, _y - rhs._y, _z - rhs._z);
	}

	// Multiply by scalar value
	Vector operator*(double k)
	{
		return Vector(_x * k, _y * k, _z * k);
	}

	// The x coordinate
	double x() const
	{
		return this->_x;
	}

	// The y coordinate
	double y() const
	{
		return this->_y;
	}

	// The z coordinate
	double z() const
	{
		return this->_z;
	}

	// Get length of vector
	double length() const
	{
		return sqrt(_x * _x + _y * _y + _z * _z);
	}

	// Get dot product of two vectors
	double dot(Vector &rhs) const
	{
		return _x * rhs._x + _y * rhs._y + _z * rhs._z;
	}

	// Get normalized vector
	Vector normalize()
	{
		double myLength = length();
		return Vector(_x / myLength, _y / myLength, _z / myLength);
	}

private:
	double _x;
	double _y;
	double _z;
};

