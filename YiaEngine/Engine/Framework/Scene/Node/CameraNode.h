#pragma once
#ifndef SCENE_NODE_CAMERA_NODE_H
#define SCENE_NODE_CAMERA_NODE_H

#include<memory>


#include"BaseSceneNode.h"


namespace YiaEngine
{
	namespace Scene
	{
		class CameraNode : BaseSceneNode
		{
			using Object_ptr = std::shared_ptr<CameraObject>;
		public:
			CameraNode():object_ref_(new CameraObject()),front_(Vec3f(0,0,1)){}
			CameraNode(Vec3f postion):position_(postion){}
			//CameraNode(const std::shared_ptr<CameraNode>&)
			CameraNode(const std::shared_ptr<CameraObject>& object) :
				object_ref_(object) {}
			CameraNode(std::shared_ptr<CameraObject>&& object) :
				object_ref_(std::move(object)) {}

			const std::shared_ptr<CameraObject> object() { return object_ref_; }
			void set_object(const std::shared_ptr<CameraObject>& object) { object_ref_ = object; }
			Vec3f front() { return front_; };
			void set_front_(const Vec3f& front) { front_ = front; }
			Mat4x4f ViewMatrix()
			{
				Vec3f up{ 0,1,0 };
				Vec3f front = front_;
				Vec3f right = Cross(up, front);
				up = Cross(front, right);
				Mat4x4f ret{
					right.x,up.x,front.x,0,
					right.y,up.y,front.y,0,
					right.z,up.z,front.z,0,
					-position_.X(),-position_.Y(),-position_.Z(),0
				};
				return ret;
			}
			Mat4x4f ProjectionMatrix()
			{
				float aspect = object_ref_->Aspect();
				float half_verticalViewAngle =0.5f * object_ref_->VerticalViewAngle();
				auto far_clip = object_ref_->FarClip();
				float near_clip = object_ref_->NearClip();
				Mat4x4f ret({
					1.f / (aspect * tanf(half_verticalViewAngle)),0,0,0,
					0,1.f / tanf(half_verticalViewAngle),0,0,
					0,0,far_clip / (far_clip - near_clip),1.f,
					0,0,-(near_clip * far_clip) / (far_clip - near_clip),0
				});
				return ret;
			}
			Mat4x4f ViewProjMatrix()
			{
				return ViewMatrix() * ProjectionMatrix();
			}
		private:
			std::shared_ptr<CameraObject>object_ref_;
			Vec3f front_;
			Translation position_;

		};

	}//Scene
}//YiaEngine

#endif // SCENE_NODE_CAMERA_NODE_H