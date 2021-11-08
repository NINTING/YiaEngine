#pragma once

#include"YiaMath.h"
#include"Common/TimeStep.h"
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

	struct Viewport
	{
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
	};
	class Camera
	{
	public:
		
#pragma region  Ù–‘
	public:
		Camera() :near_clip_(0.1f), far_clip_(1000.0f), aspect_(1.f), viewAngle(60.f), fov_type_(FovType::kVertical),up_(Math::Vec3f(0,1,0)), viewport_(){}
		float HorizontalViewAngle();
		float VerticalViewAngle();
		float HorizontalViewRad();
		float VerticalViewRad();
		float FarClip();
		float NearClip();
		float Aspect();
		float GetCameraWidth() {return viewport_.Width; };
		float GetCameraHeight() {return viewport_.Height; };
		void SetViewport(float width, float Height, float topLeftX = 0, float topLeftY = 0, float minDepth = 0.01, float maxDepth = 100);
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
		void SetFarclip(float farClip) { far_clip_ = farClip; };
		void SetNearclip(float nearCip) { near_clip_ = nearCip; };

		Math::Mat4x4f ViewMatrix();
		
		Math::Mat4x4f ProjectionMatrix();
		
		Math::Mat4x4f ViewProjMatrix();


	public:
		void Update(Timestep timestep);
		void Strafe(float d);
		void Walk(float d);
		void Rotation(Math::Vec3f axis, Math::Vec3f dir, float angle);
		void EulerRotation(float yaw, float Pitch, float roll);
		void Rotation(Math::Vec2f dir);
	private:
		Math::Vec3f position_;
		Math::Vec3f front_;
		Math::Vec3f right_;
		Math::Vec3f up_;
		Viewport viewport_;
#pragma endregion

	
	};

}