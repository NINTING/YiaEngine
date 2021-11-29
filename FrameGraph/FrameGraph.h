#pragma once

#include "pch.h"
#include"FrameResource.h"
#include "FramePass.h"
namespace YiaEngine
{
	namespace YiaFrameGraph
	{
		class FrameGraph
		{
		public:
			VirtualResourceHandle CreateTemplateRenderBuffer(const char* name,Graphic::TextureDescribe desc);
			VirtualResourceHandle RegisterResource(const char*name, VirtualResourceHandle handle);
			
			VirtualResourceHandle GetResourceId(const char* name);
			VirtualResource GetResource(const char* name);
			
			std::string GetResourceName(VirtualResourceHandle handle);
		private:
			std::unordered_map<StrHash, VirtualResourceId> resourceIdMap_;
			std::vector<std::unique_ptr<VirtualResource>> resouceList_;
			std::vector<PassNode> passNodeList_;
			std::vector<ResourceNode> resouceNodeList_;
			std::unordered_set<std::string> namedPool;

		};
	}
}



