#pragma once
#include<d3d12.h>
#include <wrl/client.h>

#include<vector>
#include<memory>

#include"GpuResource.h"
#include "ResourceAllocator.h"

#include"Math/CommonMath.h"
namespace YiaEngine
{
	namespace Graphic
	{
       ResourceAllocatorPageManager 
           ResourceAllocator::s_pageManager[AllocateType::kTypeNum] =
            { ResourceAllocatorPageManager(AllocateType::kDefault), ResourceAllocatorPageManager(AllocateType::kUpload))};

        ResourceAllocatorPageManager::ResourceAllocatorPageManager(AllocateType type):type_(type)
        {
        }
        ResourceAllocatorPage*  ResourceAllocatorPageManager::RequestPage()
        {
            ResourceAllocatorPage* page = CreateNewPage();
            page_pool_.emplace_back(page);
            return page;
        }
        ResourceAllocatorPage* ResourceAllocatorPageManager::CreateNewPage(size_t page_size)
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

            D3D12_RESOURCE_STATES default_state = D3D12_RESOURCE_STATE_GENERIC_READ;
            if(type_== AllocateType::kDefault)
            { 
                ResourceDesc.Width = page_size == 0 ? kGpuAllocatorSize : page_size;
                ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
                HeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
            }
            else if(type_ == AllocateType::kUpload)
            { 
                ResourceDesc.Width = page_size == 0 ? kCpuAllocatorSize : page_size;
                HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
                ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
            }
            ID3D12Resource* resource;
            ASSERT_SUCCEEDED(Graphic::g_device->CreateCommittedResource(
                &HeapProps,
                D3D12_HEAP_FLAG_NONE,
                &ResourceDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&resource)));

			return new ResourceAllocatorPage(resource,default_state);
		}
        void ResourceAllocator::Allocate(size_t alloc_size, int aligment)
        {
            size_t align_size = AlignUp(alloc_size, aligment - 1);

            s_pageManager[type_].RequestPage();

        }
    }
}