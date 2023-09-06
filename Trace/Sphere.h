#pragma once
#include "Object.h"

// Basic sphere object
class Sphere : public Object
{
public:
	Sphere(Vector pos, double r, Vector color);
	~Sphere();

	Vector normalAt(Vector point) override;

	Vector colorAt(Vector point) override;

	bool testBoundingBox(Vector point) override;

	bool testHit(Vector point, double *dist) override;

	double rad()
	{
		return this->radius;
	}

private:
	double radius;
	double squaredRadius;

	Vector boxMin, boxMax;
};

