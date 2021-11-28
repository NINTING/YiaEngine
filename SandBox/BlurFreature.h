#pragma once
#include "YiaEngine.h"

using namespace YiaEngine;

class BlurFeature
{

};

class BlurPass : public FramePass
{
	virtual void Setup()
	{

	}
};


class GeometryPass : public FramePass
{
	virtual void Setup()
	{
		AddInput("MainTexture");
	}
};