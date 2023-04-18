#include "stdafx.h"
#include "Sphere.h"

Sphere::Sphere(Vector pos, double r, Vector color) 
	: Object(pos, color), radius(r), squaredRadius(r * r)
{
}

Sphere::~Sphere()
{
}

Vector Sphere::normalAt(Vector point)
{
	Vector normal = (point - position).normalize();
	return normal;
}

Vector Sphere::colorAt(Vector point)
{
	return diffuse;
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
