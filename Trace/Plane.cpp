#include "stdafx.h"
#include "Plane.h"

Plane::Plane(Vector pos, Vector color, Vector normal)
	: Object(pos, color), normal(normal),
	  altColor(diffuse.z(), diffuse.x(), diffuse.y())
{
}

Vector Plane::normalAt(Vector point)
{
	return normal;
}

Vector Plane::colorAt(Vector point)
{
	int iX = (int)point.x();
	int iZ = (int)point.z();

	if ((iX + iZ) % 2 == 0)
	{
		return diffuse;
	}
	else
	{
		return altColor;
	}
}

bool Plane::testHit(Vector point, double *dist)
{
	Vector dv(point.x() - position.x(), 
			  point.y() - position.y(), 
			  point.z() - position.z());

	double sum = normal.dot(dv);
	*dist = sum;
	
	return sum < 0;
}
