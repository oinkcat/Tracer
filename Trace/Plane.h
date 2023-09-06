#pragma once
#include "Object.h"

// Basic unbounded plane
class Plane : public Object
{
public:
	Plane(Vector pos, Vector color, Vector normal);
	~Plane() {}

	Vector normalAt(Vector point) override;

	Vector colorAt(Vector point) override;

	bool testBoundingBox(Vector point) override;

	bool testHit(Vector point, double *dist) override;

private:
	Vector normal;

	Vector altColor;
};

