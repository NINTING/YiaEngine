#pragma once
#include "Core/YiaGraphic.h"

namespace YiaEngine
{
	namespace YiaFrameGraph
	{
		using VirtualResourceId = UINT;
		using StrHash = size_t;
		struct VirtualResourceHandle
		{

			
			bool operator == (const char* name)
			{
				std::hash<std::string> h;
				return NameHash == h(name);
			}
			bool operator == (const VirtualResourceHandle& handle)
			{
				return Id == handle.Id;
			}

			VirtualResourceId Id;
			StrHash NameHash;
		};

	class VirtualResource
		{
		public:

			VirtualResource(const char* name, Graphic::TextureDescribe desc) 
				:Name(name), Describe(desc) {}
			std::string Name;
			Graphic::TextureDescribe Describe;
			bool imported;
		};

		//记录资源使用情况
		struct ResourceNode
		{
			ResourceNode(int id):ResId(id),RefCount(0){}
			int ResId;
			int RefCount;
		};
	}

}