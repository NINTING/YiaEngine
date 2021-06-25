#pragma once
#ifndef SCENE_NODE_GEOMETRY_NODE_H
#define SCENE_NODE_GEOMETRY_NODE_H

#include"Object/Transform.h"
#include"BaseSceneNode.h"
#include "Math/Geometry.h"
namespace YiaEngine
{
	namespace Scene
	{
		class LightNode : BaseSceneNode
		{
			LightNode() = default;
			//LightNode(const std::shared_ptr<LightNode>&)
			LightNode(const std::shared_ptr<LightObject>& object) :
				object_ref_(object) {}
			LightNode(std::shared_ptr<LightObject>&& object) :
				object_ref_(std::move(object)) {}

			const std::shared_ptr<LightObject> object_ref() { return object_ref_; }
			void set_object_ref(const std::shared_ptr<LightObject>& object) { object_ref_ = object; }
			Vec3f target() { return target_; };
			void set_target_(const Vec3f& target) { target_ = target; }
		private:
			std::shared_ptr<LightObject>object_ref_;
			Vec3f target_;
		};

	}//Scene
}//YiaEngine

#endif // SCENE_NODE_GEOMETRY_NODE_H