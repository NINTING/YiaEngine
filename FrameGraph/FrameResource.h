#pragma once
#include "Core/YiaGraphic.h"

namespace YiaEngine
{
	namespace YiaFrameGraph
	{
		class PassNode;
		using VirtualResourceId = UINT;
		using StrHash = size_t;
		struct VirtualResourceHandle
		{
			VirtualResourceHandle() = default;
			VirtualResourceHandle(StrHash name, VirtualResourceId id) :NameHash(NameHash), Id(id) {}
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
				:Name(name), Describe(desc),imported(false),resource(nullptr) {}
			VirtualResource(const char* name, std::shared_ptr<Graphic::GpuTexture> res)
				:Name(name),  resource(res) {
				YIA_ASSERT(res != nullptr,"Texture Resource Not Nullptr");
			Describe = res->GetDescribe();
				res == nullptr ? imported = true : imported = false;
			}
			std::string Name;
			Graphic::TextureDescribe Describe;
			std::shared_ptr<Graphic::GpuTexture> resource;

			bool imported;
		};

		//记录资源使用情况
		struct ResourceNode
		{
			ResourceNode(int id):ResId(id),RefCount(0){}
			int ResId;
			int RefCount;
			PassNode* WritePass;
			int ReadCount;;
		};
	}

}