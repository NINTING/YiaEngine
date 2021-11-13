#pragma once

#include"Common/YiaMath.h"
#include"Common/Color.h"
namespace YiaEngine
{
	enum LightType
	{
		Light_Direction,
	};
	struct LightData
	{
		LightData() = default;
		Math::Vec3f Dirction;
		float Intensity;
		Color Color;
	};
	class Light
	{
	public:
		Light() = default;
		Light(LightType type,const LightData& data):type_(type),data_(data){}

		LightType type_;
		LightData data_;
	};
}