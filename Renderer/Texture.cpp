#include "pch.h"
#include "Texture.h"

namespace YiaEngine
{
	namespace Graphic
	{
		void Texture::InitializeByImage(ImageData& image)
		{
			CommandContext::InitializeTexture(*this,image);
			//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			//srvDesc.Format = desc.Format;
			//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			//srvDesc.Texture2D.MipLevels = 1;
			////UINT srv_desc_size =   Graphic::g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			////CD3DX12_CPU_DESCRIPTOR_HANDLE desc_handle(descriptor_heap.Alloc(1), 1, srv_desc_size);
			//handle = CpuDescriptorAllocator::AllocateDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			//Graphic::g_Device->CreateShaderResourceView(resource_.Get(), &srvDesc, handle);
		}
	}
}