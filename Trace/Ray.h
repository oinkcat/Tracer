#pragma once
#include <assert.h>
#include "Vector.h"
#include "Scene.h"

class Ray
{
public:

	enum class TraceType
	{
		Light,
		Reflection,
		Shadow
	};

	Ray(Vector origin, Vector dir): _origin(origin), _dir(dir), scene(nullptr) { }

	Ray(Vector dir): _origin(Vector(0, 0, 0)), _dir(dir), scene(nullptr) { }

	~Ray() { }

	Vector trace(Scene* scene, TraceType type, Object* excluded = nullptr);

	Vector& origin()
	{
		return _origin;
	}

	Vector& dir()
	{
		return _dir;
	}

private:
	Scene* scene;
	Vector _origin;
	Vector _dir;

	Vector getColorAtPoint(Object* obj, Vector point, TraceType type);
	Vector traceReflection(Object* obj, Vector point, Vector dir);
	Vector traceRefraction(Object* obj, Vector point, Vector dir);
	Vector traceLuminosity(Object* obj, Vector point);
};