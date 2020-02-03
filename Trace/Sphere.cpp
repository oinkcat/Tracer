#include "stdafx.h"
#include "Sphere.h"

Sphere::Sphere(Vector pos, double r, Vector color) 
	: Object(pos, color), radius(r)
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

	double length = sqrt(dv.dot(dv));

	*dist = length - radius;

	return length <= radius;
}
