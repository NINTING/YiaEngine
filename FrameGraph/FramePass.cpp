#include "pch.h"
#include "FramePass.h"
#include "FrameGraph/FrameGraph.h"
namespace YiaEngine
{
	namespace YiaFrameGraph
	{
		void FramePass::AddInputResource(const VirtualResourceHandle& handle)
		{
			node_.AddInputResource(handle);
		}

		VirtualResourceHandle FramePass::AddOutput(FrameGraph& fg, const VirtualResourceHandle& handle)
		{
			auto outHandle = fg.AddResourceNode(handle);
			ResourceNode& node = fg.GetResourceNode(outHandle);
			node.WritePass = &node_;
			node_.AddOutputResource(outHandle);
			return outHandle;
		}

		void PassNode::AddInputResource(const VirtualResourceHandle& handle)
		{
			reads.push_back(handle);
		}

		void PassNode::AddOutputResource(const VirtualResourceHandle& handle)
		{
			writes.push_back(handle);
		}

	}
}

