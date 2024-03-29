#pragma once
#include<d3d12.h>
#include <wrl/client.h>

#include<vector>
#include<memory>
#include<queue>

#include"GpuResource.h"
#include "ThirdParty/D3D12MemoryAllocator/include/D3D12MemAlloc.h"

namespace YiaEngine
{
	namespace Graphic
	{
		/*
		* https://docs.microsoft.com/en-us/windows/win32/direct3d12/memory-management
		*/

		class ResourceAllocatePageManager;
		class ResourceAllocatePage;
		//资源对齐必须是16的倍数
		const int DEFAULT_ALIGMENT = 256;
		enum AllocateType : int
		{
			kDefault = 0,
			kUpload = 1,
			kTypeNum = 2

		};
		enum : UINT64
		{
			kGpuAllocatorSize = 0x10000,	//64kb
			kCpuAllocatorSize = 0x20000,	//2Mb
		};
		
		struct AllocateBuffer
		{
			AllocateBuffer(GpuResource& buffer, UINT64 offset,UINT64 size,void* cpuAddress = nullptr, D3D12_GPU_VIRTUAL_ADDRESS gpuAddress = -1)
			:Buffer(buffer),Offset(offset),Size(size),CpuAddress(cpuAddress),GpuAddress(gpuAddress){}

			ID3D12Resource* GetResource() { return Buffer.NativeResource(); }
			
			GpuResource& Buffer;
			UINT64 Offset;
			UINT64 Size;
			void* CpuAddress;
			D3D12_GPU_VIRTUAL_ADDRESS GpuAddress;
		};
		class ResourceAllocatePage :public GpuResource
		{
		
		public:
			ResourceAllocatePage(ID3D12Resource* NativeResource, D3D12_RESOURCE_STATES usage)
			{
				resource_.Attach(NativeResource);
				state_ = usage;
				resource_->Map(0, nullptr, &Cpu_address_);
				Gpu_address_ = NativeResource->GetGPUVirtualAddress();
			}
			~ResourceAllocatePage()
			{
				if (Cpu_address_ != nullptr)
				{
					resource_->Unmap(0, nullptr);
					Cpu_address_ = nullptr;
				}
			}
			void Unmap()
			{
				if (Cpu_address_)
				{
					resource_->Unmap(0,nullptr);
					Cpu_address_ = nullptr;
				}
			}
			void* Cpu_address_;
			D3D12_GPU_VIRTUAL_ADDRESS Gpu_address_;
			
		};
		class ResourceAllocatePageManager
		{
		public:
			ResourceAllocatePageManager(AllocateType type);
			ResourceAllocatePage* RequestPage();
			ResourceAllocatePage* RequestResource();
			void DiscradPages(UINT64 fence,const std::vector<ResourceAllocatePage*>&lists);
			void DeleteLargePages(UINT64 fence, const std::vector<ResourceAllocatePage*>& lists);
	
			ResourceAllocatePage* CreateNewPage(UINT64 page_size = 0);
			void FreeLargePage(UINT64 fence,std::vector<ResourceAllocatePage*>list);
		private:
			AllocateType type_;
			std::vector<std::unique_ptr<ResourceAllocatePage>> page_pool_;
			std::queue <ResourceAllocatePage*>ready_queue_;
			std::queue <std::pair<UINT64,ResourceAllocatePage*>>retired_queue_;
			std::queue <std::pair<UINT64, ResourceAllocatePage*>>delete_queue;
		};
		/// <summary>
		/// ResourceAllocator主要是用于在一次执行任务,用于分配生命周期为一次任务的资源。
		/// </summary>
		class ResourceAllocator
		{
		public:
			ResourceAllocator(AllocateType type):type_(type)
			{
				page_size_ = type == kDefault ? kCpuAllocatorSize : kGpuAllocatorSize;
				cur_page_ = nullptr;
				cur_offset_ = 0;
			}
			AllocateBuffer Allocate(UINT64 alloc_size,int aligment = DEFAULT_ALIGMENT);
			void FreeResource(UINT64 fence);
		
		private:
			
			AllocateType type_;
			UINT64 page_size_;
			UINT64 cur_offset_;
			ResourceAllocatePage* cur_page_;
			std::vector<ResourceAllocatePage*>large_page_list_;
			std::vector<ResourceAllocatePage*>page_list_;

			static ResourceAllocatePageManager s_pageManager[(int)AllocateType::kTypeNum];
		};

		class TransientResource :GpuResource
		{
			~TransientResource()
			{
				
			}
			void Unmap()
			{
			}
		};

		class TemplateResourceManager {
		public:
			void AddResource(GpuResource* res);
			void FreeResource(UINT64 fence);           
			void RecoverResource();
		private:
			std::queue <std::pair<UINT64, GpuResource*>>deleteQueue;
			std::vector<GpuResource*>resourceList_;
			
		};

		class GpuResourceAllocator
		{
		public:
			void Init(ID3D12Device*,IDXGIAdapter* adater);
			HRESULT CreateResource(
				const D3D12MA::ALLOCATION_DESC* pAllocDesc,
				const D3D12_RESOURCE_DESC* pResourceDesc,
				D3D12_RESOURCE_STATES InitialResourceState,
				const D3D12_CLEAR_VALUE* pOptimizedClearValue,
				D3D12MA::Allocation** ppAllocation,
				ID3D12Resource** ppResource);
		private:

#ifdef DIRECT12
			D3D12MA::Allocator* allocator_;
#endif // DIRECT12

			
		};
	}
}