#include"pch.h"
#include"YiaMath.h"

namespace YiaEngine
{
	namespace Math
	{
		float Angle2Rad(float angle)
		{
			return angle * (float)M_PI / 180.f;
		}
		Math::Vec3f RotationByAxis(Math::Vec3f aixs, Math::Vec3f dir, float rad)
		{
			Eigen::AngleAxisf rotation_vector(rad, aixs);
			Math::Mat3x3f rotation_matrix = rotation_vector.toRotationMatrix().transpose();

			return dir * rotation_matrix;
		}
		Math::Vec3f RotationByAxisX(Math::Vec3f dir, float rad)
		{
			Eigen::AngleAxisf rotation_vector(rad, Math::Vec3f(1, 0, 0));
			Math::Mat3x3f rotation_matrix = rotation_vector.toRotationMatrix().transpose();
			
			return dir * rotation_matrix;
		}
		Math::Vec3f RotationByAxisY(Math::Vec3f dir, float rad)
		{
			Eigen::AngleAxisf rotation_vector(rad, Math::Vec3f(0, 1, 0));
			Math::Mat3x3f rotation_matrix = rotation_vector.toRotationMatrix().transpose();
			
			return dir * rotation_matrix;
		}
	}
}

