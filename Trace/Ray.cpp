#include "stdafx.h"
#include "Ray.h"

Vector Ray::trace(Scene* scene, TraceType type, Object* excluded)
{
	const double FINAL_DIST = 0.01;

	if (type == TraceType::Reflection)
	{
		assert(excluded != nullptr);
	}

	this->scene = scene;
	Vector point(_origin);

	double endDepth = scene->traceDepth();
	double rayLength = 0;
	Vector stepIn = _dir * 0.5;

	while (rayLength < endDepth)
	{
		for (auto obj : scene->sceneObjects())
		{
			if (obj == excluded) { continue; }
			if (!obj->testBoundingBox(point)) { continue; }

			double dist;

			if (obj->testHit(point, &dist))
			{
				if (type == TraceType::Shadow)
				{
					return Vector(rayLength, rayLength, rayLength);
				}
				else
				{
					Vector stepOut = stepIn * (-0.02);

					while (dist < FINAL_DIST)
					{
						point = point + stepOut;
						obj->testHit(point, &dist);
					}
				}

				return getColorAtPoint(obj, point, type);
			}
		}

		point = point + stepIn;
		rayLength += stepIn.length();
	}

	if (type != TraceType::Shadow)
	{
		return Vector(-1.0, -1.0, -1.0);
	}
	else
	{
		return Vector(rayLength, rayLength, rayLength);
	}
}

Vector Ray::getColorAtPoint(Object * obj, Vector point, TraceType type)
{
	Vector normal = obj->normalAt(point);

	// Reflection color
	Vector reflComponent(0, 0, 0);
	Vector refrComponent(0, 0, 0);

	if (type == TraceType::Light)
	{
		reflComponent = traceReflection(obj, point, normal) * 0.7;
		refrComponent = traceRefraction(obj, point, normal) * 0.5;
	}

	// Luminosity
	double shading = normal.dot(scene->light());
	shading = shading < 0.1 ? 0.1 : shading;
	Vector lumin = traceLuminosity(obj, point);
	double brightness = lumin.x() * pow(shading * 1.3, 3) + (shading * 0.5);

	Vector diffuse = obj->colorAt(point);
	Vector resultColor = (diffuse + reflComponent + refrComponent) *brightness;

	double cr = resultColor.x() > 1.0 ? 1.0 : resultColor.x();
	double cg = resultColor.y() > 1.0 ? 1.0 : resultColor.y();
	double cb = resultColor.z() > 1.0 ? 1.0 : resultColor.z();

	return Vector(cr, cg, cb);
}

Vector Ray::traceReflection(Object * obj, Vector point, Vector dir)
{
	Ray reflectionRay(point, dir);
	Vector reflColor = reflectionRay.trace(scene, TraceType::Reflection, obj);

	return (reflColor.x() > 0) ? reflColor : Vector(0, 0, 0);
}

Vector Ray::traceRefraction(Object * obj, Vector point, Vector dir)
{
	return traceReflection(obj, point, dir * (-1));
}

Vector Ray::traceLuminosity(Object * obj, Vector point)
{
	Ray toLight(point, scene->light());
	Vector tracedLength = toLight.trace(scene, TraceType::Shadow);

	double lumin = tracedLength.x() / scene->traceDepth();

	return Vector(lumin, lumin, lumin);
}
