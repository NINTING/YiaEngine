#pragma once
#ifndef FRAMEWORk_SCENE_NODE
#define FRAMEWORk_SCENE_NODE

#include <string>
#include<vector>

#include"SceneObject.h"
#include "Math/Geometry.h"

namespace YiaEngine
{
	class BaseSceneNode
	{
	public:
		BaseSceneNode() = default;
		BaseSceneNode(char*name):name_(name){}
		BaseSceneNode(const std::string& name):name_(name){}
		
	
		
		void AddChildNode(std::unique_ptr<BaseSceneNode>&& node) {
			children_.push_back(std::move(node));
		}
		virtual ~BaseSceneNode() {}
	protected:
		std::string name_;
		std::vector<std::unique_ptr<BaseSceneNode>>children_;
		std::vector<std::unique_ptr<Transform>>transforms_;
	};
	using EmptyNode = BaseSceneNode;

	class GeometryNode :BaseSceneNode
	{
	public:
		GeometryNode() = default;
		//GeometryNode(const GeometryNode &){}
		GeometryNode(const std::shared_ptr<GeometryObject>&object):
			object_ref_(object){}
		GeometryNode(std::shared_ptr<GeometryObject>&& object) :
			object_ref_(std::move(object)) {}
		const std::shared_ptr<GeometryObject> object_ref() { return object_ref_; }
		~GeometryNode() {}
		void set_object_ref(const std::shared_ptr<GeometryObject>& object)
		{
			object_ref_ = object;
		}
		void AddMatrial(const std::shared_ptr<MaterialObject>& material)
		{
			materials_.push_back(material);
		}
	private:
		bool visible_;
		bool shadow_;
		bool motion_blur_;
		//每一个LOD一个网格
		//one mesh for each level of detail 
		std::shared_ptr<GeometryObject>object_ref_;
		std::vector<std::shared_ptr<MaterialObject>>materials_;
	};
	class CameraNode : BaseSceneNode
	{
		CameraNode() = default;
		//CameraNode(const std::shared_ptr<CameraNode>&)
		CameraNode(const std::shared_ptr<CameraObject>&object):
			object_ref_(object){}
		CameraNode(std::shared_ptr<CameraObject>&& object):
			object_ref_(std::move(object)){}
		
		const std::shared_ptr<CameraObject> object_ref() { return object_ref_; }
		void set_object_ref(const std::shared_ptr<CameraObject>& object) { object_ref_ = object; }
		Vec3f target() { return target_; };
		void set_target_(const Vec3f& target) { target_ = target; }
	private:
		std::shared_ptr<CameraObject>object_ref_;
		Vec3f target_;
	};

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
		Vec3f target()  { return target_; };
		void set_target_(const Vec3f& target) { target_ = target; }
	private:
		std::shared_ptr<LightObject>object_ref_;
		Vec3f target_;
	};
}//YiaEngine

#endif // !FRAMEWORk_SCENE_NODE
