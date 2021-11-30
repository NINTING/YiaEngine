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
			VirtualResourceHandle RegisterResource(const char* name,std::shared_ptr<Graphic::GpuTexture> handle);
			
			VirtualResourceHandle AddResourceNode(const VirtualResourceHandle& handle);
			VirtualResourceHandle AddResourceNode(const char* name);
			
			VirtualResourceHandle GetResourceHandle(const char* name);
			VirtualResource GetResource(const char* name);
			const PassNode& GetPass(const char* name);
			ResourceNode& GetResourceNode(const VirtualResourceHandle& handle);
			std::string GetResourceName(VirtualResourceHandle handle);
			void AddPass(const char* name,FramePass& framePass);

		
		private:
			void Setup();
			void Compile();
			void Execute();
		private:
			std::unordered_map<std::string, VirtualResourceId> resourceIdMap_;
			std::unordered_map<std::string, int> passIdMap_;
			std::vector<std::unique_ptr<VirtualResource>> resouceList_;
			std::vector<PassNode> passNodeList_;
			std::vector<ResourceNode> resouceNodeList_;
			std::unordered_set<std::string> namedPool;
			
		};
	}
}



