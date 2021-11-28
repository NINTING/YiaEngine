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

		//��¼��Դʹ�����
		class ResourceNode
		{
			ResourceNode(VirtualResource* resource):Resource(resource),RefCount(0){}
			VirtualResource* Resource;
			int RefCount;
		};
	}

}