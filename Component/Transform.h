#pragma once
#include"pch.h"
#include"Common/YiaMath.h"

namespace YiaEngine
{
	class Transform
	{
	public:
		Math::Vec3f Position;
		Math::Vec3f EularRotation;
		Math::Vec3f QuatRotation;
		void Translation(float x,float y,float z);
		void Rotation(float x, float y, float z);
		void RotationAxis(float x, float y, float z);
		Math::Mat4x4f FinalMatrix();
	};

}