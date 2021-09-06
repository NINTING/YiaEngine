#pragma once

#include"Graphic.h"
#include"GpuResource.h"

#include"Common/ImageParser/Image.h"
#include "Texture.h"
#include"Core/CommandContext.h"
#include"Core/DescriptorHeap.h"
namespace YiaEngine
{
	namespace Graphic
	{
		void Texture::InitializeByImage(Image* image)
		{
			auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R8G8B8A8_UNORM, image->width, image->height);
			auto heap_properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
			Graphic::GpuResource target;
			ASSERT_SUCCEEDED(Graphic::g_Device->CreateCommittedResource(
				&heap_properties,
				D3D12_HEAP_FLAG_NONE,
				&desc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&resource_)));

			auto copy_command = Graphic::CommandContext::Begin();



			UINT64 textureUploadBufferSize;
		
			textureUploadBufferSize = GetRequiredIntermediateSize(resource_.Get(), 0, 1);
			AllocateBuffer uplaod_buffer = copy_command->GetTemraryUploadBuffer(textureUploadBufferSize);

			D3D12_SUBRESOURCE_DATA initData = { image->pData, image->pitch,image->data_size };
			ASSERT_SUCCEEDED(UpdateSubresources<1>(copy_command->command_list(), resource_.Get(), uplaod_buffer.GetResource(), 0, 0, 1, &initData));

			copy_command->TransitionBarrier(*this, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
			
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvDesc.Format = desc.Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			//UINT srv_desc_size =   Graphic::g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//CD3DX12_CPU_DESCRIPTOR_HANDLE desc_handle(descriptor_heap.Alloc(1), 1, srv_desc_size);
			Graphic::g_Device->CreateShaderResourceView(resource_.Get(), &srvDesc, CpuDescriptorAllocator::AllocateDescriptorHandle(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

			copy_command->End();

		}
	}
}