#pragma once
#ifndef SCENE_NODE_CAMERA_NODE_H
#define SCENE_NODE_CAMERA_NODE_H

#include<memory>


#include"BaseSceneNode.h"
#include"Object/Transform.h"
#include"Object/CameraObject.h"

namespace YiaEngine
{
	namespace Scene
	{
		class CameraNode : BaseSceneNode
		{
			using Object_ptr = std::shared_ptr<CameraObject>;
		public:
			CameraNode():object_ref_(new CameraObject()),front_(Vec3f(0,0,1)), up_(Vec3f(0, 1, 0)){}
			CameraNode(Vec3f postion):object_ref_(new CameraObject()),position_(postion),front_(Vec3f(0, 0, 1)), up_(Vec3f(0, 1, 0)) {}

			//CameraNode(const std::shared_ptr<CameraNode>&)
			CameraNode(const std::shared_ptr<CameraObject>& object) :
				object_ref_(object) {}
			CameraNode(std::shared_ptr<CameraObject>&& object) :
				object_ref_(std::move(object)) {}

			const std::shared_ptr<CameraObject> object() { return object_ref_; }
			void set_object(const std::shared_ptr<CameraObject>& object) { object_ref_ = object; }
			Vec3f front() { return front_; };
			void set_front(const Vec3f& front) { 
				front_ = Normalize(front); 
			}
			
			

			Mat4x4f ViewMatrix()
			{
				Vec3f front = front_;
				Vec3f right = Normalize(Cross(up_, front));
				
				up_ = Cross(front, right);

				float x = -Dot(Vec3f(position_), right);
				float y = -Dot(Vec3f(position_), up_);
				float z = -Dot(Vec3f(position_), front);

				Mat4x4f ret{
					right.x,up_.x,front.x,0,
					right.y,up_.y,front.y,0,
					right.z,up_.z,front.z,0,
					x, y,z,1
				};
				return ret;
			}
			Mat4x4f ProjectionMatrix()
			{
				float aspect = object_ref_->Aspect();
				
				float half_rad = 0.5f * Angle2Rad(object_ref_->VerticalViewAngle());
				float far_clip = object_ref_->FarClip();
				float near_clip = object_ref_->NearClip();
				Mat4x4f ret({
					1.f / (aspect * tanf(half_rad)),0,0,0,
					0,1.f / tanf(half_rad),0,0,
					0,0,far_clip / (far_clip - near_clip),1.f,
					0,0,-(near_clip * far_clip) / (far_clip - near_clip),0
				});
				return ret;
			}
			Mat4x4f ViewProjMatrix()
			{
				return ViewMatrix() * ProjectionMatrix();
			}
			void MoveForward(float distance)
			{
				Walk(front_ * distance);
			}
			void Walk(Vec3f dir)
			{
				position_ = position_ + dir;
			}
			void RotationEuler(float x,float y, float z)
			{
				float cx, sx;
				float cy, sy;
				float cz, sz;
				cx = cosf(-x); sx = sinf(-x);
				cy = cosf(-y); sy = sinf(-y);
				cz = cosf(-z); sz = sinf(-z);
		

				Mat3x3f R = {cy*cz+sy*sx*sz,-cy*sz+sy*sx*cz,sy*cx,
							 sz*cx,			 cz*cx,			-sx,	
							 -sy*cz+cy*sx*sz,sz*sy+cy*sx*cz,cy*cx};
				front_ =Normalize( front_ * R);
				up_ = Normalize( up_ * R);
			}

		private:
			std::shared_ptr<CameraObject>object_ref_;
			Vec3f front_;
			Vec3f up_;
			Translation position_;

		};

	}//Scene
}//YiaEngine

#endif // SCENE_NODE_CAMERA_NODE_H