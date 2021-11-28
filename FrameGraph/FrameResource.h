#pragma once
#include "Core/YiaGraphic.h"

namespace YiaEngine
{
	namespace FrameGraph
	{
		using VirtualResourceId = UINT;

		class VirtualResource
		{


			VirtualResource(const char* name, Graphic::TextureDescribe);

			Graphic::TextureDescribe describe;
			bool imported;
		};

		//记录资源使用情况
		class ResourceNode
		{
			ResourceNode(VirtualResource* resource):Resource(resource),RefCount(0){}
			VirtualResource* Resource;
			int RefCount;
		};
	}

}