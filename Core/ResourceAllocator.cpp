
#pragma once
#include"pch.h"
#include<d3d12.h>
#include <wrl/client.h>

#include<vector>
#include<memory>

#include"GpuResource.h"
#include "ResourceAllocator.h"

#include"CommandManager.h"
#include"Common/YiaMath.h"
namespace YiaEngine
{
	namespace Graphic
	{
       ResourceAllocatePageManager 
           ResourceAllocator::s_pageManager[AllocateType::kTypeNum] =
            { ResourceAllocatePageManager(AllocateType::kDefault), ResourceAllocatePageManager(AllocateType::kUpload)};

        ResourceAllocatePageManager::ResourceAllocatePageManager(AllocateType type):type_(type)
        {
        }
        ResourceAllocatePage*  ResourceAllocatePageManager::RequestPage()
        {
            ResourceAllocatePage* ret = nullptr;
            while(!retired_queue_.empty()&& g_commandManager.IsComplete(retired_queue_.front().first))
            { 
                ready_queue_.push(retired_queue_.front().second);
                retired_queue_.pop();
              
            }
            if (!ready_queue_.empty())
            {
                ret = ready_queue_.front();
                ready_queue_.pop();
            }
            else
            {
                ret = CreateNewPage();
                page_pool_.emplace_back(ret);
            }
            return ret;
        }
        void ResourceAllocatePageManager::DiscradPages(UINT64 fence, const std::vector<ResourceAllocatePage*>& lists)
        {
            for (size_t i = 0; i < lists.size(); i++)
            {
                
                retired_queue_.push(std::make_pair(fence, lists[i]));
            }
        }
        void ResourceAllocatePageManager::DeleteLargePages(UINT64 fence, const std::vector<ResourceAllocatePage*>& lists)
        {
          
            while (!delete_queue.empty() && g_commandManager.IsComplete(delete_queue.front().first))
            {
                delete delete_queue.front().second;
                delete_queue.pop();
            }

            for (size_t i = 0; i < lists.size(); i++)
            {
                lists[i]->Unmap();
                delete_queue.push(std::make_pair(fence, lists[i]));
            }
        }
        ResourceAllocatePage* ResourceAllocatePageManager::CreateNewPage(UINT64 page_size)
		{
            D3D12_HEAP_PROPERTIES HeapProps;
            HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            HeapProps.CreationNodeMask = 1;
            HeapProps.VisibleNodeMask = 1;

            D3D12_RESOURCE_DESC ResourceDesc;
            ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            ResourceDesc.Alignment = 0;
            ResourceDesc.Height = 1;
            ResourceDesc.DepthOrArraySize = 1;
            ResourceDesc.MipLevels = 1;
            ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
            ResourceDesc.SampleDesc.Count = 1;
            ResourceDesc.SampleDesc.Quality = 0;
            ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

            D3D12_RESOURCE_STATES default_state ;
            if(type_== AllocateType::kDefault)
            { 
                ResourceDesc.Width = page_size == 0 ? kGpuAllocatorSize : page_size;
                ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
                HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
                default_state = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
            }
            else if(type_ == AllocateType::kUpload)
            { 
                ResourceDesc.Width = page_size == 0 ? kCpuAllocatorSize : page_size;
                HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
                ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
                default_state = D3D12_RESOURCE_STATE_GENERIC_READ;
            }
            ID3D12Resource* rawResource = nullptr;
            ASSERT_SUCCEEDED(Graphic::g_Device->CreateCommittedResource(
                &HeapProps,
                D3D12_HEAP_FLAG_NONE,
                &ResourceDesc,
                default_state,
                nullptr,
                IID_PPV_ARGS(&rawResource)));

			return new ResourceAllocatePage(rawResource,default_state);
		}
        void ResourceAllocatePageManager::FreeLargePage(UINT64 fence, std::vector<ResourceAllocatePage*> list)
        {
        }
        AllocateBuffer ResourceAllocator::Allocate(UINT64 alloc_size, int aligment)
        {
            UINT64 align_size = Math::AlignUp(alloc_size, aligment - 1);

            if (align_size > page_size_)
            {
                //TODO: {Yia} 分配空间过大
                ResourceAllocatePage* large_page = s_pageManager[type_].CreateNewPage(align_size);
                large_page_list_.push_back(large_page);
                return AllocateBuffer(*large_page, 0, alloc_size,
                    (UINT8*)large_page->Cpu_address_,
                    large_page->Gpu_address_);
            }
            cur_offset_ = Math::AlignUp(cur_offset_, aligment - 1);
            if (cur_offset_ + align_size > page_size_)
            {
                page_list_.push_back(cur_page_);
                cur_page_ = nullptr;
            }
            if (cur_page_ == nullptr)
            {
                cur_page_ = s_pageManager[type_].RequestPage();
                
                cur_offset_ = 0;
            }
           
            AllocateBuffer ret(*cur_page_,cur_offset_,alloc_size,
                (UINT8*)cur_page_->Cpu_address_ + cur_offset_,
                cur_page_->Gpu_address_+ cur_offset_);
            cur_offset_ += alloc_size;
            return ret;
        }
        void ResourceAllocator::FreeResource(UINT64 fence)
        {
            if (cur_page_ == nullptr)
                return;
            page_list_.push_back(cur_page_);
            s_pageManager[type_].DiscradPages(fence,page_list_);
            page_list_.clear();
            
            s_pageManager[type_].FreeLargePage(fence, large_page_list_);
            large_page_list_.clear();

            cur_page_ = nullptr;
            cur_offset_ = 0;
        }
    }
}


