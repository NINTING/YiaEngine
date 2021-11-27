#pragma once

#include"SceneNode.h"
namespace YiaEngine
{
	class Scene
	{
	public:
		void OnRender();
	private:

		std::shared_ptr<SceneNode> rootNode_;
		std::vector<std::shared_ptr<SceneNode>> camera; 
		std::map<UINT, std::shared_ptr<SceneNode>>actorMap;
	};
}

