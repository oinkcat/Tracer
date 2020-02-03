#include "stdafx.h"
#include "Scene.h"

Scene::Scene() 
	: defaultColor(0.7, 0.7, 0.7), depth(DEFAULT_DEPTH), mainLight(0.2, -0.6, -0.2),
	  origin(0.3, -1.7, 0), rotation(0.43, 0, 0)
{
	auto s1 = new Sphere(Vector(0, -0.4, 4), 0.4, Vector(1, 0.3, 0.3));
	objects.push_back(s1);

	auto s2 = new Sphere(Vector(-0.8, -0.4, 3), 0.4, Vector(0.3, 1, 1));
	objects.push_back(s2);

	auto s3 = new Sphere(Vector(0.8, -0.3, 3.5), 0.3, Vector(0.3, 1, 0.3));
	objects.push_back(s3);

	auto s4 = new Sphere(Vector(0, -0.2, 2.5), 0.2, Vector(1, 0.3, 1));
	objects.push_back(s4);

	auto plane = new Plane(Vector(0, 0, 0), Vector(0.8, 0.8, 0.4), Vector(0, -1, 0));
	objects.push_back(plane);
}

Scene::~Scene()
{
	for (auto obj : objects)
	{
		delete obj;
	}
}

Vector Scene::traceAt(double aX, double aY)
{
	aX += rotation.x();
	aY += rotation.y();

	double x = sin(aY) * cos(aX);
	double y = sin(aX);
	double z = cos(aX) * cos(aY);

	Ray ray(origin, Vector(x, y, z));
	double len = ray.dir().length();

	Vector* tracedColor = ray.trace(this, Ray::TraceType::Normal);
	Vector resultColor = tracedColor != nullptr ? Vector(*tracedColor) 
											    : defaultColor;
	delete tracedColor;

	return resultColor;
}
