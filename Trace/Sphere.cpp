#include "stdafx.h"
#include "Sphere.h"

Sphere::Sphere(Vector pos, double r, Vector color) 
	: Object(pos, color), radius(r), squaredRadius(r * r),
	  boxMin(pos.x() - radius, pos.y() - radius, pos.z() - radius),
	  boxMax(pos.x() + radius, pos.y() + radius, pos.z() + radius)
{
}

Sphere::~Sphere()
{
}

Vector Sphere::normalAt(Vector point)
{
	return (point - position).normalize();
}

Vector Sphere::colorAt(Vector point)
{
	return diffuse;
}

bool Sphere::testBoundingBox(Vector point)
{
	return (point.x() >= boxMin.x() && point.x() <= boxMax.x()) &&
		   (point.y() >= boxMin.y() && point.y() <= boxMax.y()) &&
		   (point.z() >= boxMin.z() && point.z() <= boxMax.z());
}

bool Sphere::testHit(Vector point, double *dist)
{
	Vector dv(position.x() - point.x(),
			  position.y() - point.y(),
			  position.z() - point.z());

	double length = dv.dot(dv);

	*dist = length - squaredRadius;

	return length <= squaredRadius;
}
