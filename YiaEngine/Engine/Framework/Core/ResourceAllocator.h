#pragma once
#include<d3d12.h>
#include <wrl/client.h>

#include<vector>
#include<memory>

#include"GpuResource.h"

namespace YiaEngine
{
	namespace Graphic
	{
		//资源对齐必须是16的倍数
		const int DEFAULT_ALIGMENT = 256;
		enum AllocateType : int
		{
			kDefault = 0,
			kUpload = 1,
			kTypeNum = 2

		};
		enum
		{
			kGpuAllocatorSize = 0x10000,	//64kb
			kCpuAllocatorSize = 0x20000,	//2Mb
		};
		class ResourceAllocatorPage : GpuResource
		{
		public:
			ResourceAllocatorPage(ID3D12Resource* resource, D3D12_RESOURCE_STATES usage)
			{
				resource_.Attach(resource);
				usage_ = usage;
				resource->Map(0, nullptr, &cpu_address_);
				gpu_address_ = resource->GetGPUVirtualAddress();
			}
		private:
			void* cpu_address_;
			D3D12_GPU_VIRTUAL_ADDRESS gpu_address_;
			
		};
		class ResourceAllocatorPageManager
		{
		public:
			ResourceAllocatorPageManager(AllocateType type);
			ResourceAllocatorPage* RequestPage();
			ResourceAllocatorPage* RequestResource();
			ResourceAllocatorPage* CreateNewPage(size_t page_size = 0);
			
		private:
			AllocateType type_;
			std::vector<std::unique_ptr<ResourceAllocatorPage>> page_pool_;
				
		};
		class ResourceAllocator
		{
		public:
			void Allocate(size_t alloc_size,int aligment = DEFAULT_ALIGMENT);
		private:
			AllocateType type_;
			static ResourceAllocatorPageManager s_pageManager[(int)AllocateType::kTypeNum];
		};
	}
}