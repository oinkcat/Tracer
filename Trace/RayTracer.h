#pragma once
#include "Vector.h"
#include "Scene.h"

// Super LOL tracer
class RayTracer
{
public:
	RayTracer(int width, int height, double angle);
	~RayTracer();

	void clear();

	void tracePart();

	int viewportWidth() const
	{
		return this->viewWidth;
	}

	int viewportHeight() const
	{
		return this->viewHeight;
	}

	unsigned char* frame() const
	{
		return this->frameBuffer;
	}

	void setNumberOfThreads(size_t num)
	{
		this->numThreads = num;
		this->linesPerThread = viewHeight / numThreads;
		this->linesLeft = viewHeight;
	}

private:
	Scene scene;

	int viewWidth;
	int viewHeight;

	double xAngle;
	double yAngle;

	unsigned char* frameBuffer;
	int frameSize;

	size_t numThreads;
	size_t linesPerThread;
	size_t linesLeft;
};
