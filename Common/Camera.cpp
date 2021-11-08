#include"pch.h"
#include<math.h>

#include "Camera.h"
#include <Common/TimeStep.h>
#include"Common/Input.h"
#include"Common/Logger.h"
namespace YiaEngine
{

	float Camera::HorizontalViewRad()
	{
		if (fov_type_ == FovType::kHorizontal)
			return Math::Angle2Rad(viewAngle);
		else
			return 2.f * atanf((1.f / aspect_) * tanf(Math::Angle2Rad( viewAngle) * 0.5f));
	}
	float Camera::VerticalViewRad()
	{
		if (fov_type_ == FovType::kVertical)
			return Math::Angle2Rad(viewAngle);
		else
			return 2.f * atanf(aspect_ * tanf(Math::Angle2Rad(viewAngle) * 0.5f));
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

	void Camera::SetViewport(float width, float Height, float topLeftX, float topLeftY, float minDepth, float maxDepth)
	{
		viewport_.Width = width;
		viewport_.Height = Height;
		viewport_.TopLeftX= topLeftX;
		viewport_.TopLeftY = topLeftY;
		viewport_.MinDepth = minDepth;
		viewport_.MaxDepth = maxDepth;
		near_clip_ = minDepth;
		far_clip_ = maxDepth;
		aspect_ = width / Height;
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


		float half_rad = 0.5f * VerticalViewRad();
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
	void Camera::Strafe(float d)
	{
		position_ += right_ * d;
	}
	void Camera::Walk(float d)
	{
		position_ += front_ * d;
	}
	void Camera::Rotation(Math::Vec3f axis,Math::Vec3f dir,float angle)
	{
		
	}
	
	void Camera::EulerRotation(float yaw,float Pitch,float roll)
	{
		Eigen::Vector3f axis(yaw, Pitch, roll);
		Eigen::AngleAxisf rollAngle(Eigen::AngleAxisf(axis(2), Eigen::Vector3f::UnitZ()));
		Eigen::AngleAxisf pitchAngle(Eigen::AngleAxisf(axis(1), Eigen::Vector3f::UnitX()));
		Eigen::AngleAxisf yawAngle(Eigen::AngleAxisf(axis(0), Eigen::Vector3f::UnitY()));
		Math::Mat3x3f rotation_matrix; rotation_matrix = yawAngle * pitchAngle * rollAngle;
		front_ = front_ * rotation_matrix;
		up_ = up_ * rotation_matrix;
		right_ = right_ * rotation_matrix;


	
	};
	void Camera::Update(Timestep timestep)
	{
		
		if (Input::IsKeyPress(VirtualKey::RightMouse))
		{
			Math::Vec2f moveDir = Input::MouseMove();
			float xAngle =moveDir.x() / GetCameraWidth() * HorizontalViewRad() * 2;
			float yAngle =moveDir.y() / GetCameraHeight() * VerticalViewRad();
			//EulerRotation(xAngle,yAngle,0);
		
			YIA_INFO("[{0},{1}]", xAngle, yAngle);
			if (yAngle != 0)
			{
				front_ = Math::RotationByAxis(right_,front_, yAngle);
				up_ = Math::RotationByAxis(right_, up_, yAngle);
			}
			if (xAngle != 0)
			{
				front_ = Math::RotationByAxisY(front_, xAngle);
				right_ = Math::RotationByAxisY(right_, xAngle);
				up_ = Math::RotationByAxisY(up_, xAngle);
			}
		}

		if (Input::IsKeyPress(VirtualKey::Key_D))
		{

			Strafe(timestep.GetSeconds());
		}
		if (Input::IsKeyPress(VirtualKey::Key_A))
		{
			Strafe(-timestep.GetSeconds());
		}
		if (Input::IsKeyPress(VirtualKey::Key_W))
		{
			Walk(timestep.GetSeconds());

		}
		if (Input::IsKeyPress(VirtualKey::Key_S))
		{
			Walk(-timestep.GetSeconds());
		}
	}
#pragma endregion

	
}