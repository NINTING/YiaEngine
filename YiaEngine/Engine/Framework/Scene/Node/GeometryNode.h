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
		class GeometryNode :BaseSceneNode
		{
		public:
			GeometryNode() = default;
			//GeometryNode(const GeometryNode &){}
			GeometryNode(const std::shared_ptr<GeometryObject>& object) :
				object_ref_(object) {}
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
			/*std::string Serialize()
			{
				std::stringstream ss;
				std::string ret;
				ss << BaseSceneNode::Serialize();
				ss << "GeometryNode:\n";
				ss << "visible: " << visible_;
				ss << "shadow: " << shadow_;
				ss << "motion blur: " << motion_blur_;

				ss >> ret;
				return ret;
			}*/
		private:
			bool visible_;
			bool shadow_;
			bool motion_blur_;
			//每一个LOD一个网格
			//one mesh for each level of detail 
			std::shared_ptr<GeometryObject>object_ref_;
			std::vector<std::shared_ptr<MaterialObject>>materials_;
		};

	}//Scene
}//YiaEngine

#endif // SCENE_NODE_GEOMETRY_NODE_H