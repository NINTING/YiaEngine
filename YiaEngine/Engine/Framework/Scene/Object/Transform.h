#ifndef FRAMEWORK_SCENE_TRANSFORM_H
#define FRAMEWORK_SCENE_TRANSFORM_H

#include "Math/YiaMath.h"
#include "SceneEnum.h"


namespace YiaEngine
{
	namespace Scene
	{
		enum class AxisType
		{
			kXAxis,
			kYAxis,
			kZAxis
		};

		//基本的空间变换
		class Transform
		{
		public:
			Transform(Mat4x4f mat, bool object_only = false) :matrix_(mat), object_only_(object_only) {}
			Transform() :matrix_(Mat4x4f::Identity()), object_only_(false) {}
		protected:
			Mat4x4f matrix_;
			bool object_only_;
		};

		class Translation :public Transform
		{
		public:
			Translation() = default;
			Translation(const Vec3f& position):translation_(position)
			{
				matrix_[3] = position;
			}
			Translation(float dis_x, float dis_y, float dis_z) :Transform()
			{
				matrix_[3][0] = dis_x;
				matrix_[3][1] = dis_y;
				matrix_[3][2] = dis_z;
				translation_.xyz = { dis_x,dis_y,dis_z };
			}
			Translation(AxisType axis, float value) :Transform()
			{
				switch (axis)
				{
				case AxisType::kXAxis:
					matrix_[3][0] = value;
					translation_.x = value;
					break;
				case AxisType::kYAxis:
					matrix_[3][1] = value;
					translation_.y = value;
					break;
				case AxisType::kZAxis:
					matrix_[3][2] = value;
					translation_.z = value;
					break;
				default:
					assert(0);
				}
			}
			operator Vec3f()
			{
				return translation_;
			}
			float X() { return translation_.x; };
			float Y() { return translation_.y; };
			float Z() { return translation_.z; };
		private:
			Vec3f translation_;
		};

		class Rotation :public Transform
		{

		};

		class Scale :public Transform
		{

		};
	}//Scene
}//YiaEngine

#endif
