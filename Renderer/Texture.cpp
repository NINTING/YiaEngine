#include "pch.h"
#include "Texture.h"

namespace YiaEngine
{
	namespace Graphic
	{
		void Texture::InitializeByImage(ImageData& image, DXGI_FORMAT format)
		{
			auto desc = DescribeTex2D(image.Width, image.Height, format);
			
			
			CreateTextureResource(L"Tex_Image");

			CommandContext::InitializeTexture(*this,image);
			CreateView();
		}
		void Texture::SetDebugName(const wchar_t* name)
		{
			resource_->SetName(name);
		}
		void Texture::CreateView()
		{
			auto desc = resource_->GetDesc();

			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = desc.Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			srvHandle_ = CpuDescriptorAllocator::AllocateDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			Graphic::g_Device->CreateShaderResourceView(resource_.Get(), &srvDesc, srvHandle_);
		
		}
	}
}