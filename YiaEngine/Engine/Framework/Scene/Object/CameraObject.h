#pragma once
#ifndef SCENE_CAMERA_OBJECT_H
#define SCENE_CAMERA_OBJECT_H



#include"BaseSceneObject.h"
#include"SceneEnum.h"
namespace YiaEngine {
	namespace Scene
	{
		enum class FovType
		{
			kHorizontal,
			kVertical
		};
		enum class CameraType
		{
			Perspective,
			Orthogonal
		};
		class CameraObject :public BaseSceneObject
		{
		public:

			float Aspect()
			{
				return aspect_;
			}
			CameraObject() :BaseSceneObject(SceneObjectType::kCameraObject),
				near_clip_(0.1),far_clip_(1000),aspect_(1),viewAngle(60),fov_type_(FovType::kVertical) {}
			float HorizontalViewAngle()
			{
				if (fov_type_ == FovType::kHorizontal)
					return viewAngle;
				else
					return 2 * atanf((1.f / aspect_) * tanf(viewAngle * 0.5));
			}
			float VerticalViewAngle()
			{
				if (fov_type_ == FovType::kVertical)
					return viewAngle;
				else
					return 2 * atanf(aspect_ * tanf(viewAngle * 0.5));
			}
			float FarClip()
			{
				return far_clip_;
			}
			float NearClip()
			{
				return near_clip_;
			}
			
		private:
			FovType fov_type_;
			float near_clip_;
			float far_clip_;
			float aspect_;
			float viewAngle;
	
		};
	}
}

#endif