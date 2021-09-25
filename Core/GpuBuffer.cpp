#pragma once
#include"pch.h"
#include"Utility.h"
#include"Graphic.h"
#include"GpuBuffer.h"
#include"CommandContext.h"
namespace YiaEngine:: Graphic
{
 
    void GpuBuffer::Create(size_t data_size, size_t stride, void* data)
	{
        D3D12_RESOURCE_DESC desc = DescriptBuffer();
        D3D12_HEAP_PROPERTIES heap_prop;
        heap_prop.Type = D3D12_HEAP_TYPE_DEFAULT;
        heap_prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heap_prop.CreationNodeMask = 1;
        heap_prop.VisibleNodeMask = 1;
        heap_prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        buffer_size_ = data_size;
        ASSERT_SUCCEEDED(Graphic::g_Device->CreateCommittedResource(
            &heap_prop,
            D3D12_HEAP_FLAG_NONE,
            &desc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&resource_)));
        if (data != nullptr)
        {
          /*  CommandContext* initContext = CommandContext::Begin(L"InitBuffer");
            initContext->GetAllocateUploadBuffer(data_size);*/
        }
            
	}

    D3D12_VERTEX_BUFFER_VIEW GpuBuffer::VertexBufferView(size_t offset, size_t stride, size_t dataSize)
    {
        D3D12_VERTEX_BUFFER_VIEW vbo;
        vbo.BufferLocation = gpuVirtualAddress_;
        vbo.StrideInBytes = stride;
        vbo.SizeInBytes = dataSize;
        return vbo;
    }
    D3D12_INDEX_BUFFER_VIEW GpuBuffer::IndexBufferView(size_t offset, size_t stride, size_t dataSize)
    {
        D3D12_INDEX_BUFFER_VIEW ibo;
        ibo.BufferLocation = gpuVirtualAddress_ + offset;
        ibo.Format = DXGI_FORMAT_R32_UINT;;
        ibo.SizeInBytes = dataSize;
        return ibo;
    }
    D3D12_RESOURCE_DESC GpuBuffer::DescriptBuffer()
	{
        D3D12_RESOURCE_DESC Desc = {};
        Desc.Alignment = 0;
        Desc.DepthOrArraySize = 1;
        Desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        Desc.Flags = resource_flag_;
        Desc.Format = DXGI_FORMAT_UNKNOWN;
        Desc.Height = 1;
        Desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        Desc.MipLevels = 1;
        Desc.SampleDesc.Count = 1;
        Desc.SampleDesc.Quality = 0;
        Desc.Width = (UINT64)buffer_size_;
        return Desc;
	}
}