#include "pch.h"
#include "Texture.h"

namespace YiaEngine
{
	namespace Graphic
	{
		void Texture::InitializeByImage(ImageData& image, DXGI_FORMAT format)
		{

			auto desc = CD3DX12_RESOURCE_DESC::Tex2D(format, image.Width, image.Height);
			auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			
			ASSERT_SUCCEEDED(Graphic::g_Device->CreateCommittedResource(
				&heap_properties,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&resource_)));
			CommandContext::InitializeTexture(*this,image);
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
			srvHandle = CpuDescriptorAllocator::AllocateDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			Graphic::g_Device->CreateShaderResourceView(resource_.Get(), &srvDesc, srvHandle);
		
		}
	}
}