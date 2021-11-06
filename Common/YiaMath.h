#pragma once
#define _USE_MATH_DEFINES 
#include<math.h>
#include<ThirdParty/Eigen/Dense>
namespace YiaEngine
{
	namespace Math
	{
		float Angle2Rad(float angle);

		template <typename T> __forceinline T AlignUpWithMask(T value, size_t mask)
		{
			return (T)(((size_t)value + mask) & ~mask);
		}
		template <typename T> __forceinline T AlignUp(T value, size_t alignment)
		{
			return AlignUpWithMask(value, alignment - 1);
		
		}
		
		

		/// <summary>
		/// ––”≈œ»
		/// </summary>
		using Vec2f = Eigen::RowVector2f;
		using Vec3f = Eigen::RowVector3f;
		using Vec4f = Eigen::RowVector4f;
		using Mat3x3f = Eigen::Matrix<float, 3,3, Eigen::RowMajor>;
		using Mat4x4f = Eigen::Matrix<float, 4,4, Eigen::RowMajor>;
		//using Vec2f = Eigen::Vector2f;
		//using Vec3f = Eigen::Vector3f;
		//using Vec4f = Eigen::Vector4f;
		//using Mat3x3f = Eigen::Matrix3f;
		//using Mat4x4f = Eigen::Matrix4f;

		Math::Vec3f RotationByAxis(Math::Vec3f aixs,Math::Vec3f dir, float rad);
		Math::Vec3f RotationByAxisX(Math::Vec3f dir,float rad);
		Math::Vec3f RotationByAxisY(Math::Vec3f dir, float rad);
		Math::Vec3f RotationByAxisZ(Math::Vec3f dir, float rad);
	}
}