#include "stdafx.h"
#include "RayTracer.h"

// Create instance with given viewport size and view angle
RayTracer::RayTracer(int width, int height, double angle)
	: viewWidth(width), viewHeight(height)
{
	xAngle = angle;
	double ratio = (double)width / height;
	yAngle = angle / ratio;

	frameSize = width * height * 3;
	frameBuffer = new unsigned char[frameSize]();

	setNumberOfThreads(1);
}

RayTracer::~RayTracer()
{
	delete[] frameBuffer;
}

void RayTracer::clear()
{
	memset(frameBuffer, 0, frameSize);
}

void RayTracer::tracePart()
{
	double halfXAngle = xAngle / 2;
	double halfYAngle = yAngle / 2;

	int startLine = viewHeight - (int)linesLeft;
	int endLine = startLine + (int)linesPerThread;
	linesLeft -= linesPerThread;

	// All remaining lines for last thread
	if (linesLeft < linesPerThread)
	{
		endLine += (int)linesLeft;
		linesLeft = 0;
	}

	int triIdx = startLine * viewWidth * 3;

	double daX = yAngle / viewHeight;
	double daY = xAngle / viewWidth;

	double aX = startLine * daX - halfYAngle;

	for (int y = startLine; y < endLine; y++, aX += daX)
	{
		double aY = -halfXAngle;

		for (int x = 0; x < viewWidth; x++, aY += daY)
		{
			Vector colorAtPoint = scene.traceAt(aX, aY);

			// BGR
			frameBuffer[triIdx + 2] = (unsigned char)(colorAtPoint.x() * 255);
			frameBuffer[triIdx + 1] = (unsigned char)(colorAtPoint.y() * 255);
			frameBuffer[triIdx] = (unsigned char)(colorAtPoint.z() * 255);

			triIdx += 3;
		}
	}
}
