#include "pch.h"
#include "DepthBuffer.h"
namespace YiaEngine
{
	namespace Graphic
	{
		DepthBuffer::DepthBuffer()
		{

		}
		void YiaEngine::Graphic::DepthBuffer::Create(const wchar_t* Name, UINT width, UINT Height, DXGI_FORMAT format)
		{
			auto desc = DescribeTex2D(width, Height,1, 1, format, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
			D3D12_CLEAR_VALUE clearValue = { };
			clearValue.DepthStencil.Depth = 1;
			clearValue.Format = format;
			CreateTextureResource(Name, desc, &clearValue);

			CreateView(format);

			resource_->SetName(Name);

		}

		void DepthBuffer::CreateView(DXGI_FORMAT format)
		{
			D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
			dsvDesc.Format = format;
			if (resource_->GetDesc().SampleDesc.Count == 1)
			{
				dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				dsvDesc.Texture2D.MipSlice = 0;
			}
			else
				dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
			dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
			if (dsvHandle_[0].CpuDescriptorValuePtr() == ADDRESS_NULL)
			{
				dsvHandle_[0] = g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_DSV].Alloc(1);
				dsvHandle_[1] = g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_DSV].Alloc(1);
			}
			g_Device->CreateDepthStencilView(resource_.Get(), &dsvDesc, dsvHandle_[0]);
			dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH;
			g_Device->CreateDepthStencilView(resource_.Get(), &dsvDesc, dsvHandle_[1]);
			
			auto stencilFormat = GetStencilFormat(format);
			//stencil enable
			if (stencilFormat != DXGI_FORMAT_UNKNOWN)
			{
				if (dsvHandle_[2].GetCpuAddress()->ptr == ADDRESS_NULL)
				{
					dsvHandle_[2] = g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_DSV].Alloc(1);
					dsvHandle_[3] = g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_DSV].Alloc(1);
				}
				dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_STENCIL;
				g_Device->CreateDepthStencilView(resource_.Get(), &dsvDesc, dsvHandle_[2]);

				dsvDesc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH | D3D12_DSV_FLAG_READ_ONLY_STENCIL;
				g_Device->CreateDepthStencilView(resource_.Get(), &dsvDesc, dsvHandle_[3]);
			}
			else
			{
				dsvHandle_[2] = dsvHandle_[0];
				dsvHandle_[3] = dsvHandle_[1];
			}



			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			if (dsvDesc.ViewDimension == D3D12_DSV_DIMENSION_TEXTURE2D)
			{
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MipLevels = 1;
			}
			else
			{
				srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
			}
			srvDesc.Format = GetDepthFormat(format);
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			if(depthSrvHandle_.CpuDescriptorValuePtr()== ADDRESS_NULL)
				depthSrvHandle_ = g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].Alloc(1);
			g_Device->CreateShaderResourceView(resource_.Get(), &srvDesc,depthSrvHandle_);
			
			if (stencilFormat != DXGI_FORMAT_UNKNOWN)
			{
				if (stencilSrvHandle_.CpuDescriptorValuePtr() == ADDRESS_NULL)
					stencilSrvHandle_ = g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV].Alloc(1);
				srvDesc.Format = stencilFormat;
				g_Device->CreateShaderResourceView(resource_.Get(), &srvDesc, stencilSrvHandle_);
			}
		}
		DXGI_FORMAT DepthBuffer::GetDepthFormat(DXGI_FORMAT defaultFormat)
		{
			switch (defaultFormat)
			{
				// 32-bit Z w/ Stencil
			case DXGI_FORMAT_R32G8X24_TYPELESS:
			case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
			case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
				return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;

				// No Stencil
			case DXGI_FORMAT_R32_TYPELESS:
			case DXGI_FORMAT_D32_FLOAT:
			case DXGI_FORMAT_R32_FLOAT:
				return DXGI_FORMAT_R32_FLOAT;

				// 24-bit Z
			case DXGI_FORMAT_R24G8_TYPELESS:
			case DXGI_FORMAT_D24_UNORM_S8_UINT:
			case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
			case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
				return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

				// 16-bit Z w/o Stencil
			case DXGI_FORMAT_R16_TYPELESS:
			case DXGI_FORMAT_D16_UNORM:
			case DXGI_FORMAT_R16_UNORM:
				return DXGI_FORMAT_R16_UNORM;

			default:
				return DXGI_FORMAT_UNKNOWN;
			}
		}
	}
}
