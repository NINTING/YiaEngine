#pragma once

#include"pch.h"
#include"GameObject.h"
namespace YiaEngine
{
	


	class SceneNode
	{
		std::shared_ptr<SceneNode> parent;
		std::vector<std::shared_ptr<SceneNode>> childList;
		GameObject gameObject;
	public:
		void Render();
	};

}