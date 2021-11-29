#include "pch.h"
#include "FrameGraph.h"
#include"Core/YiaGraphic.h"
namespace YiaEngine
{
	namespace YiaFrameGraph
	{

		VirtualResourceHandle  FrameGraph::CreateTemplateRenderBuffer(const char* name, Graphic::TextureDescribe desc)
		{
			int resIndex = resouceList_.size();
			VirtualResourceHandle handle;
			handle.Id = resouceNodeList_.size();
			
			namedPool.insert(name);
			handle.NameHash = namedPool.hash_function()(name);
			
			ResourceNode resNode(resIndex);
			resouceNodeList_.emplace_back(resNode);
			resouceList_.emplace_back(std::make_unique<VirtualResource>(name,desc));
			resourceIdMap_[handle.NameHash] = handle.Id;
			
			return handle;
		}

		VirtualResourceHandle  FrameGraph::RegisterResource(const char* name, VirtualResourceHandle handle)
		{

		}

		VirtualResourceHandle  FrameGraph::GetResourceId(const char* name)
		{

		}

		VirtualResource FrameGraph::GetResource(const char* name)
		{

		}

	}
}

