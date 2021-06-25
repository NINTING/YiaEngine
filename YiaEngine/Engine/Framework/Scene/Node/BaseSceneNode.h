#pragma once
#ifndef SCENE_NODE_BASESCENE_NODE_H
#define SCENE_NODE_BASESCENE_NODE_H

#include<unique_ptr>


#include"Object/Transform.h"

namespace YiaEngine
{
	namespace Scene
	{
		class BaseSceneNode
		{
		public:
			BaseSceneNode() = default;
			BaseSceneNode(char* name) :name_(name) {}
			BaseSceneNode(const std::string& name) :name_(name) {}



			void AddChildNode(std::unique_ptr<BaseSceneNode>&& node) {
				children_.push_back(std::move(node));
			}
			/*virtual std::string Serialize()
			{
				return std::string{};
			}*/
			virtual ~BaseSceneNode() {}
		protected:
			std::string name_;
			std::vector<std::unique_ptr<BaseSceneNode>>children_;
			std::vector<std::unique_ptr<Transform>>transforms_;
		};
		using EmptyNode = BaseSceneNode;
	}//Scene
}//YiaEngine

#endif // SCENE_NODE_BASESCENE_NODE_H