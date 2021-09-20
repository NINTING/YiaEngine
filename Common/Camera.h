#pragma once

#include"YiaMath.h"
namespace YiaEngine
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
	class Camera
	{
	public:
		
#pragma region  Ù–‘
	public:
		Camera() :near_clip_(0.1f), far_clip_(1000.0f), aspect_(1.f), viewAngle(60.f), fov_type_(FovType::kVertical) {}
		float HorizontalViewAngle();
		float VerticalViewAngle();
		float FarClip();
		float NearClip();
		float Aspect();
	private:
		FovType fov_type_;
		float near_clip_;
		float far_clip_;
		float aspect_;
		float viewAngle;
#pragma endregion

#pragma region Transform
	public:

		Math::Vec3f Front() { return front_; };
		void Front(const Math::Vec3f& front) { front_ = front; };
		Math::Vec3f Position() { return position_; };
		void Position(const Math::Vec3f& pos) {position_ = pos;}


		Math::Mat4x4f ViewMatrix();
		
		Math::Mat4x4f ProjectionMatrix();
		
		Math::Mat4x4f ViewProjMatrix();

	private:
		Math::Vec3f position_;
		Math::Vec3f front_;
		Math::Vec3f right_;
		Math::Vec3f up_;
#pragma endregion

	
	};

}