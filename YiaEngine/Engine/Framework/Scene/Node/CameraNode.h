#pragma once
#ifndef SCENE_NODE_CAMERA_NODE_H
#define SCENE_NODE_CAMERA_NODE_H

#include"Object/Transform.h"
#include"BaseSceneNode.h"
#include "Math/Geometry.h"
namespace YiaEngine
{
	namespace Scene
	{
		class CameraNode : BaseSceneNode
		{
			CameraNode() = default;
			//CameraNode(const std::shared_ptr<CameraNode>&)
			CameraNode(const std::shared_ptr<CameraObject>& object) :
				object_ref_(object) {}
			CameraNode(std::shared_ptr<CameraObject>&& object) :
				object_ref_(std::move(object)) {}

			const std::shared_ptr<CameraObject> object_ref() { return object_ref_; }
			void set_object_ref(const std::shared_ptr<CameraObject>& object) { object_ref_ = object; }
			Vec3f target() { return target_; };
			void set_target_(const Vec3f& target) { target_ = target; }
		private:
			std::shared_ptr<CameraObject>object_ref_;
			Vec3f target_;
		};

	}//Scene
}//YiaEngine

#endif // SCENE_NODE_CAMERA_NODE_H