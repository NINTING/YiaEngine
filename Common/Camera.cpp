
#include<math.h>

#include "Camera.h"

namespace YiaEngine
{

	float Camera::HorizontalViewAngle()
	{
		if (fov_type_ == FovType::kHorizontal)
			return viewAngle;
		else
			return 2.f * atanf((1.f / aspect_) * tanf(viewAngle * 0.5f));
	}
	float Camera::VerticalViewAngle()
	{
		if (fov_type_ == FovType::kVertical)
			return viewAngle;
		else
			return 2.f * atanf(aspect_ * tanf(viewAngle * 0.5f));
	}
	float Camera::FarClip()
	{
		return far_clip_;
	}
	float Camera::NearClip()
	{
		return near_clip_;
	}
	float Camera::Aspect()
	{
		return aspect_;
	}

#pragma region Transform
	Math::Mat4x4f Camera::ViewMatrix()
	{

		right_ = up_.cross(front_);
		right_.normalize();
		up_ = front_.cross(right_);

		float x = -position_.dot(right_);
		float y = -position_.dot(up_);
		float z = -position_.dot(front_);
	



		Math::Mat4x4f ret;
		ret<<
			right_[0],up_[0],front_[0],0.f,
			right_[1],up_[1],front_[1],0.f,
			right_[2],up_[2],front_[2],0.f,
			x, y,z,1.f;
		return ret;
	}
	Math::Mat4x4f Camera::ProjectionMatrix()
	{


		float half_rad = 0.5f * Math::Angle2Rad(VerticalViewAngle());
		float far_clip = FarClip();
		float near_clip = NearClip();
		float xScale = 1.f / (aspect_ * tanf(half_rad));
		float yScale = 1.f / tanf(half_rad);
		float zA = far_clip / (far_clip - near_clip);
		float zB = -(near_clip * far_clip) / (far_clip - near_clip);
		Math::Mat4x4f ret;
		ret <<
			xScale,	0.f,	0.f,0.f,
			0.f,	yScale,	0.f,0.f,
			0.f,	0.f,	zA,	1.f,
			0.f,	0.f,	zB,	0.f;
		return ret;
	}
	Math::Mat4x4f Camera::ViewProjMatrix()
	{
		return ViewMatrix() * ProjectionMatrix();
		
	}
#pragma endregion

	
}