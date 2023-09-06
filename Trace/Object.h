#pragma once
#include "Vector.h"

// Base class for all shapes
class Object
{
public:
	Object(Vector pos, Vector color) : position(pos), diffuse(color) {}
	~Object() {}

	virtual Vector normalAt(Vector point) = 0;

	virtual Vector colorAt(Vector point) = 0;

	virtual bool testBoundingBox(Vector point) = 0;

	virtual bool testHit(Vector point, double *dist) = 0;

	Vector& pos()
	{
		return position;
	}

	Vector& color()
	{
		return diffuse;
	}

protected:
	Vector position;
	Vector diffuse;
};

