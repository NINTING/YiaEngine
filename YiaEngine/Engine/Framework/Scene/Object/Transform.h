#ifndef FRAMEWORK_SCENE_TRANSFORM_H
#define FRAMEWORK_SCENE_TRANSFORM_H

#include "Math/Geometry.h"
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
			Translation(float dis_x, float dis_y, float dis_z) :Transform()
			{
				matrix_[3][0] = dis_x;
				matrix_[3][1] = dis_y;
				matrix_[3][2] = dis_z;
			}
			Translation(AxisType axis, float value) :Transform()
			{
				switch (axis)
				{
				case AxisType::kXAxis:
					matrix_[3][0] = value;
					break;
				case AxisType::kYAxis:
					matrix_[3][1] = value;
					break;
				case AxisType::kZAxis:
					matrix_[3][2] = value;
					break;
				default:
					assert(0);
				}

			}
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
