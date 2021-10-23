#pragma once
#include"pch.h"
#include"RenderBuffer.h"
namespace YiaEngine
{
	namespace Graphic
	{
		void RenderBuffer::Create(const wchar_t * name,UINT width, UINT height, DXGI_FORMAT format, UINT numMip, UINT arraySize, UINT sampleCount)
		{
			D3D12_CLEAR_VALUE clearValue;
			clearValue.Format = format;
			clearValue.Color[0] = 1.0f;
			clearValue.Color[1] = 1.0f;
			clearValue.Color[2] = 1.0f;
			clearValue.Color[3] = 1.0f;
			CreateResource(width, height, format, &clearValue, numMip, arraySize, sampleCount);
			CreateView(format, arraySize, numMip);

			resource_->SetName(name);

		}
		void RenderBuffer::CreateFromSwapChian(const wchar_t* name, ID3D12Resource* resource)
		{
			resource_.Attach(resource);
			resource_->SetName(name);
			usage_ = D3D12_RESOURCE_STATE_PRESENT;
			D3D12_RESOURCE_DESC desc = resource->GetDesc();

			width_ = desc.Width;
			height_ = desc.Height;
			arraySize_ = desc.DepthOrArraySize;
			format_ = desc.Format;
			rtvHandle_ = g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_RTV].Alloc(1);
			Graphic::g_Device->CreateRenderTargetView(resource_.Get(), nullptr, rtvHandle_);
	

		}
		void RenderBuffer::CreateView(DXGI_FORMAT format, UINT arraySize, UINT numMips)
		{
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			if (arraySize > 1)
			{
				srvDesc.Texture2DArray.ArraySize = arraySize;
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				srvDesc.Texture2DArray.FirstArraySlice = 0;
				srvDesc.Texture2DArray.MipLevels = numMips;
				srvDesc.Texture2DArray.MostDetailedMip = 0;

				rtvDesc.Texture2DArray.ArraySize = arraySize;
				rtvDesc.Texture2DArray.FirstArraySlice = 0;
				rtvDesc.Texture2DArray.MipSlice = 0;
			}
			else
			{
				rtvDesc.Texture2D.MipSlice = 0;
				rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

				srvDesc.Texture2D.MipLevels = numMips;
				srvDesc.Texture2D.MostDetailedMip = 0;
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				
			}
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = format;
			rtvDesc.Format = format;
			if (srvHandle_.GetCpuAddress()->ptr == ADDRESS_NULL)
			{
				srvHandle_ = g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].Alloc(1);
				rtvHandle_ = g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_RTV].Alloc(1);
			}
			Graphic::g_Device->CreateShaderResourceView(resource_.Get(), &srvDesc, srvHandle_);
			Graphic::g_Device->CreateRenderTargetView(resource_.Get(), &rtvDesc, rtvHandle_);
		}

	

		void RenderBuffer::CreateResource(UINT width, UINT height, DXGI_FORMAT format, const D3D12_CLEAR_VALUE* clearValue, UINT arraySize, UINT sampleCount, UINT numMip)
		{
			auto desc = CD3DX12_RESOURCE_DESC::Tex2D(format, width, height, arraySize, numMip, sampleCount, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
			auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			ASSERT_SUCCEEDED(Graphic::g_Device->CreateCommittedResource(
				&heap_properties,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_COMMON,
				clearValue,
				IID_PPV_ARGS(&resource_)));
			usage_ = D3D12_RESOURCE_STATE_COMMON;
			
			gpuVirtualAddress_ = ADDRESS_NULL;	//用不到该属性
		
			width_ = width;
			height_ = height;
			format_ = format;
			numMips_ = numMip;
			arraySize_ = arraySize;
		}
	}
}