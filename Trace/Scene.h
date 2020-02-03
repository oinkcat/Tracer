#pragma once
#include <math.h>
#include <vector>

#include "Vector.h"
#include "Plane.h"
#include "Sphere.h"

// Scene that contains objects for tracing
class Scene
{
public:
	Scene();
	~Scene();

	double traceDepth() const
	{
		return this->depth;
	}

	Vector& light()
	{
		return this->mainLight;
	}

	std::vector<Object*>& sceneObjects()
	{
		return this->objects;
	}

	Vector traceAt(double aX, double aY);

private:
	const double DEFAULT_DEPTH = 10.0;

	Vector defaultColor;
	Vector mainLight;

	Vector origin;
	Vector rotation;

	double depth;

	std::vector<Object*> objects;
};

#include "Ray.h"