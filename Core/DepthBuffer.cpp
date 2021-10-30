#include "pch.h"
#include "DepthBuffer.h"
namespace YiaEngine
{
	namespace Graphic
	{
		void YiaEngine::Graphic::DepthBuffer::Create(const wchar_t* name, UINT width, UINT height, DXGI_FORMAT format)
		{
			auto desc = DescribeTex2D(width, height,1, 1, format, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
			D3D12_CLEAR_VALUE clearValue = { };
			CreateTextureResource(name, desc, &clearValue);

			CreateView(format);

			resource_->SetName(name);

		}

		void DepthBuffer::CreateView(DXGI_FORMAT format)
		{
			D3D12_DEPTH_STENCIL_VIEW_DESC desc;
			desc.Format = format;
			if (resource_->GetDesc().SampleDesc.Count == 1)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = 0;
			}
			else
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
			desc.Flags = D3D12_DSV_FLAG_NONE;
			g_Device->CreateDepthStencilView(resource_.Get(), &desc, dsvHandle_[0]);
			desc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH;
			g_Device->CreateDepthStencilView(resource_.Get(), &desc, dsvHandle_[1]);
			
			auto stencilFormat = GetStencilFormat(format);
			//stencil enable
			if (stencilFormat != DXGI_FORMAT_UNKNOWN)
			{
				if (dsvHandle_[2].GetCpuAddress()->ptr == ADDRESS_UNKOWN)
				{
					dsvHandle_[2] = g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_DSV].Alloc(1);
					dsvHandle_[3] = g_CpuDescriptorAllocator[D3D12_DESCRIPTOR_HEAP_TYPE_DSV].Alloc(1);
				}
				desc.Flags = D3D12_DSV_FLAG_READ_ONLY_STENCIL;
				g_Device->CreateDepthStencilView(resource_.Get(), &desc, dsvHandle_[2]);

				desc.Flags = D3D12_DSV_FLAG_READ_ONLY_DEPTH | D3D12_DSV_FLAG_READ_ONLY_STENCIL;
				g_Device->CreateDepthStencilView(resource_.Get(), &desc, dsvHandle_[3]);
			}
			else
			{
				dsvHandle_[2] = dsvHandle_[0];
				dsvHandle_[3] = dsvHandle_[1];
			}
		}

	}
}
