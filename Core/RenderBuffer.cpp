#pragma once
#include"pch.h"
#include"RenderBuffer.h"
namespace YiaEngine
{
	namespace Graphic
	{
		void RenderBuffer::Create(const wchar_t * Name,UINT width, UINT Height, DXGI_FORMAT format, UINT numMip, UINT arraySize, UINT sampleCount)
		{

			

			auto desc = DescribeTex2D(width, Height,  format, arraySize, numMip, TextureFlag::ALLOW_RENDER_TARGET);
			ClearState clearState = ClearState::DefaultState();
			CreateTextureResource(Name,clearState);

			CreateView(format, arraySize, numMip);

			resource_->SetName(Name);

		}
		void RenderBuffer::CreateFromSwapChian(const wchar_t* Name, ID3D12Resource* resource)
		{
			resource_.Attach(resource);
			resource_->SetName(Name);
			state_ = D3D12_RESOURCE_STATE_PRESENT;
			D3D12_RESOURCE_DESC desc = resource->GetDesc();

			describe_.Width = desc.Width;
			describe_.Height = desc.Height;
			describe_.DepthOrArraySize = desc.DepthOrArraySize;
			describe_.Format = desc.Format;
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

	

		
	}
}