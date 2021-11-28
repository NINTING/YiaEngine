#pragma once

#include "pch.h"
#include"FrameResource.h"
#include "FramePass.h"
namespace YiaEngine
{
	namespace FrameGraph
	{
		class FrameGraph
		{
			template<class T>
			VirtualResourceId CreateTemplateResource(const char* name,Graphic::TextureDescribe desc);
			VirtualResourceId RegisterResource(const char*name, VirtualResourceId id);
			
			VirtualResourceId GetResourceId(const char* name);
			VirtualResource GetResource(const char* name);



			std::unordered_map<std::string, VirtualResourceId> resourceIdMap_;
			std::vector<std::unique_ptr<VirtualResource>> resouceList_;
			std::vector<PassNode> passNodeList_;
			std::vector<ResourceNode> resouceNodeList_;

		};
	}
}



