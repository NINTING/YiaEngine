#pragma once
#include"pch.h"
#include"Graphic.h"
#include"GpuResource.h"
#include"UploadBuffer.h"
namespace YiaEngine
{
	namespace Graphic
	{
		void UploadBuffer::Create(const wchar_t* Name,size_t dataSize)
		{
			Destroy();
            bufferSize_ = dataSize;
            // avoid reading back from it.
            D3D12_HEAP_PROPERTIES HeapProps;
            HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
            HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
            HeapProps.CreationNodeMask = 1;
            HeapProps.VisibleNodeMask = 1;

            // Upload buffers must be 1-dimensional
            D3D12_RESOURCE_DESC ResourceDesc = {};
            ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            ResourceDesc.Width = bufferSize_;
            ResourceDesc.Height = 1;
            ResourceDesc.DepthOrArraySize = 1;
            ResourceDesc.MipLevels = 1;
            ResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
            ResourceDesc.SampleDesc.Count = 1;
            ResourceDesc.SampleDesc.Quality = 0;
            ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
            ResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

            ASSERT_SUCCEEDED(g_Device->CreateCommittedResource(&HeapProps, D3D12_HEAP_FLAG_NONE, &ResourceDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource_)));

            gpuVirtualAddress_ = resource_->GetGPUVirtualAddress();
            resource_->SetName(Name);
		}
        void* UploadBuffer::Map()
        {
            void* Memory;
            auto range = CD3DX12_RANGE(0, bufferSize_);

            resource_->Map(0, &range, &Memory);
            return Memory;
        }
        void UploadBuffer::UnMap(size_t begin, size_t end)
        {
            auto range = CD3DX12_RANGE(begin, std::min(end, bufferSize_));
            resource_->Unmap(0, &range);
        }
	}
}