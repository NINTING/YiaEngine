#include"pch.h"
#include "Transform.h"
#include"Common/YiaMath.h"

namespace YiaEngine
{
	void Transform::Translation(float x, float y, float z)
	{
		Position += Math::Vec3f(x, y, z);
	}
	void Transform::Rotation(float x, float y, float z)
	{
		EularRotation << x, y, z;
	}
	void Transform::RotationAxis(float x, float y, float z)
	{

	}
	Math::Mat4x4f Transform::FinalMatrix()
	{
	/*	Math::Mat3x3f mat = Eigen::AngleAxisf(EularRotation[0], Math::AxisX).matrix() *
		Eigen::AngleAxisf(EularRotation[1], Math::AxisY) *
			Eigen::AngleAxisf(EularRotation[2], Math::AxisZ);
		std::cout << mat.array() << std::endl;*/
		
		float cosZ = cosf(EularRotation[2]);
		float cosY = cosf(EularRotation[1]);
		float cosX = cosf(EularRotation[0]);
		float sinX = sinf(EularRotation[0]);
		float sinY = sinf(EularRotation[1]);
		float sinZ = sinf(EularRotation[2]);

		Math::Mat4x4f ret;
		ret << cosX * cosY,					cosY* sinZ,						-sinY, 0.f,
			sinX* sinY* cosZ - cosX * sinZ, sinX* sinY* sinZ + cosX * cosZ, sinX* cosY,0.f,
			cosX* sinY* cosZ + sinX * sinZ, cosX* sinY* sinZ - sinX * cosZ, cosX* cosY,0.f,
			Position.x(), Position.y(), Position.z(), 1.f;
		//std::cout << ret << std::endl;
		return ret;
	}
}