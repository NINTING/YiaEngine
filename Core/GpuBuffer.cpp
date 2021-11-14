#pragma once
#include"pch.h"
#include"Utility.h"
#include"Graphic.h"
#include"GpuBuffer.h"
#include"CommandContext.h"
#include"Common/YiaMath.h"
#include"Common/Utility.h"
namespace YiaEngine:: Graphic
{
 
    void GpuBuffer::Create(const char* Name, size_t Size, void* data)
	{
        CreateResource(Size);
        if (data != nullptr)
        {
            CommandContext::UpdateBufferData(*this, data);
        }
#ifdef _DEBUG
        wchar_t* wname = new wchar_t[strlen(Name) + 1];
        Char2Wchar(Name, wname);
        resource_->SetName(wname);
        delete wname;
#endif // _DEBUG
	}
    void GpuBuffer::CreateResource(size_t Size)
    {
        D3D12_RESOURCE_DESC  resourceDesc = DescriptBuffer();

        usage_ = D3D12_RESOURCE_STATE_COMMON;
        resourceDesc.Width = Size;
        D3D12_HEAP_PROPERTIES heapProps;
        heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
        heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProps.CreationNodeMask = 1;
        heapProps.VisibleNodeMask = 1;
        ASSERT_SUCCEEDED(g_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, usage_, nullptr, IID_PPV_ARGS(&resource_)));

        gpuVirtualAddress_ = resource_->GetGPUVirtualAddress();
    }
    void GpuBuffer::Create(const char* Name, int numElement, int elementSize, const void* initData)
    {
        elementSize_ = elementSize;
        elementCount_ = numElement;
        bufferSize_ = elementSize_ * elementCount_;
        CreateResource(bufferSize_);

#ifdef _DEBUG
        wchar_t* wname = new wchar_t[strlen(Name) + 1];
        Char2Wchar(Name, wname);
        resource_->SetName(wname);
        delete wname;
#endif // _DEBUG

        CommandContext::UpdateBufferData(*this, initData);

    }

    void GpuBuffer::Create(const char* Name, int numElement, int elementSize, const UploadBuffer& initData, UINT offset)
    {
        elementSize_ = elementSize;
        elementCount_ = numElement;
        bufferSize_ = elementSize_ * elementCount_;
        CreateResource(bufferSize_);

#ifdef _DEBUG
        wchar_t* wname = new wchar_t[strlen(Name) + 1];
        Char2Wchar(Name,wname);
        resource_->SetName(wname);
        delete wname;
#endif // _DEBUG


        CommandContext::UpdateBufferData(*this, initData,offset);
    }

    D3D12_VERTEX_BUFFER_VIEW GpuBuffer::VertexBufferView(size_t offset, UINT stride, UINT dataSize)const
    {
        D3D12_VERTEX_BUFFER_VIEW vbo;
        vbo.BufferLocation = gpuVirtualAddress_ + offset;
        vbo.StrideInBytes = stride;
        vbo.SizeInBytes = dataSize;
        return vbo;
    }
    D3D12_INDEX_BUFFER_VIEW GpuBuffer::IndexBufferView(size_t offset, UINT stride, UINT dataSize)const
    {
        D3D12_INDEX_BUFFER_VIEW ibo;
        ibo.BufferLocation = gpuVirtualAddress_ + offset;
        ibo.Format = DXGI_FORMAT_R32_UINT;;
        ibo.SizeInBytes = dataSize;
        return ibo;
    }

    void GpuBuffer::CreateConstBufferView(UINT offset,UINT size)
    {
        size = Math::AlignUp(size, 16);
        D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
        desc.BufferLocation = gpuVirtualAddress_ + offset;
        desc.SizeInBytes = size;

       auto cbvHandle =  g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].Alloc(1);
        g_Device->CreateConstantBufferView(&desc, cbvHandle);
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
        Desc.Width = (UINT64)bufferSize_;
        return Desc;
	}
}