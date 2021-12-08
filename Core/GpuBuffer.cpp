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
    void GpuBuffer::CreateResource(size_t Size, ResourceUsageEnum usage)
    {
        BufferDesc bufferDesc = {};
        bufferDesc.Size = Size;
        
        D3D12_RESOURCE_DESC  resourceDesc = DescriptBuffer(bufferDesc);

        state_ = D3D12_RESOURCE_STATE_COMMON;
        resourceDesc.Width = Size;
        D3D12_HEAP_PROPERTIES heapProps;
        heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
        heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProps.CreationNodeMask = 1;
        heapProps.VisibleNodeMask = 1;
        ASSERT_SUCCEEDED(g_Device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, state_, nullptr, IID_PPV_ARGS(&resource_)));

        gpuVirtualAddress_ = resource_->GetGPUVirtualAddress();
    }

	void GpuBuffer::CreateResource(BufferDesc desc)
	{
		D3D12_RESOURCE_DESC  resourceDesc = DescriptBuffer(desc);

		D3D12_RESOURCE_STATES startState;
		startState = D3D12_RESOURCE_STATE_COMMON;

        D3D12MA::ALLOCATION_DESC allocDesc = {};
        allocDesc.HeapType = D3D12_HEAP_TYPE_DEFAULT;


        if (desc.MemoryUsage == ResourceUsageEnum::RESOURCE_USAGE_GPU_TO_CPU)
        {
            allocDesc.HeapType = D3D12_HEAP_TYPE_READBACK;
            startState = D3D12_RESOURCE_STATE_COPY_DEST;
        }
        else if(desc.MemoryUsage == ResourceUsageEnum::RESOURCE_USAGE_CPU_TO_GPU || 
            desc.MemoryUsage == ResourceUsageEnum::RESOURCE_USAGE_CPU_ONLY)
        {
            allocDesc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
            startState = D3D12_RESOURCE_STATE_GENERIC_READ;
        }

     
        g_GpuResourceAllocator.CreateResource(&allocDesc, &resourceDesc, startState, NULL, &allocation_, &resource_);

	}

	void GpuBuffer::Create(const char* Name, int numElement, int elementSize, const void* initData)
    {
		/*elementSize_ = elementSize;
		elementCount_ = numElement;*/
        bufferSize_ = elementSize * numElement;
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
		/* elementSize_ = elementSize;
		 elementCount_ = numElement;  */
        bufferSize_ = elementSize * numElement;
        CreateResource(bufferSize_);

#ifdef _DEBUG
        wchar_t* wname = new wchar_t[strlen(Name) + 1];
        Char2Wchar(Name,wname);
        resource_->SetName(wname);
        delete wname;
#endif // _DEBUG


        CommandContext::UpdateBufferData(*this, initData,offset);
    }

	void GpuBuffer::Create(const BufferDesc& desc)
	{
        CreateResource(desc);
        if (desc.name)
        {
            wchar_t debugName[128] = {};
            Char2Wchar(desc.name, debugName);
            resource_->SetName(debugName);
        }
        if (desc.InitData)
        {
            CommandContext::UpdateBufferData(*this,desc.InitData);
        }
        if (desc.CreationFlags & BUFFER_CREATION_FLAG_NO_DESCRIPTOR_VIEW_CREATION == 0)
        {
            if ((int)desc.DescriptorType & (int)DescriptorTypeFlags::DESCRIPTOR_TYPE_UNIFORM_BUFFER)
            {
                CreateConstBufferView();
            }
            if ((int)desc.DescriptorType & (int)DescriptorTypeFlags::DESCRIPTOR_TYPE_BUFFER)
            {
                //SRV
            }
			if ((int)desc.DescriptorType & (int)DescriptorTypeFlags::DESCRIPTOR_TYPE_RW_BUFFER)
			{
				//UAV
			}

        }
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
        ibo.Format = DXGI_FORMAT_R32_UINT;
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
	void GpuBuffer::CreateConstBufferView()
	{
		
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
		desc.BufferLocation = gpuVirtualAddress_ ;
		desc.SizeInBytes = bufferSize_;

		auto cbvHandle = g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].Alloc(1);
		g_Device->CreateConstantBufferView(&desc, cbvHandle);
	}
    D3D12_RESOURCE_DESC GpuBuffer::DescriptBuffer(BufferDesc desc)
	{
        D3D12_RESOURCE_DESC Desc = {};
		//Alignment must be 64KB (D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT) or 0, which is effectively 64KB.
        //https://msdn.microsoft.com/en-us/library/windows/desktop/dn903813(v=vs.85).aspx
        Desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
        Desc.DepthOrArraySize = 1;
        Desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        Desc.Flags = D3D12_RESOURCE_FLAG_NONE;
        Desc.Format = DXGI_FORMAT_UNKNOWN;
        Desc.Height = 1;
        Desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        Desc.MipLevels = 1;
        Desc.SampleDesc.Count = 1;
        Desc.SampleDesc.Quality = 0;
        Desc.Width = desc.Size;
        
        return Desc;
	}

	GpuBuffer* GpuBuffer::CreateBuffer(BufferDesc desc)
	{
        GpuBuffer* buffer = new GpuBuffer();
        buffer->Create(desc);
        return buffer;
	}

}