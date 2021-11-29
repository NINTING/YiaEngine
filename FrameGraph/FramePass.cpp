#include "pch.h"
#include "FramePass.h"
namespace YiaEngine
{
	namespace YiaFrameGraph
	{
		void FramePass::AddInputResource(const VirtualResourceHandle& handle)
		{
			node_.AddInputResource(handle);
		}

		void PassNode::AddInputResource(const VirtualResourceHandle& handle)
		{
			
		}

	}
}

